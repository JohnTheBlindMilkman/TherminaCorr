#include <iostream>
#include <TString.h>
#include "Parser.h"
#include "Messages.h"
#include "Configurator.h"
#include "Accessibility.h"
#include "THGlobal.h"
#include "Compliance.h"

using namespace std;

int main(int argc, char **argv)
{
// ##############################################################
// # Constants definition					
// ##############################################################

    const TString sParList[] = {"LambdaInv","RadiusInv","LambdaOSL","RadiusOut","RadiusSide","RadiusLong"};
    const int len = (int) sizeof(sParList)/sizeof(sParList[0]);

// ##############################################################
// # Variable initialisation					
// ##############################################################

    int noGraphs,minkT,maxkT;
    TString sFilePath,sMainINI,sEventDir,sExpFilePath,sParam,sModelName,sExpGraphName[len];  
    Configurator *sMainConfig;
    Messages *msg;
    Accessibility *accss;
    Compliance *comp;
    Parser *tParser;

// ##############################################################
// # Command line analysis					
// ##############################################################

    msg = new Messages();
    sMainINI = "./Q2test.ini";

    if(argc > 1)
    {
        sFilePath = argv[1];
        if(sFilePath.Contains("-h") || sFilePath.Contains("--help")) 
        {
            msg->MessageHelpQ2();
            return 0;
        }
        else if(sFilePath.Contains("-v") || sFilePath.Contains("--version")) 
        {
            msg->MessageVersion();
            return 0;
        }
        if(argc > 2)
            sMainINI = argv[2];

        if(argc > 3)
            PRINT_MESSAGE("Warning: Omitting unnecessary arguments");
    }
    else 
    {
        msg->MessageHelp();
        return _ERROR_GENERAL_FILE_NOT_FOUND_;
    }

    msg->MessageIntro();  

    sEventDir = sFilePath;
    {
        int tPos=0;
        for(int i=1; i<sEventDir.Length(); i++) 
            if(sEventDir[i]=='/')
                tPos = i;
        
        sEventDir.Resize(tPos+1);
    }

// ##############################################################
// # Read configuration file (femtoFit.ini)			
// ############################################################## 

    sMainConfig = new Configurator;
    tParser = new Parser(sMainINI);
    tParser->ReadINI(sMainConfig);
    delete tParser;
  
// ##############################################################
// # ReadParameters						
// ##############################################################  

    accss = new Accessibility();
    noGraphs = 0;

    try 
    {
        sExpFilePath = sMainConfig->GetParameter("Loc");
        for(int i = 0; i < len; i++)
        {
            sParam = sMainConfig->GetParameter(sParList[i].Data());
            sExpGraphName[i] = sParam;
            if(sParam != "none")
                noGraphs++;
        }
        
        minkT = sMainConfig->GetParameter("MinkT").Atoi();
        maxkT = sMainConfig->GetParameter("MaxkT").Atoi(); 
    }
    catch (TString tError) 
    {
        PRINT_DEBUG_1("Q2test - Caught exception " << tError);
        PRINT_MESSAGE("Did not find one of the necessary parameters in the parameters file.");
        exit(_ERROR_CONFIG_PARAMETER_NOT_FOUND_);
    }
    
    accss->CopyINIFile(sMainINI,sEventDir);

    if(noGraphs == 0)
    {
        PRINT_MESSAGE("All graphs in " + sMainINI + " set to \"none\". Finishing execution.");
        exit(0);
    }

// ##############################################################
// # Fit
// ############################################################## 
    
    for(int i = 0; i < len; i++)
    {
        if(Compliance::getModelName(sExpGraphName[i],i,minkT,maxkT,sModelName))
        {
            comp = new Compliance(sFilePath,sModelName,sExpFilePath,sExpGraphName[i]);
            comp->printResult(true);
            comp->~Compliance();
        }
    }
    


    return 0;
}