#include <TPad.h>

#include "HBTFit.h"

HBTFit::HBTFit()
{
}

HBTFit::~HBTFit()
{
}

void HBTFit::getfitprojc(TH3D *expden, TH3D **projhist, TF3 *funqk)
{
    (*projhist) = new TH3D(*expden);

    for (int q1int = 1; q1int <= expden->GetNbinsX(); q1int++)
        for (int q2int = 1; q2int <= expden->GetNbinsY(); q2int++) 
            for (int q3int = 1; q3int <= expden->GetNbinsZ(); q3int++) 
                (*projhist)->SetBinContent(q1int, q2int, q3int,expden->GetBinContent(q1int, q2int, q3int) * funqk->Eval(expden->GetXaxis()->GetBinCenter(q1int),expden->GetYaxis()->GetBinCenter(q2int),expden->GetZaxis()->GetBinCenter(q3int)));

}

void HBTFit::preparepad()
{
    gPad->SetFillStyle(4000);
    gPad->SetFillColor(0);
    gPad->SetRightMargin(0.02);
    gPad->SetTopMargin(0.03);
    gPad->SetBottomMargin(0.12);
    gPad->SetGridy();
}

void HBTFit::preparehist(TH1D *hist, int type)
{
    if ((type==2) || (type == 3)) 
    {
        if (type == 2)
            hist->SetLineColor(4);
        else
            hist->SetLineColor(8);
        hist->SetLineWidth(2);
        hist->SetLineStyle(2);
    }
    else 
    {
        hist->SetMarkerSize(1.0);
        hist->SetMarkerColor(type+1);
        hist->SetMarkerStyle(type==0?8:24);
        hist->SetTitle("");
        hist->SetMinimum(hist->GetMinimum()*0.9);
        hist->SetMaximum(hist->GetMaximum()*1.1);
        hist->GetXaxis()->SetLabelSize(0.055);
        hist->GetYaxis()->SetLabelSize(0.055);
        hist->GetXaxis()->SetTitleSize(0.055);
        hist->GetYaxis()->SetTitleSize(0.055);
        hist->GetYaxis()->SetTitleOffset(0.8);
    }
}

TH1D* HBTFit::getproj(TH3D *numq, TH3D *denq, int nproj, int wbin, double norm)
{
    Double_t intnexp = norm;
    Double_t intdexp = 1.0;
    TH1D *hpxc,*hpyc,*hpzc;

    Int_t binc = numq->GetXaxis()->FindFixBin(0.0);
    Int_t binm = binc + wbin;

    TH1D *denbuf,*numbuf;

    for (int ia=nproj; ia<=nproj; ia++) 
    {

        numq->GetXaxis()->SetRange(binc, binm);
        numq->GetYaxis()->SetRange(binc, binm);
        numq->GetZaxis()->SetRange(binc, binm);
        denq->GetXaxis()->SetRange(binc, binm);
        denq->GetYaxis()->SetRange(binc, binm);
        denq->GetZaxis()->SetRange(binc, binm);
        switch (ia) 
        {
            case 0:
                denq->GetXaxis()->SetRange(1,numq->GetNbinsX());
                numq->GetXaxis()->SetRange(1,numq->GetNbinsX());
                denbuf = new TH1D(*((TH1D *) denq->Project3D("x")));
                numbuf = new TH1D(*((TH1D *) numq->Project3D("x")));
                hpxc = new TH1D(*numbuf);
                hpxc->Sumw2();
                hpxc->Reset("ICE");
                hpxc->Divide(numbuf, denbuf, 1.0, 1.0, "");
                for (int iter=1; iter<hpxc->GetNbinsX(); iter++)
                    if (numbuf->GetBinContent(iter)) 
                    {
                        Double_t dn = numbuf->GetBinError(iter);
                        Double_t an = numbuf->GetBinContent(iter);
                        Double_t dd = denbuf->GetBinError(iter);
                        Double_t ad = denbuf->GetBinContent(iter);
                        hpxc->SetBinError(iter, TMath::Sqrt((dn*dn*ad*ad + dd*dd*an*an + dd*dd*dn*dn)/(ad*ad*ad*ad)));
                    }
                else 
                    hpxc->SetBinContent(iter,0);
                hpxc->Scale(intdexp/intnexp);
                break;
            case 1:
                denq->GetYaxis()->SetRange(1,numq->GetNbinsY());
                numq->GetYaxis()->SetRange(1,numq->GetNbinsY());
                denbuf = new TH1D(*((TH1D *) denq->Project3D("y")));
                numbuf = new TH1D(*((TH1D *) numq->Project3D("y")));
                hpyc = new TH1D(*numbuf);
                hpyc->Sumw2();
                hpyc->Reset("ICE");
                hpyc->Divide(numbuf, denbuf, 1.0, 1.0, "");
                for (int iter=1; iter<hpyc->GetNbinsX(); iter++)
                    if (numbuf->GetBinContent(iter)) 
                    {
                        Double_t dn = numbuf->GetBinError(iter);
                        Double_t an = numbuf->GetBinContent(iter);
                        Double_t dd = denbuf->GetBinError(iter);
                        Double_t ad = denbuf->GetBinContent(iter);
                        hpyc->SetBinError(iter, TMath::Sqrt((dn*dn*ad*ad + dd*dd*an*an + dd*dd*dn*dn)/(ad*ad*ad*ad)));
                    }
                else 
                    hpyc->SetBinContent(iter,0);
                hpyc->Scale(intdexp/intnexp);
                break;
            case 2:
                denq->GetZaxis()->SetRange(1,numq->GetNbinsZ());
                numq->GetZaxis()->SetRange(1,numq->GetNbinsZ());
                denbuf = new TH1D(*((TH1D *) denq->Project3D("z")));
                numbuf = new TH1D(*((TH1D *) numq->Project3D("z")));
                hpzc = new TH1D(*numbuf);
                hpzc->Sumw2();
                hpzc->Reset("ICE");
                hpzc->Divide(numbuf, denbuf, 1.0, 1.0, "");
                for (int iter=1; iter<hpzc->GetNbinsX(); iter++)
                    if (numbuf->GetBinContent(iter)) 
                    {
                        Double_t dn = numbuf->GetBinError(iter);
                        Double_t an = numbuf->GetBinContent(iter);
                        Double_t dd = denbuf->GetBinError(iter);
                        Double_t ad = denbuf->GetBinContent(iter);
                        hpzc->SetBinError(iter, TMath::Sqrt((dn*dn*ad*ad + dd*dd*an*an + dd*dd*dn*dn)/(ad*ad*ad*ad)));
                    }
                else 
                    hpzc->SetBinContent(iter,0);
                hpzc->Scale(intdexp/intnexp);
                break;
        }
    }
    if (nproj == 0)
        return hpxc;
    else if (nproj == 1)
        return hpyc;
    else
        return hpzc;
}

Double_t HBTFit::fungek(Double_t *x, Double_t *par)
{
    Double_t qosq = x[0]*x[0];
    Double_t qssq = x[1]*x[1];
    Double_t qlsq = x[2]*x[2];
    Double_t lam =  TMath::Abs(par[1]);

    Double_t gpart = exp((-par[2]*par[2]*qosq-par[3]*par[3]*qssq-par[4]*par[4]*qlsq)/0.038938);

    return (par[0] * (1 + lam*gpart));
}

Double_t HBTFit::fungek1D(Double_t *x, Double_t *par)
{
    Double_t qinv = x[0]*x[0];
    Double_t lam =  TMath::Abs(par[0]);
    Double_t gpart = exp((-par[1]*par[1]*qinv)/0.038938);

    return (par[2] * (1 + lam*gpart));
}