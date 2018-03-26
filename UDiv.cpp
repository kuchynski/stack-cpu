//----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UDiv.h"
//----------------------------------------------------------------------------
#pragma resource "*.dfm"
TDivForm *DivForm;
//----------------------------------------------------------------------------
__fastcall TDivForm::TDivForm(TComponent *Owner)
	: TForm(Owner)
{
  AnsiString str_grid_div[5] = {"Имя", "Частота, Гц", "Выход", "Реальная частота, Гц", "Погрешность, %"};
  for(int i = 0; i < StringGridDiv->ColCount; i ++)
    StringGridDiv->Cells[i][0] = str_grid_div[i];//пишем заголовок таблицы
}
//----------------------------------------------------------------------------
bool TDivForm::Run(CTextOfGrid &op)
{
  op.PaintOnStringGrid(StringGridDiv);
  EditInputClk->Text = str_input_div;
  ButOk->Enabled = false;
  result = false;
  ShowModal();
  if(result)
  {
    op = CTextOfGrid("Делители1", StringGridDiv);
    str_input_div = EditInputClk->Text;
  }
  return result;
}
//----------------------------------------------------------------------------
void __fastcall TDivForm::ButAddClick(TObject *Sender)
{
  StringGridDiv->RowCount =  StringGridDiv->RowCount + 1;
  StringGridDiv->FixedRows =  1;
  int k = StringGridDiv->RowCount-1;
  while(k > StringGridDiv->Row)
  {
    for(int i = 0; i < StringGridDiv->ColCount; i ++)
      StringGridDiv->Cells[i][k] =  StringGridDiv->Cells[i][k-1];
    k --;
  }
  for(int i = 0; i < StringGridDiv->ColCount; i ++)
    StringGridDiv->Cells[i][StringGridDiv->Row] =  "";
}
//---------------------------------------------------------------------------
void __fastcall TDivForm::ButSubPortClick(TObject *Sender)
{
  if(StringGridDiv->Row < 1)
    return;
  int k = StringGridDiv->Row;
  while(k < (StringGridDiv->RowCount-1))
  {
    for(int i = 0; i < StringGridDiv->ColCount; i ++)
      StringGridDiv->Cells[i][k] =  StringGridDiv->Cells[i][k+1];
    k ++;
  }
  StringGridDiv->RowCount =  StringGridDiv->RowCount - 1;
}
//---------------------------------------------------------------------------
void __fastcall TDivForm::StringGridDivGetEditMask(TObject *Sender,
      int ACol, int ARow, AnsiString &Value)
{
  if(ACol == 1)
    Value = "00000000";
}
//---------------------------------------------------------------------------
bool TDivForm::Linker()
{
  ButOk->Enabled = false;
  ButRun->Click();
  return ButOk->Enabled;
}
//---------------------------------------------------------------------------
void __fastcall TDivForm::ButRunClick(TObject *Sender)
{
  div_frequency.Clear();
  AnsiString str_input_div = EditInputClk->Text;
  input_div = 0;
  for(int i = 1; i <= str_input_div.Length(); i ++)
    if(str_input_div[i] != ' ')
      input_div = input_div * 10 + (str_input_div[i] - '0');
  if(!input_div)
  {
    MessageBox(0, "Нет входной частоты?", "Частота процессора", MB_ICONSTOP | MB_TASKMODAL | MB_OK);
    return;
  }
  EditInputClk->Text = IntToStr(input_div);

  for(int i = 1; i < StringGridDiv->RowCount; i ++)
  {
    AnsiString name = "";
    for(int j = 1; j <= StringGridDiv->Cells[0][i].Length(); j ++)
      if(StringGridDiv->Cells[0][i][j] != ' ')
        name = name + StringGridDiv->Cells[0][i][j];
    StringGridDiv->Cells[0][i] = name;
    AnsiString str_div = StringGridDiv->Cells[1][i];
    int div = 0;
    for(int i = 1; i <= str_div.Length(); i ++)
      if(str_div[i] != ' ')
        div = div * 10 + (str_div[i] - '0');
    if(!div && StringGridDiv->Cells[0][i] != "")
    {
      AnsiString err = "Частота '" + name + "'";
      MessageBox(0, "Должна быть больше 0!", err.c_str(), MB_ICONSTOP | MB_TASKMODAL | MB_OK);
      continue;
    }
    if(!div) continue;
    if(div > input_div)
      div= input_div;
    StringGridDiv->Cells[1][i] = IntToStr(div);
    int k1 = input_div / div;
    int k2 = k1 + 1;
    float real_div1 = (float)input_div / (float)k1;
    float real_div2 = (float)input_div / (float)k2;
    float r1 = fabs((float)div - real_div1);
    float r2 = fabs((float)div - real_div2);
    float real_div = (r1 <= r2)? real_div1 : real_div2;
    int k = (r1 <= r2)? k1 : k2;
    StringGridDiv->Cells[3][i] = FloatToStrF(real_div , ffFixed, 9, 9);
    StringGridDiv->Cells[4][i] = FloatToStrF(fabs(100.0*((float)div-(float)real_div) / (float)div) , ffFixed, 3, 6);
    if(!div_frequency.AddDiv(name, k, StringGridDiv->Cells[2][i] == "да"))
    {
      AnsiString err = "Частота '" + name + "'";
      MessageBox(0, "С таким именем частота уже есть.", err.c_str(), MB_ICONSTOP | MB_TASKMODAL | MB_OK);
      continue;
    }
  }

  ButOk->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TDivForm::StringGridDivSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
  if(!ARow)
    return;
  switch(ACol)
  {
    case 0:
    case 1: StringGridDiv->Options << goEditing;
      ButOk->Enabled = false;
      break;
    case 3:
    case 4:
      StringGridDiv->Options >> goEditing;
      break;
    case 2:
      StringGridDiv->Options >> goEditing;
      if(StringGridDiv->Cells[ACol][ARow] == "")
        StringGridDiv->Cells[ACol][ARow] = "да";
      else
        StringGridDiv->Cells[ACol][ARow] = "";
      ButOk->Enabled = false;
  }
}
//---------------------------------------------------------------------------
void __fastcall TDivForm::ButOkClick(TObject *Sender)
{
  result = true;
}
//---------------------------------------------------------------------------
void __fastcall TDivForm::EditInputClkChange(TObject *Sender)
{
  ButOk->Enabled = false;
}
//---------------------------------------------------------------------------


