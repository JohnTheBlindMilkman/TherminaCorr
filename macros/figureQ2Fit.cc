#include <iostream>
#include <fstream>
#include <TCanvas.h>
#include <TGraph.h>
#include <TString.h>
#include <TStyle.h>
#include <TColor.h>
#include <TSystem.h>
#include <TLegend.h>
#include <TAxis.h>

using namespace std;

void figureQ2Fit()
{
    const int noE = 7;
    const TString folderPath = "/home/jedkol/Downloads/modelSR/HBT/MotoMoto";
    const TString epsType[noE] = {"E6","E5","E4","E3","E2","E1","E0"};
    const TString modelName[] = {"gRinv_5_12","gRout_5_12","gRside_5_12","gRlong_5_12"};
    const TString rName[] = {"R_{inv}","R_{out}","R_{side}","R_{long}"};
    const double epsVal[noE] = {-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0.0};

    ifstream ifFile;
    TGraph *gRes[4];
    TCanvas *canv;
    TString sName;
    double yVal[4][noE];
    TLegend *tLeg;

    for(int i = 0; i < noE; i++)
    {
        ifFile.open(Form("%s/H160%sD4femto/Q2result.txt",folderPath.Data(),epsType[i].Data()));
        ifFile >> sName >> sName;
        for(int j = 0; j < 4; j++)
            ifFile >> sName >> yVal[j][i];

        ifFile.close();
    }

    canv = new TCanvas("canv","canv",800,1000);
    canv->Divide(1,4);
    //tLeg = new TLegend(0.8,0.7,0.9,0.9);

    for(int i = 0; i < 4; i++)
    {
        canv->cd(i+1);
        gRes[i] = new TGraph(noE,epsVal,&yVal[i][0]);
        gRes[i]->SetMarkerStyle(20);
        gRes[i]->SetTitle(rName[i].Data());
        gRes[i]->GetXaxis()->SetRangeUser(-0.7,0.1);
        //tLeg->AddEntry(gRes[i],rName[i].Data());
        //if(i == 0)
            gRes[i]->Draw("apl plc pmc");
        //else
        //gRes[i]->Draw("same pl plc pmc");
        //canv->Modified();
        //canv->Update();
    }
    //tLeg->Draw("same");
}