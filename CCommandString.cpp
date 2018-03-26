//---------------------------------------------------------------------------
#pragma hdrstop

#include "CCommandString.h"
//---------------------------------------------------------------------------
CCommandString::CCommandString(const CCommandString &ks)
{
  str = ks.str;
  line = ks.line;
  page = ks.page;
}
//---------------------------------------------------------------------------
CCommandString CCommandString::operator=(const CCommandString &ks)
{
  if(this != &ks)
  {
    str = ks.str;
    line = ks.line;
    page = ks.page;
  }
  return *this;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
