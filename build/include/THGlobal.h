#ifndef _TH2_GLOBAL_H_
    #define _TH2_GLOBAL_H_

    #include <iostream>
    #include <stdlib.h>
    #include <cstdio>
    #include <TString.h>

    // Global static constants

    static const double kTwoPi3 = 248.0502134423985614038105205368;
    static const double kTwoPi2 =  39.4784176043574344753379639995;
    static const double kHbarC  =   0.1973269631;

    static const int kFileNameMaxChar = 2000;
    static const int NoParams = 7;
    static const int projWidth[] = {0,4,10};

    static const TString sParNames[NoParams] = {"LambdaInv","Rinv","Norm","LambdaOSL","Rout","Rside","Rlong"};
    static const TString sProjNames[] = {"out","side","long"};

    // Define version of THERMINATOR 2

    #define _THERMINACORR_VERSION_  "1.6.0"

    // Define compilation specific variables

    #ifndef _CXX_VER_
    #define _CXX_VER_ "g++("<<__GNUC__<<'.'<<__GNUC_MINOR__<<'.'<<__GNUC_PATCHLEVEL__<<")"
    #endif
    #ifndef _ROOT_VER_
    #define _ROOT_VER_ " .  /  "
    #endif

    // Define global errors

    #define _ERROR_GENERAL_FILE_NOT_FOUND_		1
    #define _ERROR_GENERAL_MODEL_UNKNOWN_		2
    #define _ERROR_GENERAL_UNSUPORTED_VALUE_ 3
    #define _ERROR_CONFIG_PARAMETER_NOT_FOUND_	4
    #define _ERROR_LIBRARY_EMPTY_			5
    #define _ERROR_LIBRARY_TAG_NOT_FOUND_		6
    #define _ERROR_LIBRARY_TAG_ATTRIB_NOT_FOUND_	7
    #define _ERROR_LIBRARY_VECTOR3D_NOT_FOUND_	8
    #define _ERROR_FEMTO_UNKNOWN_PAIRTYPE_		16
    #define _ERROR_FEMTO_WRONG_KT_ORDER_		17
    #define _ERROR_FEMTO_GRAPH_NOT_FOUND_		18

    // Define DEBUG information

    #define PRINT_MESSAGE(_mes) std::cout << _mes << std::endl;
    #if _DEBUG_LEVEL_==0
        #define PRINT_DEBUG_3(_mes)
        #define PRINT_DEBUG_2(_mes)
        #define PRINT_DEBUG_1(_mes)
    #elif _DEBUG_LEVEL_==1
        #define PRINT_DEBUG_3(_mes)
        #define PRINT_DEBUG_2(_mes)
        #define PRINT_DEBUG_1(_mes) std::cerr << _mes << std::endl;
    #elif _DEBUG_LEVEL_==2
        #define PRINT_DEBUG_3(_mes)
        #define PRINT_DEBUG_2(_mes) std::cerr << _mes << std::endl;
        #define PRINT_DEBUG_1(_mes) std::cerr << _mes << std::endl;
    #elif _DEBUG_LEVEL_==3
        #define PRINT_DEBUG_3(_mes) std::cerr << "DL3:" << _mes << std::endl;
        #define PRINT_DEBUG_2(_mes) std::cerr << "DL2:" << _mes << std::endl;
        #define PRINT_DEBUG_1(_mes) std::cerr << "DL1:" << _mes << std::endl;
    #endif

#endif /*_TH2_GLOBAL_H_*/