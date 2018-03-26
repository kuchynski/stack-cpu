//---------------------------------------------------------------------------
#pragma hdrstop
#pragma package(smart_init)

#include "CDataFilter.h"

long kol_m, kol_p;
unsigned maskam, maskap;
unsigned maskam_z, maskap_z;
//---------------------------------------------------------------------------
CDataFilter::CDataFilter(): mantisa(0), por(0), znak(true)
{
}
//---------------------------------------------------------------------------
void CDataFilter::SetMP(int m, int p)
{
  kol_m = m;
  kol_p = p;
  maskam = 0x7FFFFFFF >> (32 - kol_m);
  maskap = 0x7FFFFFFF >> (32 - kol_p);
  maskam_z = 0x01 << (kol_m - 1);
  maskap_z = 0x01 << (kol_p - 1);
}
//---------------------------------------------------------------------------
CDataFilter::CDataFilter(int d)
{
  znak = (d >= 0);         //true - >= 0 !!!
  mantisa = NormaMan(abs(d));
  por = (mantisa)? (kol_m-1) : 0;
}
//---------------------------------------------------------------------------
CDataFilter::CDataFilter(double d)
{
  znak = (d >= 0);
  int p;
  double d1 = frexp(fabs(d), &p);
  mantisa = d1 * (maskam+1);
  por = GetHexPor(p);
}
//---------------------------------------------------------------------------
CDataFilter CDataFilter::operator=(const int &d)
{
  znak = (d >= 0);
  mantisa = NormaMan(abs(d));
  por = (mantisa)? (kol_m-1) : 0;
  return *this;
}
//---------------------------------------------------------------------------
CDataFilter CDataFilter::operator=(const double &d)
{
  znak = (d >= 0);
  int p;
  double d1 = frexp(fabs(d), &p);
  mantisa = d1 * (maskam+1);
  por = GetHexPor(p);
  return *this;
}
//---------------------------------------------------------------------------
CDataFilter CDataFilter::operator+(const CDataFilter &d)
{
  CDataFilter ret;
  unsigned man1, man2, por1, por2;
  bool znak1, znak2;
  if(GetIntPor(por) > GetIntPor(d.por))
  {
    man1 = mantisa; znak1 = znak;
    man2 = d.mantisa; znak2 = d.znak;
    por1 = por; por2 = d.por;
  }
  else
  {
    man2 = mantisa; znak2 = znak;
    man1 = d.mantisa; znak1 = d.znak;
    por1 = d.por; por2 = por;
  }
  unsigned por3 = GetHexPor(GetIntPor(por1) - GetIntPor(por2));
 // unsigned por3 = GetHexPor(por1 - por2);
  unsigned man3 = (por3 < (kol_m-1))? (man2 >> por3) : 0;
  bool znak3 = (por3 < (kol_m-1))? znak2 : true;
  unsigned man4, man5;
  bool znak4;
  if(man1 >= man3)
  {
    man4 = man1; znak4 = znak1;
    man5 = man3;
  }
  else
  {
    man4 = man3; znak4 = znak3;
    man5 = man1;
  }
  unsigned man6 = man4 + man5;
  unsigned man7 = man4 - man5;
  unsigned man8 = (znak1 ^ znak3)? man7 : man6;
  if(man8 == 0)
  {
    ret.por = 0;
    ret.mantisa = man1;
    ret.znak = true;
  }
  else
  {
    ret.znak = znak4;
    if(man8 & maskam_z)
    {
      ret.por = NormaPor(por1 + 1);
      ret.mantisa = NormaMan(man8 >> 1);
    }
    else
    {
      unsigned max = 1 << (kol_m - 1 - 1);
      unsigned lm = man8;
      unsigned sd = 0;
      for(int i = (kol_m - 1); i; i --, max = max >> 1)
        if(lm >= max)
        { sd = i; break;}
      ret.mantisa = NormaMan(man8 << ((kol_m-1)-sd));
      ret.por = NormaPor(por1 - NormaPor((kol_m-1)-sd));
    }
  }
  return ret;
}
//---------------------------------------------------------------------------
CDataFilter CDataFilter::operator-(const CDataFilter &d)
{
  CDataFilter ret;
  unsigned man1, man2, por1, por2;
  bool znak1, znak2;
  if(GetIntPor(por) >= GetIntPor(d.por))
  {
    man1 = mantisa; znak1 = znak;
    man2 = d.mantisa; znak2 = !d.znak;
    por1 = por; por2 = d.por;
  }
  else
  {
    man2 = mantisa; znak2 = znak;
    man1 = d.mantisa; znak1 = !d.znak;
    por1 = d.por; por2 = por;
  }
  unsigned por3 = GetHexPor(GetIntPor(por1) - GetIntPor(por2));
  unsigned man3 = (por3 < kol_m)? (man2 >> por3) : 0;
  bool znak3 = znak2;
  unsigned man4, man5;
  bool znak_res;
  if(man1 >= man3)
  {
    man4 = man1; znak_res = znak1;
    man5 = man3;
  }
  else
  {
    man4 = man3; znak_res = znak3;
    man5 = man1;
  }
  unsigned man6 = man4 + man5;
  unsigned man7 = man4 - man5;
  unsigned man8 = (znak1 ^ znak3)? man7 : man6;
  if(man8 == 0)
  {
    ret.por = 0;
    ret.mantisa = man1;
    ret.znak = true;
  }
  else
  {
    ret.znak = znak_res;
    if(man8 & maskam_z)
    {
      ret.por = NormaPor(por1 + 1);
      ret.mantisa = NormaMan(man8 >> 1);
    }
    else
    {
      unsigned max = 1 << (kol_m - 1 - 1);
      unsigned lm = man8;
      unsigned sd = 0;
      for(int i = (kol_m - 1); i; i --, max = max >> 1)
        if(lm >= max)
        { sd = i; break;}
      ret.mantisa = NormaMan(man8 << ((kol_m-1)-sd));
      ret.por = NormaPor(GetHexPor(GetIntPor(por1) - ((kol_m-1)-sd)));
   //   ret.por = NormaPor(por1 - NormaPor((kol_m-1)-sd));
    }
  }
  return ret;
}
//---------------------------------------------------------------------------
CDataFilter CDataFilter::operator*(const CDataFilter &d)
{
  CDataFilter ret;
  unsigned __int64 man1 = static_cast<unsigned __int64>(mantisa) * static_cast<unsigned __int64>(d.mantisa);
  if(man1 == 0)
  {
    ret.por = 0;
    ret.mantisa = 0;
    ret.znak = true;
  }
  else
  {
    ret.znak = !(znak ^ d.znak);
    unsigned __int64 max = static_cast<unsigned __int64>(1) << (2*(kol_m - 1) - 1);
    unsigned __int64 lm = man1;
    unsigned sd = 0;
    for(int i = (2*(kol_m - 1)); i; i --, max = max / 2)
      if(lm >= max)
      { sd = i; break;}
    if(sd > (kol_m-1))
      ret.mantisa = NormaMan((unsigned)(man1 >> (sd - (kol_m-1))));
    else
      ret.mantisa = NormaMan((unsigned)man1);
      //ret.mantisa = NormaMan((unsigned)(man1 << ((kol_m-1) - sd)));
    ret.por = NormaPor(NormaPor(por + d.por) + NormaPor(sd - 2*(kol_m-1)));
  }
  return ret;
}
//---------------------------------------------------------------------------
int CDataFilter::GetInt()
{
  int ret = NormaMan(mantisa);
  int sd = GetIntPor(por) - (kol_m-1);
  if(sd > 0)
    ret = 0;
  else
  {
    sd = abs(sd);
    if(sd >= (kol_m-1))
      ret = 0;
    else
      ret = NormaMan(mantisa >> sd);
  }
  return (znak)? ret : - ret;
}
//---------------------------------------------------------------------------
int CDataFilter::GetIntPor(unsigned d)
{
  unsigned maska_d   = 0xFFFFFFFF << kol_p;
  return (d & maskap_z)? (maska_d | NormaPor(d)) : NormaPor(d);
}
//---------------------------------------------------------------------------
double CDataFilter::GetDouble()
{
  long double ret = (double)mantisa/(double)(maskam+1);
  ret = ldexp(ret, GetIntPor(por));
  return (znak)? ret : - ret;
}
//---------------------------------------------------------------------------
unsigned CDataFilter::NormaMan(unsigned d)
{
  return d & maskam;
}
//---------------------------------------------------------------------------
unsigned CDataFilter::NormaPor(unsigned d)
{
  return d & (maskap | maskap_z);
}
//---------------------------------------------------------------------------
unsigned CDataFilter::GetHexPor(int d)
{
  return (d >= 0)? (d & maskap) : ((d & maskap) | maskap_z);
}
//---------------------------------------------------------------------------
AnsiString CDataFilter::GetZnak()
{
  if(znak)
    return "\'0\'";
  return "\'1\'";
}
//---------------------------------------------------------------------------
AnsiString CDataFilter::GetMon()
{
  AnsiString ret = "";
  unsigned m = mantisa;
  for(int i = 0; i < (kol_m-1); i ++, m /= 2)
    ret = ((m % 2)? "1" : "0") + ret;
  return "\"" + ret + "\"";
}
//---------------------------------------------------------------------------
AnsiString CDataFilter::GetPor()
{
  AnsiString ret = "";
  unsigned p = por;
  for(int i = 0; i < kol_p; i ++, p /= 2)
    ret = ((p % 2)? "1" : "0") + ret;
  return "\"" + ret + "\"";
}
//---------------------------------------------------------------------------

