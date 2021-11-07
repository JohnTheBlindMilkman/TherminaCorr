#ifndef _TH2_GLOBAL_H_
  #define _TH2_GLOBAL_H_

#include <iostream>
#include <stdlib.h>
#include <cstdio>

// Global static constants

static const double kTwoPi3 = 248.0502134423985614038105205368;
static const double kTwoPi2 =  39.4784176043574344753379639995;
static const double kHbarC  =   0.1973269631;

static const int kFileNameMaxChar = 2000;

// Define version of THERMINATOR 2

#define _THERMINATOR2_VERSION_  "2.0.3"

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
#define _ERROR_CONFIG_PARAMETER_NOT_FOUND_	3
#define _ERROR_LIBRARY_EMPTY_			4
#define _ERROR_LIBRARY_TAG_NOT_FOUND_		5
#define _ERROR_LIBRARY_TAG_ATTRIB_NOT_FOUND_	6
#define _ERROR_LIBRARY_VECTOR3D_NOT_FOUND_	7
#define _ERROR_FEMTO_UNKNOWN_PAIRTYPE_		16

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