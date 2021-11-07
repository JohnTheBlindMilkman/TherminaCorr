#ifndef _TH2_CONFIGURATOR_H_
  #define _TH2_CONFIGURATOR_H_

#include <vector>
#include <TString.h>

struct Parameter
{
  TString keyword;
  TString value;
};

class Configurator 
{

    public:
        Configurator();
        ~Configurator();

        TString GetParameter(const char* aKeyword) noexcept(false);
        void AddParameter(Parameter* aPar);
        int PrintParameters();

    private:    
        std::vector<Parameter> mParameters;	
};

#endif