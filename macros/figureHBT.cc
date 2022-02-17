#include <iostream>
#include <TFile.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TString.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TAxis.h>
#include <TPaveText.h>
#include <TText.h>
#include <TLegend.h>
#include <TColor.h>

using namespace std;

const int nPads = 6;
const EColor color[] = {kGreen,kRed,kOrange};

void preparegraph(TGraphErrors *aGE)
{
    aGE->SetTitle("");
    aGE->SetMarkerStyle(20);
    aGE->GetXaxis()->SetLimits(30.,460.);
}

void preparegraph(TGraphAsymmErrors *aGE, int col, bool isLambda = false)
{
    double min,max;

    if(isLambda)
    {
        min = 0.0;
        max = 1.5;
    }
    else
    {
        min = 1.9;
        max = 8.1;
    }
    aGE->SetTitle("");
    aGE->SetMarkerStyle(20);
    if(col == 0)
    {
        aGE->SetMarkerColor(color[col]+2);
        aGE->SetLineColor(color[col]+2);
    }
    else
    {
        aGE->SetMarkerColor(color[col]);
        aGE->SetLineColor(color[col]);
    }
    aGE->SetMinimum(min);
    aGE->SetMaximum(max);
    aGE->GetXaxis()->SetLimits(30.,460.);
}

void figureHBT()
{  
    gStyle->SetPalette(kDeepSea);

    const TString folderPath = "/home/jedkol/Downloads/modelSR/HBT/MotoMoto";
    const TString tName[] = {"#pi^{+} - #pi^{+}","#pi^{-} - #pi^{-}","#pi^{0} - #pi^{0}"};
    const TString pName[] = {"pipi","piMpiM","pi0pi0"};
    const TString fName[] = {"pipi","pimpim","pi0pi0"};
    const TString epsType[] = {"E0","E1","E2","E3","E4","E5","E6"};
    const TString modelName[] = {"gRinv_5_12","gRout_5_12","gRside_5_12","gRlong_5_12","gLambdaInv_5_12","gLambdaOSL_5_12"};
    const TString expName[] = {"geExpInv","geExpO","geExpS","geExpL","geExpLamInv","geLamosl"};
    const TString rName[] = {"R_{inv}","R_{out}","R_{side}","R_{long}","#lambda_{inv}","#lambda_{osl}"};
    const double epsVal[] = {0.0,-0.1,-0.2,-0.3,-0.4,-0.5,-0.6};

    TGraphErrors *gModel[7][nPads];
    TGraphAsymmErrors *gaeExp[3][nPads];
    TFile *fExpData,*fModelData;
    TPaveText *rType[nPads];
    TText *txt[nPads];
    TLegend *leg;
    TCanvas *c1;
    int maxExpGraph;
    TString drawCom;

    for(int i = 0; i < nPads; i ++) //prepare text labels
    {
        rType[i] = new TPaveText(0.15,0.15,0.25,0.25,"NDC");
        rType[i]->SetFillColor(0);
        txt[i] = rType[i]->AddText(rName[i].Data());
        txt[i]->SetTextColor(2);
    }

    for(int i = 0; i < 3; i++)  //input experimental data
    {
        fExpData = TFile::Open(Form("/home/jedkol/Downloads/modelSR/macrosForHBT/%sR3DExp.root",fName[i].Data()),"read");
        if(fExpData->IsOpen())
        {
            cout << "File " << Form("%sR3DExp.root",fName[i].Data()) << " is open" << endl;

            for(int j = 0; j < nPads; j++)
                gaeExp[i][j] = (TGraphAsymmErrors*) fExpData->Get(expName[j].Data());
        }

        fExpData->Close();
    }

    for(int i = 0; i < 7; i++)  //input model data
    {
        fModelData = TFile::Open(Form("%s/H160%sD4femto/parameterFit%s.root",folderPath.Data(),epsType[i].Data(),pName[0].Data()),"read");
        if(fModelData->IsOpen())
        {
            cout << "File " << Form("parameterFit%s.root",pName[0].Data()) << " is open" << endl;

            for(int j = 0; j < nPads; j++)
                gModel[i][j] = (TGraphErrors*) fModelData->Get(modelName[j].Data());
        }

        fModelData->Close();
    }

    c1 = new TCanvas("c1","",900,900);
    c1->Divide(2,3,0,0);

    leg = new TLegend(0.7,0.5,1.,1.);

    for(int i = 0; i < nPads; i++)
    {
        c1->cd(i+1);
        if(i < 4)
            gPad->SetTickx(2);
        else
            gaeExp[0][i]->GetXaxis()->SetTitle("k_{T} [MeV]");

        if(i == 1 || i == 3 || i == 5)
            gPad->SetTicky(2);
        else
            gaeExp[0][i]->GetYaxis()->SetTitle(Form("%s %s",rName[i].Data(),"[fm]"));

        if(i < 4)
            maxExpGraph = 3;
        else    
            maxExpGraph = 2;

        for(int j = 0; j < maxExpGraph; j++)
        {
            if(gaeExp[j][i] != nullptr)
            {
                if(i < 4)
                    preparegraph(gaeExp[j][i],j);
                else 
                    preparegraph(gaeExp[j][i],j,true);
                if(j == 0)
                    drawCom = "ap";
                else
                    drawCom = "same p";

                gaeExp[j][i]->Draw(drawCom.Data());

                if(i == 0)
                    leg->AddEntry(gaeExp[j][i],tName[j].Data(),"p");
            }
        }
        rType[i]->Draw("same");

        for(int j = 0; j < 7; j++)
        {
            preparegraph(gModel[j][i]);
            gModel[j][i]->Draw("same lpx plc pmc");

            if(i == 0)
                leg->AddEntry(gModel[j][i],Form("#epsilon = %.1f",epsVal[j]),"l");
        }
    }

    c1->cd(1);
    leg->Draw("same");

    c1->Draw();
}