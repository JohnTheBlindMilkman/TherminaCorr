#include <iostream>
#include "Messages.h"
#include "THGlobal.h"

using namespace std;

Messages::Messages()
{
}

Messages::~Messages()
{
}

void Messages::MessageIntro()
{
    PRINT_MESSAGE("  _______ _    _ ______ _____  __  __ _____ _   _          _____ ____  _____  _____  ");
    PRINT_MESSAGE(" |__   __| |  | |  ____|  __ \\|  \\/  |_   _| \\ | |   /\\   / ____/ __ \\|  __ \\|  __ \\ ");
    PRINT_MESSAGE("    | |  | |__| | |__  | |__) | \\  / | | | |  \\| |  /  \\ | |   | |  | | |__) | |__) |");
    PRINT_MESSAGE("    | |  |  __  |  __| |  _  /| |\\/| | | | | . ` | / /\\ \\| |   | |  | |  _  /|  _  / ");
    PRINT_MESSAGE("    | |  | |  | | |____| | \\ \\| |  | |_| |_| |\\  |/ ____ \\ |___| |__| | | \\ \\| | \\ \\ ");
    PRINT_MESSAGE("    |_|  |_|  |_|______|_|  \\_\\_|  |_|_____|_| \\_/_/    \\_\\_____\\____/|_|  \\_\\_|  \\_\\");
    PRINT_MESSAGE("");
    PRINT_MESSAGE("  ***********************************************************************"	);
    PRINT_MESSAGE("  *\t\t1D and 3D HBT fit for THERMINATOR 2, version "<<_THERMINACORR_VERSION_<<"\t\t\t*"	);
    PRINT_MESSAGE("  *\t\t\t\t\t\t\t\t\t*"							);
    PRINT_MESSAGE("  * source code based on: THERMINATOR 2 \t\t*"		);
    PRINT_MESSAGE("  * author: Jedrzej Kolas\t\t*"		);
    PRINT_MESSAGE("  * Please don't sue me, I have a family.\t\t*"				);
    PRINT_MESSAGE("  ***********************************************************************"	);
}

void Messages::MessageHelp()
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

void Messages::MessageHelpQ2()
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

void Messages::MessageVersion()
{
    PRINT_MESSAGE("version:\tTherminacorr version "<<_THERMINACORR_VERSION_);
    PRINT_MESSAGE("compiled with:\t"<<_CXX_VER_<<", ROOT("<<_ROOT_VER_<<")");
    std::cout <<  "  preprocessor: ";
#ifdef _DEBUG_LEVEL_
    std::cout << "DEBUG="<<_DEBUG_LEVEL_<<" ";
#endif
    std::cout << std::endl;
}