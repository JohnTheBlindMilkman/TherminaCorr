#ifndef _TH2_COMPLIANCE_H_
    #define _TH2_COMPLIANCE_H_

    #include <TGraph.h>
    #include <TFile.h>
    #include "Storage.h"

    class Compliance
    {
        public:
            Compliance();
            Compliance(TString sModFilePath, TString sModName, TString sExpFilePath, TString sExpName);
            ~Compliance();

            double printResult(bool moreInfo = false);
            static bool getModelName(TString expName, int iter, int minkT, int maxkT, TString &modName);

        private:
            double Q2Test(int &ndf,double &totDiff);
            double getClosest(double xVal, int &i, double &x, double &y);

            TFile *mMFile,*mEFile;
            TGraph *mExp,*mMod;
            TString mModName,mEventDir;
    };

#endif