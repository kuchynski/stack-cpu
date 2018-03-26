//---------------------------------------------------------------------------
#pragma hdrstop

#include "CCommand.h"
#include "CDictionary.h"
//---------------------------------------------------------------------------
CCommand::CCommand(AnsiString n, unsigned k):
  name(n), kod1(k), kol_kod(1){}
//---------------------------------------------------------------------------
CCommand::CCommand(AnsiString n, unsigned k1, unsigned k2):
  name(n), kod1(k1), kod2(k2), kol_kod(2)
{}
//---------------------------------------------------------------------------
bool CCommand::SetKod2(unsigned k2)
{
  if(kol_kod == 2)
    kod2 = k2;
  return kol_kod == 2;
}
//---------------------------------------------------------------------------
bool CCommand::SetCall(unsigned k2)
{
  if(name != "CALL")
    return false;
  kod2 = k2;
  return true;
}
//---------------------------------------------------------------------------
bool CCommand::SetJMP(int m, unsigned k2)
{
  if((name != "JMP") && (name != "JZ"))
    return false;
  if(kod2 == m)
    kod2 = k2;
  else
    return false;  
  return true;
}
//---------------------------------------------------------------------------
AnsiString CCommand::GetName(int kol_hex)
{
  if(kol_kod == 2)
    return name + " " + IntToHex((int)kod2, kol_hex);
  return name;  
}
//---------------------------------------------------------------------------
bool CCommand::Run(unsigned a, unsigned &k1, unsigned &k2, int &kol)
{
  if(address != a)
    return false;
  k1 = kod1;
  k2 = kod2;
  kol = kol_kod;
  return true;
}
//---------------------------------------------------------------------------
AnsiString CCommand::GetNameCommand(unsigned a)
{
  if(address != a)
    return "";
  return name;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
