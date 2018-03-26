//---------------------------------------------------------------------------
#ifndef CDataH
#define CDataH

#include <math.h>
#include <stdio.h>
#include <Math.hpp>
//---------------------------------------------------------------------------
class CDataFilter
{
  private:
    bool znak;
    unsigned mantisa, por;
    unsigned GetHexPor(int d);
    unsigned NormaMan(unsigned d);
    unsigned NormaPor(unsigned d);
    int GetIntPor(unsigned d);
  public:
    CDataFilter();
    CDataFilter(int d);
    CDataFilter(double d);
    CDataFilter operator=(const int &d);
    CDataFilter operator=(const double &d);
    CDataFilter operator+(const CDataFilter &d);
    CDataFilter operator-(const CDataFilter &d);
    CDataFilter operator*(const CDataFilter &d);

    int GetInt();
    double GetDouble();
    void SetMP(int m, int p);
    AnsiString GetZnak();
    AnsiString GetMon();
    AnsiString GetPor();
};
//---------------------------------------------------------------------------
#endif
