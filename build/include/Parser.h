#ifndef _TH2_PARSER_H_
  #define _TH2_PARSER_H_

#include <fstream>
#include <TString.h>
#include "Configurator.h"

class Parser 
{
    public:
        Parser();
        Parser(const char* aFileName);
        ~Parser();

        void ReadINI(Configurator* aINI);

        TString	mFileName;
        std::ifstream mFile;
};

#endif
