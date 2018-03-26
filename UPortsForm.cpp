//----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UPortsForm.h"
//----------------------------------------------------------------------------
#pragma resource "*.dfm"
TPortsForm *PortsForm;
//----------------------------------------------------------------------------
__fastcall TPortsForm::TPortsForm(TComponent *Owner)
	: TForm(Owner)
{
  AnsiString str_grid_ports[7] = {"Имя", "Разрядность", "Вход", "Прерывание", "Приоритет", "Выход", "Выход irq"};
  for(int i = 0; i < StringGridPorts->ColCount; i ++)
    StringGridPorts->Cells[i][0] = str_grid_ports[i];//пишем заголовок таблицы
}
//----------------------------------------------------------------------------
bool TPortsForm::Run(CTextOfGrid &op)
{
  op.PaintOnStringGrid(StringGridPorts);
  result = false;
  ShowModal();
  if(result)
    op = CTextOfGrid("Порты1", StringGridPorts);
  return result;
}
//----------------------------------------------------------------------------
void __fastcall TPortsForm::StringGridPortsSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
  if(!ARow)
    return;
  switch(ACol)
  {
    case 0:
    case 1:
    case 4: StringGridPorts->Options << goEditing; break;
    case 2:
    case 5:
    case 6:
      if(StringGridPorts->Cells[ACol][ARow] == "")
      {
        StringGridPorts->Cells[ACol][ARow] = "есть";
        if(ACol == 2)
          StringGridPorts->Cells[3][ARow] = "IRQ_" + StringGridPorts->Cells[0][ARow];
      }
      else
      {
        StringGridPorts->Cells[ACol][ARow] = "";
        if(ACol == 2)
          StringGridPorts->Cells[3][ARow] = "";
        if(ACol == 5)
          StringGridPorts->Cells[6][ARow] = "";
      }
      break;
    case 3:
      StringGridPorts->Options >> goEditing;
  }
}
//---------------------------------------------------------------------------
void __fastcall TPortsForm::ButAddClick(TObject *Sender)
{
  StringGridPorts->RowCount =  StringGridPorts->RowCount + 1;
  StringGridPorts->FixedRows =  1;
  int k = StringGridPorts->RowCount-1;
  while(k > StringGridPorts->Row)
  {
    for(int i = 0; i < StringGridPorts->ColCount; i ++)
      StringGridPorts->Cells[i][k] =  StringGridPorts->Cells[i][k-1];
    k --;
  }
  for(int i = 0; i < StringGridPorts->ColCount; i ++)
    StringGridPorts->Cells[i][StringGridPorts->Row] =  "";
}
//---------------------------------------------------------------------------
void __fastcall TPortsForm::ButSubPortClick(TObject *Sender)
{
  if(StringGridPorts->Row < 1)
    return;
  int k = StringGridPorts->Row;
  while(k < (StringGridPorts->RowCount-1))
  {
    for(int i = 0; i < StringGridPorts->ColCount; i ++)
      StringGridPorts->Cells[i][k] =  StringGridPorts->Cells[i][k+1];
    k ++;
  }
  StringGridPorts->RowCount =  StringGridPorts->RowCount - 1;
}
//---------------------------------------------------------------------------
void __fastcall TPortsForm::ButtonOkClick(TObject *Sender)
{
  result = true;
}
//---------------------------------------------------------------------------
void __fastcall TPortsForm::StringGridPortsGetEditMask(TObject *Sender,
      int ACol, int ARow, AnsiString &Value)
{
  if((ACol == 1) || (ACol == 4))
    Value = "00";
}
//---------------------------------------------------------------------------
void __fastcall TPortsForm::StringGridPortsSetEditText(TObject *Sender,
      int ACol, int ARow, const AnsiString Value)
{
  if(!ACol)
    if(StringGridPorts->Cells[3][StringGridPorts->Row] != "")
      StringGridPorts->Cells[3][StringGridPorts->Row] = "IRQ_" + Value;
}
//---------------------------------------------------------------------------

