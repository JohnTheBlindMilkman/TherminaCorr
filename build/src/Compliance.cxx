#include "Compliance.h"
#include "THGlobal.h"

using namespace std;

Compliance::Compliance():mExpNames(0),mMinBin(0),mMaxBin(0)
{
}

Compliance::Compliance(TString sModFilePath, TString sModName, TString sExpFilePath, TString sExpName)
{
    mEFile = TFile::Open(sModFilePath);
    if(!mEFile->IsOpen() || mEFile->IsZombie()) 
    {
        PRINT_MESSAGE("<Compliance::Compliance>\tFile "<<sModFilePath<<" not found.");
        exit(_ERROR_GENERAL_FILE_NOT_FOUND_);
    }

    mMFile = TFile::Open(sExpFilePath);
    if(!mMFile->IsOpen() || mMFile->IsZombie()) 
    {
        PRINT_MESSAGE("<Compliance::Compliance>\tFile "<<sExpFilePath<<" not found.");
        exit(_ERROR_GENERAL_FILE_NOT_FOUND_);
    }

    mMod = (TGraph*) mMFile->Get(sModName);
    mExp = (TGraph*) mEFile->Get(sExpName);
}

Compliance::~Compliance()
{
    if(mEFile->IsOpen())
        mEFile->Close();

    if(mMFile->IsOpen())
        mMFile->Close();
}

void Compliance::printResult(bool moreInfo)
{
    double totDif;
    PRINT_MESSAGE("============== Q2 Test Result ==============");
    PRINT_MESSAGE(Form("Parameter:\t%s",mExp->GetName()));
    PRINT_MESSAGE(Form("Q2:\t%f",Q2Test(totDif)));
    if(moreInfo)
        PRINT_MESSAGE(Form("Total difference on x axis:\t%f",totDif));
    PRINT_MESSAGE("============================================");
}

bool Compliance::getModelName(TString expName, int iter, int minkT, int maxkT, TString &modName)
{
    if(expName == "none")
    {
        modName = "";
        return false;
    }
    if(iter > 1)
        iter++;
    
    modName = TString::Format("g%s_%i_%i",sParNames[iter].Data(),minkT,maxkT);
    return true;
}

double Compliance::Q2Test(double &totDiff)
{
    const int elem = mMod->GetN();
    int iter;
    double xMod,yMod,xExp,yExp,Q2;

    totDiff = 0;

    for(int i = 0; i < elem; i++)
    {
        mMod->GetPoint(i,xMod,yMod);
        totDiff += getClosest(xMod,iter,xExp,yExp);
        if(yExp > 0)
            Q2 += (yMod*yMod - 2*yMod*yExp + yExp*yExp)/(yExp*yExp);
    }

    return fabs(Q2)/elem;
}

double Compliance::getClosest(double xVal, int &i, double &x, double &y)
{
    const int elem = mExp->GetN();
    double xx,yy;
    double dist,distTemp;

    mExp->GetPoint(0,x,y);
    dist = fabs(xVal-x);
    if(dist < numeric_limits<double>::epsilon()) // baisically means "if(xVal == x)" but for floating point variables
        return dist;

    for(int ii = 1; ii < elem; ii++)
    {
        mExp->GetPoint(ii,xx,yy);
        distTemp = fabs(xVal-x);
        if(distTemp < dist)
        {
            dist = distTemp;
            i = ii;
            x = xx;
            y = yy;
        }
    }
    return dist;
}

