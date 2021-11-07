#include <fstream>
#include <sstream>
#include <math.h>
#include <TMath.h>
#include "THGlobal.h"
#include "Parser.h"

using namespace std;

Parser::Parser()
: mFileName(""), mFile(0)
{
}

Parser::Parser(const char* aFileName)
: mFileName(aFileName)
{
    mFile.open(mFileName);
    if((!mFile) && (!mFile.is_open())) 
    {
        PRINT_MESSAGE("<Parser::Parser>\tFile "<<mFileName<<" not found.");
        exit(_ERROR_GENERAL_FILE_NOT_FOUND_);
    }
    PRINT_DEBUG_2("<Parser::Parser>\tReading form file "<<mFileName);
}

Parser::~Parser()
{
    mFile.close();
}

void Parser::ReadINI(Configurator* aINI)
{
    Parameter tOpt;
    TString	tStr;
    int	tLine;
    int	i;
    char tBuff[200];

    PRINT_DEBUG_3("<Parser::ReadINI>");
    mFile.seekg(0, std::ios::beg);
    tLine = 0;
    while ((!mFile.eof())) 
    {
        tLine++;
        mFile.getline(tBuff,200);
        tStr = tBuff;
        if((tStr.IsNull()) || (tStr.IsWhitespace()) || (tStr[0] == '#') || (tStr[0] == ';')) 
        {
            PRINT_DEBUG_3("\tLine "<<tLine<<" Ignoring  : "<<tStr.Data());
            continue;
        } 
        else if(tStr.Contains('[')) 
        {
            PRINT_DEBUG_2("\tLine "<<tLine<<" Section   : "<<tStr.Data());
            continue;
        } 
        else if(!tStr.Contains('=')) 
        {
            PRINT_DEBUG_1("\tLine "<<tLine<<" WARNING   : "<<tStr.Data());
            PRINT_DEBUG_1("\tWrong format. Treating as commentary.")
            continue;
        }
        tStr.ReplaceAll(" ","");
        tStr.ReplaceAll("\t","");
        tOpt.keyword = "";
        tOpt.value = "";
        for(i=0; tStr[i] != '='; i++)
            tOpt.keyword += tStr[i];
        for(i++; i<tStr.Length(); i++)
            tOpt.value += tStr[i];
        aINI->AddParameter(&tOpt);
        PRINT_DEBUG_2("\tLine "<<tLine<<" Parameter : "<<tOpt.keyword<<" = "<<tOpt.value);
    }
}