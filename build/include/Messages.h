// ==========================================================================
// TherminaCorr - an extension of THERMINATOR 2 therm2_hbtfit functionality
// Author: Jedrzej Kolas
// Code at: https://github.com/JohnTheBlindMilkman/TherminaCorr
// ==========================================================================

#ifndef _TH2_MESSAGES_H_
    #define _TH2_MESSAGES_H_

    #include <vector>
    #include <TString.h>

    using namespace std;

    class Messages
    {
        public:
            struct Summ
            {
                int kT;
                bool Linv;
                bool Losl;
            };   
            static void Intro();
            static void Help();
            static void HelpQ2();
            static void Summary(double low, double high, vector<TString> files, vector<Summ> su);
            static void Version();
            
    };

#endif