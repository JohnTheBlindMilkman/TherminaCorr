#include <fstream>
#include <sstream>
#include <cstring>
#include "Configurator.h"
#include "THGlobal.h"

using namespace std;

Configurator::Configurator()
{
    mParameters.clear();
}

Configurator::~Configurator()
{
    mParameters.clear();
}

TString Configurator::GetParameter(const char* aKeyword) noexcept(false)
{
    vector<Parameter>::iterator Iter;
    for (Iter = mParameters.begin(); Iter != mParameters.end(); Iter++)
        if (Iter->keyword == aKeyword) 
        {
            PRINT_DEBUG_2("<Configurator::GetParameter>\tReturning value " << Iter->value << " for keyword " << Iter->keyword);
            return Iter->value;
        }
    throw *(new TString(aKeyword));
}

void Configurator::AddParameter(Parameter* aPar)
{
    mParameters.push_back(*aPar);
}

int Configurator::PrintParameters()
{
    vector<Parameter>::iterator Iter;
    PRINT_MESSAGE("<Configurator::PrintParameters>");
    for (Iter = mParameters.begin(); Iter != mParameters.end(); Iter++)
        PRINT_MESSAGE("\tKeyword: " << Iter->keyword << "\tValue: " << Iter->value);
    return 0;
}