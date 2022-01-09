#include <TDatime.h>
#include "Storage.h"

Storage::Storage()
{
}

Storage::Storage(TString sFileNam)
{
    TDatime tDate;
    TString sTime;
    ofStr = new ofstream(sFileNam,ios_base::app);

    tDate.Set();
    sTime = tDate.AsSQLString();

    *ofStr << "[" << sTime << "]" << endl;
}

Storage::~Storage()
{
    delete ofStr;
}

void Storage::appendToTxt(TString sValName, double parVal, double parErr)
{
    (*ofStr) << sValName << "\t" << parVal << "\t" << parErr << endl;
}

void Storage::appendToTxt(TString sValName, double parVal)
{
    (*ofStr) << sValName << "\t" << parVal << endl;
}