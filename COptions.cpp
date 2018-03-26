//---------------------------------------------------------------------------
#pragma hdrstop
#include "COptions.h"

COptionsProc options_proc;
//---------------------------------------------------------------------------
COptionsProc::COptionsProc():
  znak(false), razrad(8), kol_reg_stackdata(8), kol_reg_stackreturn(8){}
//---------------------------------------------------------------------------
COptionsProc::COptionsProc(long c, bool z, long r, long krsd, long krsr):
  chip(c), znak(z), razrad(r), kol_reg_stackdata(krsd), kol_reg_stackreturn(krsr){}
//---------------------------------------------------------------------------
COptionsProc::COptionsProc(FILE *f):
  znak(false), razrad(8), kol_reg_stackdata(8), kol_reg_stackreturn(8)
{
  if(fread(&chip, sizeof(chip), 1, f) != 1)
    return;
  if(fread(&znak, sizeof(znak), 1, f) != 1)
    return;
  if(fread(&razrad, sizeof(razrad), 1, f) != 1)
    return;
  if(fread(&kol_reg_stackdata, sizeof(kol_reg_stackdata), 1, f) != 1)
    return;
  if(fread(&kol_reg_stackreturn, sizeof(kol_reg_stackreturn), 1, f) != 1)
    return;
  if(fread(zapas, sizeof(zapas), 1, f) != 1)
    return;
}
//---------------------------------------------------------------------------
COptionsProc::COptionsProc(const COptionsProc &op)
{
  chip = op.chip;
  znak = op.znak;
  razrad = op.razrad;
  kol_reg_stackdata = op.kol_reg_stackdata;
  kol_reg_stackreturn = op.kol_reg_stackreturn;
}
//---------------------------------------------------------------------------
COptionsProc COptionsProc::operator=(const COptionsProc &op)
{
  if(this != &op)
  {
    chip = op.chip;
    znak = op.znak;
    razrad = op.razrad;
    kol_reg_stackdata = op.kol_reg_stackdata;
    kol_reg_stackreturn = op.kol_reg_stackreturn;
  }
  return *this;
}
//---------------------------------------------------------------------------
bool COptionsProc::SaveToFile(FILE *f)const
{
  if(fwrite(&chip, sizeof(chip), 1, f) != 1)
    return false;
  if(fwrite(&znak, sizeof(znak), 1, f) != 1)
    return false;
  if(fwrite(&razrad, sizeof(razrad), 1, f) != 1)
    return false;
  if(fwrite(&kol_reg_stackdata, sizeof(kol_reg_stackdata), 1, f) != 1)
    return false;
  if(fwrite(&kol_reg_stackreturn, sizeof(kol_reg_stackreturn), 1, f) != 1)
    return false;
  if(fwrite(zapas, sizeof(zapas), 1, f) != 1)
    return false;
  return true;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
CTextOfGrid::CTextOfGrid(AnsiString str, int s, FILE *f): size(s), name(str)
{
  int kol;
  AnsiString str_f = CString(f).AnsiString();
  if(str_f != str)
    return;
  if(fread(&kol, sizeof(kol), 1, f) != 1)
    return;
  try
  {
    for(int i = 0; i < kol; i ++)
    {
      SStroka ss;
      for(int j = 0; j < size; j ++)
        ss.str[j] = CString(f);
      stroki.push_back(ss);
    }
  }
  catch(CError)
  {
  }
}
//---------------------------------------------------------------------------
CTextOfGrid::CTextOfGrid(AnsiString str, TStringGrid *sg, bool fixed_row): name(str)
{
  size = sg->ColCount;
  int begin = (fixed_row)? 1 : 0;
  for(int i = begin; i < sg->RowCount; i ++)
  {
    SStroka ss;
    for(int j = 0; j < sg->ColCount; j ++)
      ss.str[j] = sg->Cells[j][i].c_str();
    stroki.push_back(ss);
  }    
}
//---------------------------------------------------------------------------
CTextOfGrid::CTextOfGrid(const CTextOfGrid &op)
{
  name = op.name;
  size = op.size;
  stroki = op.stroki;
}
//---------------------------------------------------------------------------
CTextOfGrid CTextOfGrid::operator=(const CTextOfGrid &op)
{
  if(this != &op)
  {
    name = op.name;
    size = op.size;
    stroki.erase(stroki.begin(), stroki.end());
    stroki = op.stroki;
  }
  return *this;
}
//---------------------------------------------------------------------------
void CTextOfGrid::PaintOnStringGrid(TStringGrid *sg, bool fixed_row)
{
  int begin = (fixed_row)? 1 : 0;
  sg->RowCount = stroki.size() + begin;
  if(sg->RowCount > 1 && fixed_row)
    sg->FixedRows =  1;
  if(!stroki.size())
    return;
  vector<SStroka>:: iterator p = stroki.begin();
  for(int i = begin; i < sg->RowCount; i ++, p++)
    for(int j = 0; j < sg->ColCount; j ++)
      sg->Cells[j][i] = p->str[j].AnsiString();
}
//---------------------------------------------------------------------------
AnsiString CTextOfGrid::GetPosition(int x, int y) 
{
  if((y < 0) || (y >= size) || (x < 0) || (x > 9))
    return "";
  vector<SStroka>:: iterator p = stroki.begin() + y;
  return p->str[x].AnsiString();
}
//---------------------------------------------------------------------------
bool CTextOfGrid::SaveToFile(FILE *f)
{
  int kol = stroki.size();

  CString str = name.c_str();
  str.SaveToFile(f);
  if(fwrite(&kol, sizeof(kol), 1, f) != 1)
    return false;
  try
  {
    for(vector<SStroka>:: iterator p = stroki.begin(); p < stroki.end(); p ++)
      for(int j = 0; j < size; j ++)
        p->str[j].SaveToFile(f);
  }     
  catch(CError)
  {
    return false;
  }
  return true;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
