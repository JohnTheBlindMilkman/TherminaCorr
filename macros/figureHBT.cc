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
#include <TVirtualPad.h>

using namespace std;

const int nPads = 6;
const EColor color[] = {kGreen,kRed,kOrange};

void preparegraph(TGraphErrors *aGE)
{
    aGE->SetTitle("");
    aGE->SetMarkerStyle(20);
    aGE->GetXaxis()->SetLimits(30.,449.999);
}

void preparegraph(TGraphAsymmErrors *aGE, int col, bool isLambda = false)
{
    double min,max;

    if(isLambda)
    {
        min = -0.2;
        max = 1.7;
    }
    else
    {
        min = 1.7;
        max = 8.3;
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
    aGE->GetXaxis()->SetLimits(30.,449.999);
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
    const double Xmin[] = {0.35,0.15,0.35,0.15,0.35,0.15}, Ymin[] = {0.15,0.15,0.15,0.75,0.35,0.35}, Xmax[] = {0.45,0.25,0.45,0.25,0.45,0.25}, Ymax[] = {0.25,0.25,0.25,0.85,0.45,0.45};
    const int NoExp = 3, NoMod = 7;

    TGraphErrors *gModel[NoMod][nPads];
    TGraphAsymmErrors *gaeExp[NoExp][nPads];
    TFile *fExpData,*fModelData;
    TPaveText *rType[nPads];
    TText *txt[nPads];
    TLegend *legExp,*legMod;
    TCanvas *c1;
    TVirtualPad *tvp;
    int maxExpGraph;
    TString drawCom;

    for(int i = 0; i < nPads; i ++) //prepare text labels
    {
        rType[i] = new TPaveText(Xmin[i],Ymin[i],Xmax[i],Ymax[i],"NDC");
        rType[i]->SetFillColor(0);
        txt[i] = rType[i]->AddText(rName[i].Data());
        txt[i]->SetTextColor(2);
    }

    for(int i = 0; i < NoExp; i++)  //input experimental data
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

    for(int i = 0; i < NoMod; i++)  //input model data
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
    c1->SetMargin(0.,0.,0.2,0.2);
    c1->Divide(2,3,0,0);

    legExp = new TLegend(0.8,0.7,1.,1.);
    legMod = new TLegend(0.6,0.5,0.8,1.);

    for(int i = 0; i < nPads; i++)
    {
        tvp = c1->cd(i+1);
        tvp->SetGrid(1,1);
        gaeExp[0][i]->GetXaxis()->SetNdivisions(508);
        gaeExp[0][i]->GetYaxis()->SetNdivisions(505);

        gaeExp[0][i]->GetYaxis()->SetLabelFont(62);
        gaeExp[0][i]->GetYaxis()->SetLabelOffset();
            
        if(i > 3)
        {
            gaeExp[0][i]->GetXaxis()->SetTitle("k_{T} [MeV]");
            gaeExp[0][i]->GetXaxis()->SetTitleSize(0.08);
            gaeExp[0][i]->GetXaxis()->SetTitleOffset(1.1);
            gaeExp[0][i]->GetXaxis()->SetLabelSize(0.08);
            gaeExp[0][i]->GetXaxis()->SetLabelFont(62);
            gaeExp[0][i]->GetXaxis()->SetLabelOffset();

            gaeExp[0][i]->GetYaxis()->SetTitleSize(0.09);
            gaeExp[0][i]->GetYaxis()->SetLabelSize(0.08);
        }
        else
        {
            gaeExp[0][i]->GetYaxis()->SetTitleSize(0.1);
            gaeExp[0][i]->GetYaxis()->SetLabelSize(0.1);
        }

        if(i == 0 || i == 2 || i == 4)
        {
            gaeExp[0][i]->GetYaxis()->SetTitle(Form("%s %s",rName[i].Data(),"[fm]"));
            gaeExp[0][i]->GetYaxis()->SetTitleOffset(0.8);
            gaeExp[0][i]->GetYaxis()->CenterTitle(true);

            tvp->SetLeftMargin(0.2);
        }
        else
        {
            tvp->SetRightMargin(0.2);
        }

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
                {
                    drawCom = "ap";
                    if(i == 1 || i == 3 || i == 5)
                        drawCom += " y+";
                }
                    
                else
                    drawCom = "same p";

                gaeExp[j][i]->Draw(drawCom.Data());

                if(i == 0)
                    legExp->AddEntry(gaeExp[j][i],tName[j].Data(),"p");
            }
        }
        rType[i]->Draw("same");

        for(int j = 0; j < NoMod; j++)
        {
            preparegraph(gModel[j][i]);
            gModel[j][i]->Draw("same lp plc pmc");

            if(i == 0)
                legMod->AddEntry(gModel[j][i],Form("#epsilon = %.1f",epsVal[j]),"lp");
        }
    }

    c1->cd(1);
    legExp->Draw("same");

    c1->cd(2);
    legMod->Draw("same");

    c1->Draw();
}