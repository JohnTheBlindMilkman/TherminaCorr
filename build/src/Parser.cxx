/********************************************************************************
 *                                                                              *
 *             THERMINATOR 2: THERMal heavy-IoN generATOR 2                     *
 *                                                                              *
 * Version:                                                                     *
 *      Release, 2.0.3, 1 February 2011                                         *
 *                                                                              *
 * Authors:                                                                     *
 *      Mikolaj Chojnacki   (Mikolaj.Chojnacki@ifj.edu.pl)                      *
 *      Adam Kisiel         (kisiel@if.pw.edu.pl)                               *
 *      Wojciech Broniowski (Wojciech.Broniowski@ifj.edu.pl)                    *
 *      Wojciech Florkowski (Wojciech.Florkowski@ifj.edu.pl)                    *
 *                                                                              *
 * Project homepage:                                                            *
 *      http://therminator2.ifj.edu.pl/                                         *
 *                                                                              *
 * For the detailed description of the program and further references           *
 * to the description of the model please refer to                              *
 * http://arxiv.org/abs/1102.0273                                               *
 *                                                                              *
 * This code can be freely used and redistributed. However if you decide to     *
 * make modifications to the code, please, inform the authors.                  *
 * Any publication of results obtained using this code must include the         *
 * reference to arXiv:1102.0273 and the published version of it, when           *
 * available.                                                                   *
 *                                                                              *
 ********************************************************************************/

// Modified by: Jedrzej Kolas

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