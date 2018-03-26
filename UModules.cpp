//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UModules.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TModulForm *ModulForm;
//---------------------------------------------------------------------------
__fastcall TModulForm::TModulForm(TComponent* Owner) : TForm(Owner)
{
  AnsiString str_grid_modul[1] = {"Имя"};
  for(int i = 0; i < StringGridModul->ColCount; i ++)
    StringGridModul->Cells[i][0] = str_grid_modul[i];//пишем заголовок таблицы
  AnsiString str_grid_entity[4] = {"Имя", "Разрядность", "in/out", "external/internal"};
  for(int i = 0; i < grid_entity->ColCount; i ++)
    grid_entity->Cells[i][0] = str_grid_entity[i];//пишем заголовок таблицы
}
//---------------------------------------------------------------------------
bool TModulForm::Run(CTextOfGrid &tog_modul)
{
  result = false;
  tog_modul.PaintOnStringGrid(StringGridModul);
  moduls_new = moduls;
  if(StringGridModul->Row)
    PrintModul(StringGridModul->Row-1);
  ShowModal();
  if(result)
  {
    tog_modul = CTextOfGrid("Модули1", StringGridModul);
    moduls = moduls_new;
  }
  return result;
}
//---------------------------------------------------------------------------
void __fastcall TModulForm::OKBtnClick(TObject *Sender)
{
  if(StringGridModul->Row)
    SetModul(StringGridModul->Row-1);
  result = true;
}
//---------------------------------------------------------------------------
void __fastcall TModulForm::CancelBtnClick(TObject *Sender)
{
  result = false;
}
//---------------------------------------------------------------------------
void TModulForm::PrintModul(int n)
{
  SModul sf = moduls_new.GetModul(n);
  sf.entity.PaintOnStringGrid(grid_entity);
  memo_signals->Lines->Text = sf.signals.AnsiString();
  memo_arch->Lines->Text = sf.arch.AnsiString();
}
//---------------------------------------------------------------------------
void TModulForm::SetModul(int n)
{
  SModul sf = moduls_new.GetModul(n);
  sf.name = CString(StringGridModul->Cells[0][n+1].c_str());
  sf.entity = CTextOfGrid("Entity1", grid_entity);
  sf.signals = CString(memo_signals->Lines->Text.c_str());
  sf.arch = CString(memo_arch->Lines->Text.c_str());
  moduls_new.Set(n, sf);
}
//---------------------------------------------------------------------------
void __fastcall TModulForm::grid_entityGetEditMask(TObject *Sender,
      int ACol, int ARow, AnsiString &Value)
{
  if(ACol == 1)
    Value = "00";
}
//---------------------------------------------------------------------------
void __fastcall TModulForm::grid_entitySelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
  if(!ARow)
  {
    grid_entity->Options >> goEditing;
    return;
  }
  switch(ACol)
  {
    case 0: case 1: grid_entity->Options << goEditing; break;
    case 2: grid_entity->Options >> goEditing;
      if(grid_entity->Cells[ACol][ARow] == "in")
        grid_entity->Cells[ACol][ARow] = "out";
      else
        grid_entity->Cells[ACol][ARow] = "in";
      break;
    case 3: grid_entity->Options >> goEditing;
      if(grid_entity->Cells[ACol][ARow] == "external")
        grid_entity->Cells[ACol][ARow] = "internal";
      else
        grid_entity->Cells[ACol][ARow] = "external";
      break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TModulForm::BitBtn1Click(TObject *Sender)
{
  grid_entity->RowCount =  grid_entity->RowCount + 1;
  grid_entity->FixedRows =  1;
  int k = grid_entity->RowCount-1;
  while(k > grid_entity->Row)
  {
    for(int i = 0; i < grid_entity->ColCount; i ++)
      grid_entity->Cells[i][k] =  grid_entity->Cells[i][k-1];
    k --;
  }
  grid_entity->Cells[0][grid_entity->Row] =  "";
  grid_entity->Cells[1][grid_entity->Row] =  "1";
  grid_entity->Cells[2][grid_entity->Row] =  "in";
  grid_entity->Cells[3][grid_entity->Row] =  "external";
}
//---------------------------------------------------------------------------
void __fastcall TModulForm::BitBtn2Click(TObject *Sender)
{
  if(grid_entity->Row < 1)
    return;
  int k = grid_entity->Row;
  while(k < (grid_entity->RowCount-1))
  {
    for(int i = 0; i < grid_entity->ColCount; i ++)
      grid_entity->Cells[i][k] = grid_entity->Cells[i][k+1];
    k ++;
  }
  grid_entity->RowCount = grid_entity->RowCount - 1;
}
//---------------------------------------------------------------------------
void __fastcall TModulForm::StringGridModulSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect)
{
  if(!ARow)
  {
    StringGridModul->Options >> goEditing;
    return;
  }
  StringGridModul->Options << goEditing;
  if(StringGridModul->Row != ARow)
  {
    if(StringGridModul->Row)
      SetModul(StringGridModul->Row-1);
    if(ARow)
      PrintModul(ARow-1);
  }
}
//---------------------------------------------------------------------------
void __fastcall TModulForm::grid_entitySetEditText(TObject *Sender,
      int ACol, int ARow, const AnsiString Value)
{
  if(!ARow)
  {
    grid_entity->Options >> goEditing;
    return;
  }
}
//---------------------------------------------------------------------------
void __fastcall TModulForm::StringGridModulSetEditText(TObject *Sender,
      int ACol, int ARow, const AnsiString Value)
{
  if(!ARow)
  {
    StringGridModul->Options >> goEditing;
    return;
  }
}
//---------------------------------------------------------------------------
void __fastcall TModulForm::ButAddClick(TObject *Sender)
{
  StringGridModul->RowCount =  StringGridModul->RowCount + 1;
  StringGridModul->FixedRows =  1;
  int k = StringGridModul->RowCount-1;
  while(k > StringGridModul->Row)
  {
    for(int i = 0; i < StringGridModul->ColCount; i ++)
      StringGridModul->Cells[i][k] =  StringGridModul->Cells[i][k-1];
    k --;
  }
  for(int i = 0; i < StringGridModul->ColCount; i ++)
    StringGridModul->Cells[i][StringGridModul->Row] =  "";
  if(StringGridModul->RowCount == 2)
    moduls_new.Add(0 ,SModul());
  else
    moduls_new.Add(StringGridModul->Row-1 ,SModul());
}
//---------------------------------------------------------------------------

void __fastcall TModulForm::ButSubClick(TObject *Sender)
{
  if(StringGridModul->Row < 1)
    return;
  int k = StringGridModul->Row;
  while(k < (StringGridModul->RowCount-1))
  {
    for(int i = 0; i < StringGridModul->ColCount; i ++)
      StringGridModul->Cells[i][k] = StringGridModul->Cells[i][k+1];
    k ++;
  }
  StringGridModul->RowCount = StringGridModul->RowCount - 1;
  moduls_new.Sub(StringGridModul->Row-1);
}
//---------------------------------------------------------------------------

