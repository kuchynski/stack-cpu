//---------------------------------------------------------------------------
#pragma hdrstop

#include "CModuls.h"
#include "COptions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

CModuls moduls;
//---------------------------------------------------------------------------
SModul::SModul(FILE *f): name(f), entity("Entity1", 4, f), signals(f), arch(f) {}
//---------------------------------------------------------------------------
void SModul::SaveToFile(FILE *f)
{
  name.SaveToFile(f);
  entity.SaveToFile(f);
  signals.SaveToFile(f);
  arch.SaveToFile(f);
}
//---------------------------------------------------------------------------
CModuls::~CModuls()
{
   Clear();
}
//---------------------------------------------------------------------------
void CModuls::Clear()
{
  modules.clear();
}
//---------------------------------------------------------------------------
bool CModuls::LoadFromFile(FILE *f)
{
  modules.clear();
  int size;
  if(fread(&size, sizeof(size), 1, f) != 1)
    return false;
  for(int i = 0; i < size; i++)
    modules.push_back(SModul(f));
  return true;
}
//---------------------------------------------------------------------------
bool CModuls::SaveToFile(FILE *f)
{
  int size = modules.size();
  if(fwrite(&size, sizeof(size), 1, f) != 1)
    return false;
  for(vector <SModul>:: iterator p = modules.begin(); p < modules.end(); p ++)
    p->SaveToFile(f);
  return true;
}
//---------------------------------------------------------------------------
bool CModuls::Sub(int n)
{
  if((n >= modules.size()) || (n < 0))
    return false;
  vector <SModul>:: iterator p = modules.begin() + n;
  modules.erase(p);
  return true;
}
//---------------------------------------------------------------------------
bool CModuls::Add(int n, SModul &f)
{
  if((n > modules.size()) || (n < 0))
    return false;
  vector <SModul>:: iterator p = modules.begin() + n;
  int s1= modules.size();
  modules.insert(p, f);
  int s2= modules.size();
}
//---------------------------------------------------------------------------
bool CModuls::Set(int n, SModul &f)
{
  if((n >= modules.size()) || (n < 0))
    return false;
  vector <SModul>:: iterator p = modules.begin() + n;
  Sub(n);
  Add(n, f);
}
//---------------------------------------------------------------------------
SModul CModuls::GetModul(int n)
{
  if((n >= modules.size()) || (n < 0))
    return SModul();
  vector <SModul>:: iterator p = modules.begin() + n;
  return *p;
}
//---------------------------------------------------------------------------
void CModuls::PrintVHDLSignals(TMemo *memo)
{
  for(vector <SModul>:: iterator p = modules.begin(); p < modules.end(); p ++)
    memo->Lines->Add(p->signals.AnsiString());
}
//---------------------------------------------------------------------------
void CModuls::PrintVHDLProcess(TMemo *memo)
{
  for(vector <SModul>:: iterator p = modules.begin(); p < modules.end(); p ++)
    memo->Lines->Add(p->arch.AnsiString());
}
//---------------------------------------------------------------------------
