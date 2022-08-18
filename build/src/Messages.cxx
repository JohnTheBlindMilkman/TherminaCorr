// ==========================================================================
// TherminaCorr - an extension of THERMINATOR 2 therm2_hbtfit functionality
// Author: Jedrzej Kolas
// Code at: https://github.com/JohnTheBlindMilkman/TherminaCorr
// ==========================================================================

#include <iostream>
#include "Messages.h"
#include "THGlobal.h"

using namespace std;

void Messages::Intro()
{
    PRINT_MESSAGE("  _______ _    _ ______ _____  __  __ _____ _   _          _____ ____  _____  _____  ");
    PRINT_MESSAGE(" |__   __| |  | |  ____|  __ \\|  \\/  |_   _| \\ | |   /\\   / ____/ __ \\|  __ \\|  __ \\ ");
    PRINT_MESSAGE("    | |  | |__| | |__  | |__) | \\  / | | | |  \\| |  /  \\ | |   | |  | | |__) | |__) |");
    PRINT_MESSAGE("    | |  |  __  |  __| |  _  /| |\\/| | | | | . ` | / /\\ \\| |   | |  | |  _  /|  _  / ");
    PRINT_MESSAGE("    | |  | |  | | |____| | \\ \\| |  | |_| |_| |\\  |/ ____ \\ |___| |__| | | \\ \\| | \\ \\ ");
    PRINT_MESSAGE("    |_|  |_|  |_|______|_|  \\_\\_|  |_|_____|_| \\_/_/    \\_\\_____\\____/|_|  \\_\\_|  \\_\\");
    PRINT_MESSAGE("");
    PRINT_MESSAGE("  ***********************************************************************");
    PRINT_MESSAGE("  *         1D and 3D HBT fit for THERMINATOR 2, version "<<_THERMINACORR_VERSION_<<"          *");
    PRINT_MESSAGE("  *                                                                     *");
    PRINT_MESSAGE("  * source code based on: THERMINATOR 2                                 *");
    PRINT_MESSAGE("  * author: Jedrzej Kolas                                               *");
    PRINT_MESSAGE("  * Please don't sue me, I have a family.                               *");
    PRINT_MESSAGE("  ***********************************************************************");
}

void Messages::Help()
{
    PRINT_MESSAGE("Usage: ");
    PRINT_MESSAGE("thermiCorr <FOLDER_PATH> <PAIR_TYPE> <KT_MIN> <KT_MAX> [HBTFIT_INI]");
    PRINT_MESSAGE("thermiCorr [OPTION]");
    PRINT_MESSAGE("  <FOLDER_PATH>\t\tpath to folder with femto ROOT files created with therm2_femto");
    PRINT_MESSAGE("  <PAIR_TYPE>\t\tparticle type created using therm2_femto;\t\toptions: pion-pion or pionM-pionM");
    PRINT_MESSAGE("  <KT_MIN>\t\tstarting kT bin number (from therm2_femto)");
    PRINT_MESSAGE("  <KT_MAX>\t\tfinishing kT bin number (from therm2_femto)");
    PRINT_MESSAGE("  [HBTFIT_INI]\t\tmain settings file;\t\tdefault: femtoFit.ini");
    PRINT_MESSAGE("  [OPTION]");
    PRINT_MESSAGE("    -h | --help\t\tthis screen");
    PRINT_MESSAGE("    -v | --version\tversion information");
}

void Messages::HelpQ2()
{
    PRINT_MESSAGE("Usage: ");
    PRINT_MESSAGE("Q2test <FILE_PATH> [Q2TEST_INI]");
    PRINT_MESSAGE("Q2test [OPTION]");
    PRINT_MESSAGE("  <FILE_PATH>\t\tpath to therminacorr output file containing fit parameters");
    PRINT_MESSAGE("  [Q2TEST_INI]\t\tmain settings file;\t\tdefault: Q2test.ini");
    PRINT_MESSAGE("  [OPTION]");
    PRINT_MESSAGE("    -h | --help\t\tthis screen");
    PRINT_MESSAGE("    -v | --version\tversion information");
}

void Messages::Summary(double low, double high, vector<TString> files, vector<Summ> su)
{
    PRINT_MESSAGE("===============================================================================");

    if(files.empty() && su.empty())
    {
        PRINT_MESSAGE("Everything was successful");
    }
    else
    {
        if(!files.empty())
        {
            PRINT_MESSAGE("Files that were not found: ");
            for(auto it = begin(files); it != end(files); it++)
            {
                PRINT_MESSAGE(*it);
            }
            if(!su.empty())
            {
                PRINT_MESSAGE("");
                PRINT_MESSAGE("");
            }
        }
        if(!su.empty())
        {
            char inv, osl;
            
            PRINT_MESSAGE("Fits outside bounds, where lambda is not in (" << low << ", " << high << "):");
            PRINT_MESSAGE("                   " << "kT bin" << "            " << "Lambda Inv" << "        " << "Lambda OSL");
            for(auto it = begin(su); it != end(su); it++)
            {
                inv = ' ';
                osl = ' ';
                if(it->Linv)
                    inv = '+';
                if(it->Losl)
                    osl = '+';
                PRINT_MESSAGE("                     " << it->kT << "             " << inv << "                   " << osl);
            }
        }
    }

    PRINT_MESSAGE("===============================================================================");
}

void Messages::Version()
{
    PRINT_MESSAGE("version:\t"<<_THERMINACORR_VERSION_);
    PRINT_MESSAGE("compiled with:");
    PRINT_MESSAGE("\t\t"<<_CXX_VER_);
    PRINT_MESSAGE("\t\tROOT("<<_ROOT_VER_<<")");
#ifdef _DEBUG_LEVEL_
    PRINT_MESSAGE("DEBUG=\t"<<_DEBUG_LEVEL_);
#endif
}