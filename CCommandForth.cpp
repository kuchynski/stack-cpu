//---------------------------------------------------------------------------
#pragma hdrstop

#include "CCommandForth.h"
//---------------------------------------------------------------------------
CCommandForth::CCommandForth(AnsiString n, unsigned k, AnsiString nc): name(n)
{
  if(nc == "")
    nc = n;
  commands.push_back(CCommand(nc, k));
}
//---------------------------------------------------------------------------
CCommandForth::CCommandForth(AnsiString n, unsigned k1, unsigned k2, AnsiString nc): name(n)
{
  if(nc == "")
    nc = n;
  commands.push_back(CCommand(nc, k1, k2));
}
//---------------------------------------------------------------------------
int CCommandForth::GetKolKod()
{
  int ret = 0;
  for(vector <CCommand>:: iterator p = commands.begin(); p < commands.end(); p ++)
    ret += p->GetKolKod();
  return ret;  
}
//---------------------------------------------------------------------------
bool CCommandForth::SetKod2(unsigned k2)
{
  if(commands.size() != 1)
    return false;
  return commands[0].SetKod2(k2);
}
//---------------------------------------------------------------------------
unsigned CCommandForth::SetAddress(unsigned a)
{
  address = a;
  unsigned address_end = a;
  for(vector <CCommand>:: iterator p = commands.begin(); p < commands.end(); p ++)
  {
    p->SetAddress(address_end);
    address_end += p->GetKolKod();
  }
  return address_end;
}
//---------------------------------------------------------------------------
unsigned CCommandForth::GetAddress()
{
  if(!commands.size())
    return 0;
  return commands[0].GetAddress();
}
//---------------------------------------------------------------------------
bool CCommandForth::SetCall(AnsiString n, unsigned k2)
{
  if(name != ("CALL " + n))
    return false;
  if(!commands.size())
    return false;
  commands[0].SetCall(k2);
  return true;
}
//---------------------------------------------------------------------------
bool CCommandForth::SetJMP(int m, unsigned k2)
{
  if(!commands.size())
    return false;
  for(vector <CCommand>:: iterator p = commands.begin(); p < commands.end(); p ++)
    p->SetJMP(m, k2);
  return true;
}
//---------------------------------------------------------------------------
AnsiString CCommandForth::GetAllCommandsName(int kol_hex)
{
  AnsiString ret;
  for(vector <CCommand>:: iterator p = commands.begin(); p < commands.end(); p ++)
    ret += (p->GetName(kol_hex) + " ");
  return ret;  
}
//---------------------------------------------------------------------------
bool CCommandForth::Run(unsigned a, unsigned &k1, unsigned &k2, int &kol)
{
  for(vector <CCommand>:: iterator p = commands.begin(); p < commands.end(); p ++)
    if(p->Run(a, k1, k2, kol))
      return true;
  return false;
}
//---------------------------------------------------------------------------
AnsiString CCommandForth::GetNameCommand(unsigned a)
{
  for(vector <CCommand>:: iterator p = commands.begin(); p < commands.end(); p ++)
    if(p->GetNameCommand(a) != "")
      return p->GetNameCommand(a);
  return "";
}
//---------------------------------------------------------------------------
void CCommandForth::SetMetka(vector<int> metka)
{
  if(commands.size())
    commands[0].SetMetka(metka);
}
//---------------------------------------------------------------------------
vector<int> CCommandForth::GetMetka(unsigned &a)
{
  vector<int> metka;
  for(vector <CCommand>:: iterator p = commands.begin(); p < commands.end(); p ++)
  {
    metka = p->GetMetka();
    if(metka.size())
    {
      a = GetAddress();
      break;
    }
  }
  return metka;
}
//---------------------------------------------------------------------------
CCommandForth CCommandForth::operator+=(const CCommandForth &c)
{
  for(int i = 0; i < c.commands.size(); i++)
    commands.push_back(c.commands[i]);
//  for(vector <CCommand>:: iterator p = c.commands.begin(); p < c.commands.end(); p ++)
//    commands.push_back(*p);
  return *this;  
}
//---------------------------------------------------------------------------
void CCommandForth::AddCommandForth(CCommandForth c)
{
  for(vector <CCommand>:: iterator p = c.commands.begin(); p < c.commands.end(); p ++)
    commands.push_back(*p);
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
