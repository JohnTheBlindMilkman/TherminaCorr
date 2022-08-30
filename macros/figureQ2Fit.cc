// ==========================================================================
// TherminaCorr - an extension of THERMINATOR 2 therm2_hbtfit functionality
// Author: Jedrzej Kolas
// Code at: https://github.com/JohnTheBlindMilkman/TherminaCorr
// ==========================================================================

#include <iostream>
#include <fstream>
#include <TCanvas.h>
#include <TGraph.h>
#include <TH2.h>
#include <TString.h>
#include <TStyle.h>
#include <TColor.h>
#include <TSystem.h>
#include <TLegend.h>
#include <TAxis.h>

using namespace std;

void figureQ2Fit()
{
    gStyle->SetOptStat(0);
    gStyle->SetPalette(kPastel);
    
    const int noE = 7;
    const TString folderPath = "/home/jedkol/Downloads/modelSR";
    const TString epsType[] = {"E6","E5","E4","E3","E2","E1","E0"};
    const TString modelName[] = {"gRinv_5_12","gRout_5_12","gRside_5_12","gRlong_5_12"};
    const TString rName[] = {"R_{inv}","R_{out}","R_{side}","R_{long}"};
    const double epsVal[] = {-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0.0};

    ifstream ifFile;
    TGraph *gRes[4];
    TCanvas *canv1;
    TString sName;
    double yVal[4][noE];
    TLegend *tLeg;

    for(int i = 0; i < noE; i++)
    {
        ifFile.open(Form("%s/MotoMotoNoLimit/H170%sD5femto/Chi2result.txt",folderPath.Data(),epsType[i].Data()));
        ifFile >> sName >> sName;
        for(int j = 0; j < 4; j++)
            ifFile >> sName >> yVal[j][i];

        ifFile.close();
    }    

    canv1 = new TCanvas("canv1","canv1",800,1000);
    canv1->SetMargin(0.15,0.05,0.15,0.05);
    tLeg = new TLegend(0.8,0.75,0.95,0.95,"","NDC");
    tLeg->SetBorderSize(0);
    tLeg->SetFillStyle(0);

    for(int i = 0; i < 4; i++)
    {
        gRes[i] = new TGraph(noE,epsVal,&yVal[i][0]);
        gRes[i]->SetLineColor(kRed);
        gRes[i]->SetLineWidth(5);
        gRes[i]->SetTitle(";#epsilon;#chi^{2}/ndf");
        gRes[i]->GetYaxis()->SetRangeUser(0,300);
        gRes[i]->GetXaxis()->SetRangeUser(-0.7,0.1);
        
        if(i == 0)
            gRes[i]->Draw("ac plc");
        else    
            gRes[i]->Draw("c plc same");

        tLeg->AddEntry(gRes[i],rName[i].Data(),"l");
    }

    tLeg->Draw("same");

    canv1->SaveAs(folderPath + "/outThesisGraphs/fitCaseD.eps");
}