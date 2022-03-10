#include <TH1D.h>
#include <TH3D.h>
#include <TFile.h>
#include <TF1.h>
#include <TF3.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include "Accessibility.h"
#include "Configurator.h"
#include "HBTFit.h"
#include "Messages.h"
#include "Parser.h"
#include "Storage.h"
#include "THGlobal.h"

using namespace std;

int main(int argc, char **argv)
{
    Configurator* sMainConfig;
    HBTFit *hbtFit;
    Storage *store;
    TH3D *ratq;
    TH1D *ratq1;
    TF3 *funqg,*funqk;
    TF1 *funqg1,*funqk1;
    TDatime tDate;
    TString tInRootName,numname,denname,numname1d,denname1d,partName,sMainINI,sPairType,sEventDir,sTimeStamp,sOvvr;
    bool losl,linv;
    int minkT,maxkT,firstkT,stepkT,dofix[NoParams];
    double limShift,lowCut,highCut,lambda,pars[NoParams],parmin[NoParams],parmax[NoParams],maxrange,maxx=0.0, maxy=0.0, maxz=0.0;
    vector<Messages::Summ> outOfBound;
    vector<TString> missFiles;

    Accessibility *accss = new Accessibility();
    
    tDate.Set();
    sTimeStamp = tDate.AsSQLString();

// ##############################################################
// # Command line analysis					
// ##############################################################
  
    minkT = 0;
    maxkT = 0;
    
    if (argc > 1) 
    {
        tInRootName = argv[1];
        if(tInRootName.Contains("-h") || tInRootName.Contains("--help")) 
        {
            Messages::Help();
            return 0;
        } 
        else if(tInRootName.Contains("-v") || tInRootName.Contains("--version")) 
        {
            Messages::Version();
            return 0;
        }
        if (argc > 2) 
        {
            sPairType = argv[2];
            if(sPairType == "pion-pion" || sPairType == "pionM-pionM")
            {
                if(sPairType == "pion-pion")
                    partName = "pipi";
                else    
                    partName = "piMpiM";

                sMainINI = argv[3];
                if(sMainINI.IsDigit()) 
                {
                    minkT = sMainINI.Atoi();
                    sMainINI = argv[4];
                    if(sMainINI.IsDigit())
                    {
                        maxkT = sMainINI.Atoi();

                        if(minkT > maxkT)
                        {
                            PRINT_DEBUG_1("Command line: wrong kT order");
                            PRINT_MESSAGE("Error: kTmin is greater than kTmax");
                            return _ERROR_FEMTO_WRONG_KT_ORDER_;
                        }
                    }
                    else
                    {
                        PRINT_DEBUG_1("Command line: kTmax = NAN");
                        PRINT_MESSAGE("Error: kTmax is not a number");
                        return _ERROR_FEMTO_WRONG_KT_ORDER_;
                    }
                }
                else
                {
                    PRINT_DEBUG_1("Command line: kTmin = NAN");
                    PRINT_MESSAGE("Error: kTmin is not a number");
                    return _ERROR_FEMTO_WRONG_KT_ORDER_;
                }
                sMainINI = "./femtoFit.ini";
            }
            else
            {
                PRINT_DEBUG_1("Command line: unknown pair type");
                PRINT_MESSAGE("Error: Unknown pair type");
                return _ERROR_FEMTO_UNKNOWN_PAIRTYPE_;
            }
        } 
        else
        {
            PRINT_MESSAGE("Error: unknown argument " + tInRootName);
            return _ERROR_GENERAL_FILE_NOT_FOUND_;
        }
        if (argc > 5)
            sMainINI = argv[5];
    } 
    else 
    {
        Messages::Help();
        return _ERROR_GENERAL_FILE_NOT_FOUND_;
    }

    Messages::Intro();  

    sEventDir = Accessibility::getEventDir(tInRootName);

// ##############################################################
// # Read configuration file (femtoFit.ini)			
// ############################################################## 

    sMainConfig = new Configurator;
    Parser* tParser = new Parser(sMainINI);
    tParser->ReadINI(sMainConfig);
    delete tParser;
  
// ##############################################################
// # ReadParameters						
// ##############################################################  

    try 
    {
        for(int i = 0; i < NoParams; i++)
            accss->GetParameter(sMainConfig,sParNames[i],&pars[i], &dofix[i], &parmin[i], &parmax[i]);

        maxrange = sMainConfig->GetParameter("MaxFitRange").Atof();
        numname  = sMainConfig->GetParameter("Numerator");
        denname  = sMainConfig->GetParameter("Denominator");
        numname1d  = sMainConfig->GetParameter("Numerator1D");
        denname1d  = sMainConfig->GetParameter("Denominator1D");
        firstkT = sMainConfig->GetParameter("FirstkT").Atoi();
        stepkT = sMainConfig->GetParameter("WidthkT").Atoi();
        sOvvr = sMainConfig->GetParameter("Override");
    }
    catch (TString tError) 
    {
        PRINT_DEBUG_1("therm2_hbtfit - Caught exception " << tError);
        PRINT_MESSAGE("Did not find one of the necessary parameters in the parameters file.");
        exit(_ERROR_CONFIG_PARAMETER_NOT_FOUND_);
    }
    try 
    {
        maxx = sMainConfig->GetParameter("MaxFitRangeX").Atof();
        maxy = sMainConfig->GetParameter("MaxFitRangeY").Atof();
        maxz = sMainConfig->GetParameter("MaxFitRangeZ").Atof();
    }
    catch (TString tError) 
    {
    }
    try 
    {
        lowCut = sMainConfig->GetParameter("LambdaCutLow").Atof();
        highCut = sMainConfig->GetParameter("LambdaCutHigh").Atof();
    }
    catch (TString tError) 
    {
    }
    
    accss->CopyINIFile(sMainINI,sEventDir);

// ##############################################################
// # Log file entry 						
// ##############################################################

    {
        TString tLogName;
        ofstream tLogFile;   
        
        try 
        {
            tLogName = sMainConfig->GetParameter("LogFile");
        }
        catch (TString tError) 
        {
        }   
        if (!tLogName.IsNull()) 
        {
            tDate.Set();
            
            tLogFile.open(tLogName, ios_base::app);
            if (static_cast<long>(tLogFile.tellp()) == 0) 
                tLogFile << "# THERMINATOR 2 Log File"<<endl;
            
            tLogFile << '['<<tDate.AsSQLString()<<"]\ttherm2_hbtfit\t"<<sPairType<<'\t';
            tLogFile << "[input]\t"<<tInRootName<<'\t'<<sMainINI<<'\t'<<sPairType<<'\t';
            tLogFile << endl;
            tLogFile.close();
        }
    }

// ##############################################################
// # Fit							
// ############################################################## 

    hbtFit = new HBTFit();
    const int kTlen = maxkT-minkT+1;
    TFile* tInRootFile = new TFile();
    TH3D *numq,*denq;
    TH1D *numq1,*denq1;
    TGraphErrors *gFitRes[NoParams];
    vector<double> parVals[NoParams];
    vector<double> parErrs[NoParams];
    vector<double> kTVal[NoParams];
    int iter2;

    funqg = new TF3("funqg",hbtFit, &HBTFit::fungek, -0.15, 0.15, -0.15, 0.15, -0.15, 0.15, 5);
    if (maxx > 0)
        funqg->SetRange(0, 0, 0, maxx, maxy, maxz);
    else
        funqg->SetRange(-maxrange, -maxrange, -maxrange, maxrange, maxrange, maxrange);
    
    funqk = funqg;
    

    funqg1 = new TF1("funqg1",hbtFit, &HBTFit::fungek1D, -0.15, 0.15, 3);
    if (maxx > 0)
        funqg1->SetRange(0, 0, 0, maxx, maxy, maxz);
    else
        funqg1->SetRange(-maxrange, -maxrange, -maxrange, maxrange, maxrange, maxrange);
    
    funqk1 = funqg1;
    for (int iter = 0; iter < 3; iter++)
    {
        if (dofix[iter]) 
        {
            if (dofix[iter] == 2) 
            {
                funqk1->SetParLimits(iter, parmin[iter], parmax[iter]);
                funqk1->SetParameter(iter, pars[iter]);
            }
            else
                funqk1->FixParameter(iter, pars[iter]);
        }
        else
            funqk1->SetParameter(iter, pars[iter]);

        funqk1->SetParName(iter, sParNames[iter]);
    }
    
    for(int ii = minkT; ii <= maxkT; ii++)
    {
        for (int iter = 2; iter < NoParams; iter++)
        {
            iter2 = iter-2;
            if (dofix[iter]) 
            {
                if (dofix[iter] == 2) 
                {
                    if(iter == 4)
                    {
                        limShift = (ii-minkT)*0.40;
                        funqk->SetParLimits(iter2, parmin[iter]-limShift, parmax[iter]-limShift);
                        funqk->SetParameter(iter2, pars[iter]-limShift);
                    }
                    else if(iter == 5)
                    {
                        limShift = (ii-minkT)*0.20;
                        funqk->SetParLimits(iter2, parmin[iter]-limShift, parmax[iter]-limShift);
                        funqk->SetParameter(iter2, pars[iter]-limShift);
                    }
                    else if(iter == 6)
                    {
                        limShift = (ii-minkT)*0.25;
                        funqk->SetParLimits(iter2, parmin[iter]-limShift, parmax[iter]-limShift);
                        funqk->SetParameter(iter2, pars[iter]-limShift);
                    }
                    else
                    {
                        funqk->SetParLimits(iter2, parmin[iter], parmax[iter]);
                        funqk->SetParameter(iter2, pars[iter]);
                    }
                }
                else
                    funqk->FixParameter(iter2, pars[iter]);
            }
            else
                funqk->SetParameter(iter2, pars[iter]);

            funqk->SetParName(iter2, sParNames[iter]);
        }

        lambda = 0;
        PRINT_MESSAGE("["<<sTimeStamp<<"]\tFitting.");

        tInRootName = TString::Format("%sfemto%s%ia.root",sEventDir.Data(),partName.Data(),ii);
        tInRootFile = TFile::Open(tInRootName.Data());
        if(tInRootFile == nullptr)
        {
            missFiles.push_back(tInRootName);
            continue;
        }
            
        numq1 = new TH1D(*((TH1D *) tInRootFile->Get(numname1d)));
        denq1 = new TH1D(*((TH1D *) tInRootFile->Get(denname1d)));
        ratq1 = new TH1D(*numq1);
        ratq1->Reset("ICE");
        ratq1->Divide(numq1, denq1, 1.0, 1.0);
        ratq1->SetName("ratq1");
        ratq1->SetTitle("ratq1");
        ratq1->Fit(funqk1, "RBMPE");

        numq = new TH3D(*((TH3D *) tInRootFile->Get(numname)));
        denq = new TH3D(*((TH3D *) tInRootFile->Get(denname)));
        ratq = new TH3D(*numq);
        ratq->Reset("ICE");
        ratq->Divide(numq, denq, 1.0, 1.0);
        ratq->SetName("ratq");
        ratq->SetTitle("ratq");
        ratq->Fit(funqk, "RBMPE");

// ##############################################################
// # Save fit values						
// ############################################################## 

        TString   tOutTextName;
        double parVal,parErr,kTmiddle;
        linv = false;
        losl = false;
        
        tOutTextName = sEventDir + "hbtradii.txt";
        if(ii == minkT)
            store = new Storage(tOutTextName, sOvvr);
        else
            store = new Storage(tOutTextName);

        PRINT_DEBUG_1("Fit results:");
        for (int iter = 0; iter < NoParams; iter++)
        {
            if(iter < 3)
            {
                parVal = fabs(funqk1->GetParameter(iter));
                parErr = fabs(funqk1->GetParError(iter));
                if(iter == 0)
                    lambda = parVal;
            }
            else
            {
                parVal = fabs(funqk->GetParameter(iter-2));
                parErr = fabs(funqk->GetParError(iter-2));
                if(iter == 3)
                    lambda = parVal;
            }

            store->appendToTxt(sParNames[iter],parVal,parErr);
            if(lambda > lowCut && lambda < highCut)
            {
                kTVal[iter].push_back(firstkT+(ii-minkT)*stepkT);
                parVals[iter].push_back(parVal);
                parErrs[iter].push_back(parErr);
            }
            else
            {
                if(iter == 0)
                    linv = true;
                else if(iter == 3)
                    losl = true;
            }
            PRINT_DEBUG_1("\t" + sParNames[iter] << parVal << " +/- " << parErr);
        }
        delete store;    

        if(linv || losl)
            outOfBound.push_back({ii,linv,losl});  
    
// ##############################################################
// # Make plots							
// ############################################################## 

        TH3D *fitnq; 

        TH1D *hpx1 = hbtFit->getproj(numq, denq, 0, 0, pars[0]);
        TH1D *hpy1 = hbtFit->getproj(numq, denq, 1, 0, pars[0]);
        TH1D *hpz1 = hbtFit->getproj(numq, denq, 2, 0, pars[0]);
        
        TH1D *hpx2 = hbtFit->getproj(numq, denq, 0, 4, pars[0]);
        TH1D *hpy2 = hbtFit->getproj(numq, denq, 1, 4, pars[0]);
        TH1D *hpz2 = hbtFit->getproj(numq, denq, 2, 4, pars[0]);
        
        TH1D *hpx3 = hbtFit->getproj(numq, denq, 0, 10, pars[0]);
        TH1D *hpy3 = hbtFit->getproj(numq, denq, 1, 10, pars[0]);
        TH1D *hpz3 = hbtFit->getproj(numq, denq, 2, 10, pars[0]);
        
        hbtFit->getfitprojc(denq, &fitnq, funqk);

        TH1D *fpx1 = hbtFit->getproj(fitnq, denq, 0, 0, pars[0]);
        TH1D *fpy1 = hbtFit->getproj(fitnq, denq, 1, 0, pars[0]);
        TH1D *fpz1 = hbtFit->getproj(fitnq, denq, 2, 0, pars[0]);
        
        TH1D *fpx2 = hbtFit->getproj(fitnq, denq, 0, 4, pars[0]);
        TH1D *fpy2 = hbtFit->getproj(fitnq, denq, 1, 4, pars[0]);
        TH1D *fpz2 = hbtFit->getproj(fitnq, denq, 2, 4, pars[0]);
        
        TH1D *fpx3 = hbtFit->getproj(fitnq, denq, 0, 10, pars[0]);
        TH1D *fpy3 = hbtFit->getproj(fitnq, denq, 1, 10, pars[0]);
        TH1D *fpz3 = hbtFit->getproj(fitnq, denq, 2, 10, pars[0]);
        
        hpx1->SetName("CFproj_out_1");
        hpy1->SetName("CFproj_side_1");
        hpz1->SetName("CFproj_long_1");

        hpx2->SetName("CFproj_out_2");
        hpy2->SetName("CFproj_side_2");
        hpz2->SetName("CFproj_long_2");

        hpx3->SetName("CFproj_out_3");
        hpy3->SetName("CFproj_side_3");
        hpz3->SetName("CFproj_long_3");

        fpx1->SetName("FITproj_out_1");
        fpy1->SetName("FITproj_side_1");
        fpz1->SetName("FITproj_long_1");

        fpx2->SetName("FITproj_out_2");
        fpy2->SetName("FITproj_side_2");
        fpz2->SetName("FITproj_long_2");

        fpx3->SetName("FITproj_out_3");
        fpy3->SetName("FITproj_side_3");
        fpz3->SetName("FITproj_long_3");

        hbtFit->preparehist(hpx1,1);
        hbtFit->preparehist(hpy1,1);
        hbtFit->preparehist(hpz1,1);

        hbtFit->preparehist(hpx2,1);
        hbtFit->preparehist(hpy2,1);
        hbtFit->preparehist(hpz2,1);

        hbtFit->preparehist(hpx3,1);
        hbtFit->preparehist(hpy3,1);
        hbtFit->preparehist(hpz3,1);

        hbtFit->preparehist(fpx1,3);
        hbtFit->preparehist(fpy1,3);
        hbtFit->preparehist(fpz1,3);

        hbtFit->preparehist(fpx2,3);
        hbtFit->preparehist(fpy2,3);
        hbtFit->preparehist(fpz2,3);

        hbtFit->preparehist(fpx3,3);
        hbtFit->preparehist(fpy3,3);
        hbtFit->preparehist(fpz3,3);

        TCanvas *cancf = new TCanvas ("cancf", "cancf", 1200, 1200);
        gPad->SetFillColor(0);
        gPad->SetFillStyle(4000);
        cancf->Divide(3,3,0.0001,0.0001);
        
        hpx1->GetXaxis()->SetTitle("q_{out} [GeV/c]");
        hpy1->GetXaxis()->SetTitle("q_{side} [GeV/c]");
        hpz1->GetXaxis()->SetTitle("q_{long} [GeV/c]");

        hpx1->GetYaxis()->SetTitle("C(q_{out})");
        hpy1->GetYaxis()->SetTitle("C(q_{side})");
        hpz1->GetYaxis()->SetTitle("C(q_{long})");

        hpx2->GetXaxis()->SetTitle("q_{out} [GeV/c]");
        hpy2->GetXaxis()->SetTitle("q_{side} [GeV/c]");
        hpz2->GetXaxis()->SetTitle("q_{long} [GeV/c]");

        hpx2->GetYaxis()->SetTitle("C(q_{out})");
        hpy2->GetYaxis()->SetTitle("C(q_{side})");
        hpz2->GetYaxis()->SetTitle("C(q_{long})");

        hpx3->GetXaxis()->SetTitle("q_{out} [GeV/c]");
        hpy3->GetXaxis()->SetTitle("q_{side} [GeV/c]");
        hpz3->GetXaxis()->SetTitle("q_{long} [GeV/c]");

        hpx3->GetYaxis()->SetTitle("C(q_{out})");
        hpy3->GetYaxis()->SetTitle("C(q_{side})");
        hpz3->GetYaxis()->SetTitle("C(q_{long})");

        cancf->cd(1);
        hbtFit->preparepad();

        hpx1->Draw("HISTPE1");
        fpx1->Draw("SAMEHISTL");

        cancf->cd(2);
        hbtFit->preparepad();

        hpy1->Draw("HISTPE1");
        fpy1->Draw("SAMEHISTL");

        cancf->cd(3);
        hbtFit->preparepad();

        hpz1->Draw("HISTPE1");
        fpz1->Draw("SAMEHISTL");

        cancf->cd(4);
        hbtFit->preparepad();

        hpx2->Draw("HISTPE1");
        fpx2->Draw("SAMEHISTL");

        cancf->cd(5);
        hbtFit->preparepad();

        hpy2->Draw("HISTPE1");
        fpy2->Draw("SAMEHISTL");

        cancf->cd(6);
        hbtFit->preparepad();

        hpz2->Draw("HISTPE1");
        fpz2->Draw("SAMEHISTL");

        cancf->cd(7);
        hbtFit->preparepad();

        hpx3->Draw("HISTPE1");
        fpx3->Draw("SAMEHISTL");

        cancf->cd(8);
        hbtFit->preparepad();

        hpy3->Draw("HISTPE1");
        fpy3->Draw("SAMEHISTL");

        cancf->cd(9);
        hbtFit->preparepad();

        hpz3->Draw("HISTPE1");
        fpz3->Draw("SAMEHISTL");

// ##############################################################
// # Save plots							
// ##############################################################   
    
        TString   tOutRootName;
        TFile*    tOutRootFile = new TFile();
        tOutRootName = tInRootName;
        tOutRootName.ReplaceAll("femto","hbtfit");
        tOutRootFile = TFile::Open(tOutRootName,"RECREATE");
        tOutRootFile->cd();
        
        hpx1->Write();
        hpy1->Write();
        hpz1->Write();

        hpx2->Write();
        hpy2->Write();
        hpz2->Write();

        hpx3->Write();
        hpy3->Write();
        hpz3->Write();

        fpx1->Write();
        fpy1->Write();
        fpz1->Write();

        fpx2->Write();
        fpy2->Write();
        fpz2->Write();

        fpx3->Write();
        fpy3->Write();
        fpz3->Write();

        tOutRootFile->Close();
        tInRootFile->Close();
        tDate.Set(); 
        PRINT_MESSAGE("["<<tDate.AsSQLString()<<"]\tFile "<<tOutTextName<<" and");
        PRINT_MESSAGE("\tfile "<<tOutRootName<<" written.");

        delete cancf;

// ##############################################################
// # Log file entry 						
// ##############################################################

        {
            TString  tLogName;
            TString  tORootName;
            TString  tOTextName;
            ofstream tLogFile;   
            
            try 
            {
                tLogName = sMainConfig->GetParameter("LogFile");
            }
            catch (TString tError) 
            {
            }   
            if (!tLogName.IsNull()) 
            {
                tDate.Set();
                tORootName = tOutRootName;
                tOTextName = tOutTextName;
                tORootName.ReplaceAll(sEventDir,"");      
                tOTextName.ReplaceAll(sEventDir,"");
                
                tLogFile.open(tLogName, ios_base::app);
                if (static_cast<long>(tLogFile.tellp()) == 0) 
                    tLogFile << "# THERMINATOR 2 Log File"<<endl;

                tLogFile << '['<<tDate.AsSQLString()<<"]\ttherm2_hbtfit\t"<<sPairType<<'\t';
                tLogFile << "[output]\t"<<sEventDir<<'\t'<<tORootName<<'\t'<<tOTextName<<'\t';
                tLogFile << endl;
                tLogFile.close();
            }    
        }
    
// ##############################################################
// # Temporary file						
// ##############################################################  

        ofstream tTmpFile;
        TString tTmpFileName;
        
        tTmpFileName = TString::Format("./hbtfit_%s.tmp",sPairType.Data()); 
        tTmpFile.open(tTmpFileName, ios_base::app);
        if((tTmpFile) && tTmpFile.is_open()) 
            tTmpFile << tOutRootName << endl;
        else
        {
            PRINT_MESSAGE("Unable to create temp file "<<tTmpFileName);
            exit(_ERROR_GENERAL_FILE_NOT_FOUND_);
        }
    }

    TFile *paramFile = new TFile();
    paramFile = TFile::Open(Form("%sparameterFit%s.root",sEventDir.Data(),partName.Data()),"RECREATE");
    for (int iter = 0; iter < NoParams; iter++) 
    {
        gFitRes[iter] = new TGraphErrors(kTVal[iter].size(),&kTVal[iter][0],&parVals[iter][0],0,&parErrs[iter][0]);
        gFitRes[iter]->Write(Form("g%s_%i_%i",sParNames[iter].Data(),minkT,maxkT));
    }
    
    paramFile->Close();

    Messages::Summary(lowCut,highCut,missFiles,outOfBound);

    return 0;
}
