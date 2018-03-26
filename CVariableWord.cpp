//---------------------------------------------------------------------------
#pragma hdrstop

#include "CVariableWord.h"
//---------------------------------------------------------------------------
CVariableWord::CVariableWord(AnsiString n, TypeOfWord t, bool fg, int kol, int *v):
               CWord(n, t), from_grid(fg)
{
  if(kol < 1)
    throw this;
  if(t == twConst)
    value.push_back(v? v[0] : 0);
  else
    for(int i = 0; i < kol; i ++)
      value.push_back(v? v[i] : 0);
}
//---------------------------------------------------------------------------
void CVariableWord::SetKod(unsigned k1, unsigned k2)
{
  address = k2;
  kod = CCommandForth(GetName(), k1, k2, (GetType() == twConst)? "CONSTANT" : "ADDRESS");
//  CWord::SetKod(k1, k2);
}
//---------------------------------------------------------------------------
unsigned CVariableWord::SetAddress(unsigned a)
{
  address = a;
  return address + value.size();
}
//---------------------------------------------------------------------------
bool CVariableWord::GetValue(unsigned a, int &v)
{
  if(a < address)
    return false;
  if((a - address) >= value.size()) 
    return false;
  v = value[a - address];  
  return true;
}
//---------------------------------------------------------------------------
bool CVariableWord::SetValue(unsigned a, int v)
{
  if(a < address)
    return false;
  if((a - address) >= value.size()) 
    return false;
  value[a - address] = v;  
  return true;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
