//---------------------------------------------------------------------------
#pragma hdrstop

#include "CWord.h"
//---------------------------------------------------------------------------
CWord::CWord(AnsiString n, TypeOfWord t):
  type(t), name(n), kol_use(0)
{}
//---------------------------------------------------------------------------
CStandartWord::CStandartWord(AnsiString n, int kd, bool re):
              CWord(n, twStandart), realy(re),kol_kod(kd)
{}
//---------------------------------------------------------------------------
void CStandartWord::SetKod(unsigned k1, unsigned k2)
{
  if(kol_kod < 2)
    kod = CCommandForth(GetName(), k1);
  else
    kod = CCommandForth(GetName(), k1, k2);
}
//---------------------------------------------------------------------------
bool CStandartWord::IsThe(AnsiString n, bool r)
{
  if(!r)
   return realy? CWord::IsThe(n) : false;
  return CWord::IsThe(n);
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
