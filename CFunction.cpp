//---------------------------------------------------------------------------
#pragma hdrstop

#include "CFunction.h"
//---------------------------------------------------------------------------
CFunction::CFunction(AnsiString n, vector <CCommandForth> c): name(n)
{
  for(vector <CCommandForth>:: iterator p = c.begin(); p < c.end(); p ++)
    AddCommandForth(*p);
}
//---------------------------------------------------------------------------
int CFunction::GetKolKod(int i)
{
  int ret = 0;
  if(i < 0)
    for(vector <CCommandForth>:: iterator p = commands.begin(); p < commands.end(); p ++)
      ret += p->GetKolKod();
  else
    if(i < commands.size())
      ret = commands[i].GetKolKod();
  return ret;
}
//---------------------------------------------------------------------------
int CFunction::GetKolCommands()
{
  int ret = 0;
  for(vector <CCommandForth>:: iterator p = commands.begin(); p < commands.end(); p ++)
    ret += p->GetKolCommands();
  return ret;
}
//---------------------------------------------------------------------------
void CFunction::AddCommandForth(CCommandForth c)
{
//  AnsiString call = "CALL" + c.
  commands.push_back(c);
}
//---------------------------------------------------------------------------
unsigned CFunction::SetAddress(unsigned a)
{
  address = a;
  unsigned address_end = a;
  for(vector <CCommandForth>:: iterator p = commands.begin(); p < commands.end(); p ++)
    address_end = p->SetAddress(address_end);
  SetJMP();  
  return address_end;
}
//---------------------------------------------------------------------------
unsigned CFunction::GetAddress()
{
  if(!commands.size())
    return 0;
  return commands[0].GetAddress();
}
//---------------------------------------------------------------------------
bool CFunction::SetCall(AnsiString n, unsigned k2)
{
  for(vector <CCommandForth>:: iterator p = commands.begin(); p < commands.end(); p ++)
    p->SetCall(n, k2);
  return true;
}
//---------------------------------------------------------------------------
void CFunction::SetJMP()
{
  unsigned address;
  vector<int> metka;
  for(vector <CCommandForth>:: iterator p = commands.begin(); p < commands.end(); p ++)
  {
    metka.clear();
    metka = p->GetMetka(address);
    for(vector <int>:: iterator i = metka.begin(); i < metka.end(); i ++)
      for(vector <CCommandForth>:: iterator r = commands.begin(); r < commands.end(); r ++)
        r->SetJMP(*i, address);
  }
  metka.clear();
}
//---------------------------------------------------------------------------
AnsiString CFunction::GetCommandForthName(int i)
{
  if((i >= commands.size()) || (i < 0))
    return "";
  return commands[i].GetName();
}
//---------------------------------------------------------------------------
int CFunction::GetPage(int i)
{
  if((i >= commands.size()) || (i < 0))
    return 0;
  return commands[i].GetPage();
}
//---------------------------------------------------------------------------
int CFunction::GetLine(int i)
{
  if((i >= commands.size()) || (i < 0))
    return 0;
  return commands[i].GetLine();
}
//---------------------------------------------------------------------------
int CFunction::GetLineReal(int i)
{
  if((i >= commands.size()) || (i < 0))
    return 0;
  return commands[i].GetLineReal();
}
//---------------------------------------------------------------------------
AnsiString CFunction::GetCommandName2(int i, int kol_hex)
{
  if((i >= commands.size()) || (i < 0))
    return "";
  return commands[i].GetAllCommandsName(kol_hex);
}
//---------------------------------------------------------------------------
bool CFunction::Run(unsigned a, unsigned &k1, unsigned &k2, int &kol)
{
  for(vector <CCommandForth>:: iterator p = commands.begin(); p < commands.end(); p ++)
    if(p->Run(a, k1, k2, kol))
      return true;
  kol = 0;    
  return false;
}
//---------------------------------------------------------------------------
AnsiString CFunction::GetNameCommand(unsigned a)
{
  for(vector <CCommandForth>:: iterator p = commands.begin(); p < commands.end(); p ++)
    if(p->GetNameCommand(a) != "")
      return p->GetNameCommand(a);
  return "";
}
//---------------------------------------------------------------------------
CCommandForth* CFunction::GetCommandFort(unsigned a)
{
  for(vector <CCommandForth>:: iterator p = commands.begin(); p < commands.end(); p ++)
    if(p->GetNameCommand(a) != "")
      return p;
  return NULL;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
