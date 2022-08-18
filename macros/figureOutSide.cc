// ==========================================================================
// TherminaCorr - an extension of THERMINATOR 2 therm2_hbtfit functionality
// Author: Jedrzej Kolas
// Code at: https://github.com/JohnTheBlindMilkman/TherminaCorr
// ==========================================================================

#include <iostream>
#include <TFile.h>
#include <TH2D.h>
#include <TMath.h>
#include <TStyle.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TString.h>
#include "/home/jedkol/Downloads/therminator2_HADES_SR/macro/events2chain.C"

using namespace std;

void progress(Long64_t entry, Long64_t noEntries)
{
    if(! (entry%1000))
    {
        float progress = (float) entry/noEntries;
        cout << "Progress: " << int(progress * 100) << " %\r";
        cout.flush();
    }
}

void preparePlot(TH2D *h)
{
    h->GetXaxis()->SetLabelSize(0.06);
    h->GetXaxis()->SetTitleSize(0.06);
    h->GetXaxis()->SetTitleOffset(1.);
    h->GetXaxis()->SetNdivisions(505);

    h->GetYaxis()->SetLabelSize(0.06);
    h->GetYaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetTitleOffset(1.);
    h->GetYaxis()->SetNdivisions(505);
}

struct TVec
{
    double px;
    double py;
    double pz;
};

double dotProd(TVec v1, TVec v2)
{
    return v1.px*v2.px+v1.py*v2.py+v1.pz*v2.pz;
}

TVec vecProd(TVec v1, TVec v2)
{
    TVec newV;
    newV.px = v1.py*v2.pz-v1.pz*v2.py;
    newV.py = v1.pz*v2.px-v1.px*v2.pz;
    newV.pz = v1.px*v2.py-v1.py*v2.px;

    return newV;
}

double mag(TVec v)
{
    return TMath::Sqrt(v.px*v.px+v.py*v.py+v.pz*v.pz);
}

void figureOutSide()
{
    gStyle->SetOptStat(0);
    gStyle->SetPalette(kPastel);

    const TString aEventDir = "/home/jedkol/lustre/hades/user/kjedrzej/HubbDeltMotoMoto/H165E0D4femto/";
    const TString sOptDir = "/home/jedkol/Downloads/modelSR/outThesisGraphs/";
    const int nBins = 150, minBin = -25, maxBin = 25;
    int partPID,partParPID;
    double ene,Px,Py,Pz,x,y,pt,mt,xOut,xSide;
    static ParticleCoor Particle;
    Long64_t Events,Entries;
    TVec posVec,momVec;
    TChain* Chain = events2chain(aEventDir, 5, &Particle, &Events);

    TH2D *hPrim = new TH2D("hPrim","Primordial;x_{side} [fm];x_{out} [fm]",nBins,minBin,maxBin,nBins,minBin,maxBin);
    TH2D *hRes = new TH2D("hRes","Resonances;x_{side} [fm];x_{out} [fm]",nBins,minBin,maxBin,nBins,minBin,maxBin);
    TH2D *hAll = new TH2D("hAll","Total;x_{side} [fm];x_{out} [fm]",nBins,minBin,maxBin,nBins,minBin,maxBin);
    
    Entries = Chain->GetEntries();
    for(Long64_t i = 0; i < Entries; i++) 
    {
        progress(i,Entries);

        Chain->GetEntry(i);

        partPID = Particle.pid;
        ene = Particle.e;
        Px = Particle.px;
        Py = Particle.py;
        Pz = Particle.pz;

        if(partPID != 211 && partPID != -211)
            continue;
        if(ene == Pz)
            continue;

        x = Particle.x;
        y = Particle.y;
        partParPID = Particle.fatherpid;
        pt  = TMath::Sqrt(Px*Px+Py*Py);
        if(pt > 0.12 && pt < 0.16)
        {
            posVec = {x,y,0};
            momVec = {Px,Py,0};

            xOut = dotProd(posVec,momVec)/pt;
            xSide = vecProd(posVec,momVec).pz/pt;
            if(partPID == partParPID)
                hPrim->Fill(xSide,xOut);
            else
                hRes->Fill(xSide,xOut);

            hAll->Fill(xSide,xOut);
        }
    }
    cout << endl;

    TCanvas *c = new TCanvas("c","",1800,600);
    c->Divide(3,1);
    c->cd(1)->SetMargin(0.15,0.15,0.15,0.15);
    preparePlot(hPrim);
    hPrim->Draw("colz");
    c->cd(2)->SetMargin(0.15,0.15,0.15,0.15);
    preparePlot(hRes);
    hRes->Draw("colz");
    c->cd(3)->SetMargin(0.15,0.15,0.15,0.15);
    preparePlot(hAll);
    hAll->Draw("colz");

    c->SaveAs(sOptDir + "outSideCaseC.eps");
}