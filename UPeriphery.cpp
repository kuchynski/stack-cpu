//----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UPeriphery.h"
//----------------------------------------------------------------------------
#pragma resource "*.dfm"
TPeripheryForm *PeripheryForm;
//----------------------------------------------------------------------------
__fastcall TPeripheryForm::TPeripheryForm(TComponent *Owner)
	: TForm(Owner)
{
  AnsiString str_grid_uart[9] = {"Имя", "Разрядность", "Частота", "Приём", "Прерывание ",
                    "Приоритет", "Передача", "Прерывание ", "Приоритет"};
  for(int i = 0; i < StringGridUart->ColCount; i ++)
    StringGridUart->Cells[i][0] = str_grid_uart[i];//пишем заголовок таблицы
  AnsiString str_grid_timer[5] = {"Имя", "Разрядность", "Частота", "Прерывание ", "Приоритет"};
  for(int i = 0; i < StringGridTimer->ColCount; i ++)
    StringGridTimer->Cells[i][0] = str_grid_timer[i];//пишем заголовок таблицы
}
//----------------------------------------------------------------------------
bool TPeripheryForm::Run(CTextOfGrid &tog_uart, CTextOfGrid &tog_timer)
{
  tog_uart.PaintOnStringGrid(StringGridUart);
  tog_timer.PaintOnStringGrid(StringGridTimer);
  result = false;
  ShowModal();
  if(result)
  {
    tog_uart = CTextOfGrid("Uarts1", StringGridUart);
    tog_timer = CTextOfGrid("Таймеры1", StringGridTimer);
  }
  return result;
}
//---------------------------------------------------------------------------
void __fastcall TPeripheryForm::StringGridUartGetEditMask(TObject *Sender,
      int ACol, int ARow, AnsiString &Value)
{
  if((ACol == 1) || (ACol == 5) || (ACol == 8))
    Value = "00";
}
//---------------------------------------------------------------------------
void __fastcall TPeripheryForm::StringGridUartSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
  if(!ARow)
    return;
  switch(ACol)
  {
    case 0: case 1: case 2: case 5: case 8: StringGridUart->Options << goEditing; break;
    case 3: case 6:
    {
      StringGridUart->Options >> goEditing;
      if(StringGridUart->Cells[ACol][ARow] == "")
      {
        StringGridUart->Cells[ACol][ARow] = "есть";
        if(ACol == 3)
          StringGridUart->Cells[4][ARow] = "IRQ_" + StringGridUart->Cells[0][ARow] + "_in";
        if(ACol == 6)
          StringGridUart->Cells[7][ARow] = "IRQ_" + StringGridUart->Cells[0][ARow] + "_out";
      }
      else
      {
        StringGridUart->Cells[ACol][ARow] = "";
        if(ACol == 3)
          StringGridUart->Cells[4][ARow] = "";
        if(ACol == 6)
          StringGridUart->Cells[7][ARow] = "";
      }
      break;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TPeripheryForm::ButOkClick(TObject *Sender)
{
  result = true;
}
//---------------------------------------------------------------------------
void __fastcall TPeripheryForm::StringGridUartSetEditText(TObject *Sender,
      int ACol, int ARow, const AnsiString Value)
{
  if(!ACol)
    if(StringGridUart->Cells[4][StringGridUart->Row] != "")
      StringGridUart->Cells[4][StringGridUart->Row] = "IRQ_" + Value + "_in";
  if(!ACol)
    if(StringGridUart->Cells[7][StringGridUart->Row] != "")
      StringGridUart->Cells[7][StringGridUart->Row] = "IRQ_" + Value + "_out";
}
//----------------------------------------------------------------------------
void __fastcall TPeripheryForm::ButAddUartClick(TObject *Sender)
{
  StringGridUart->RowCount =  StringGridUart->RowCount + 1;
  StringGridUart->FixedRows =  1;
  int k = StringGridUart->RowCount-1;
  while(k > StringGridUart->Row)
  {
    for(int i = 0; i < StringGridUart->ColCount; i ++)
      StringGridUart->Cells[i][k] =  StringGridUart->Cells[i][k-1];
    k --;
  }
  for(int i = 0; i < StringGridUart->ColCount; i ++)
    StringGridUart->Cells[i][StringGridUart->Row] =  "";
}
//---------------------------------------------------------------------------
void __fastcall TPeripheryForm::ButSubUartClick(TObject *Sender)
{
  if(StringGridUart->Row < 1)
    return;
  int k = StringGridUart->Row;
  while(k < (StringGridUart->RowCount-1))
  {
    for(int i = 0; i < StringGridUart->ColCount; i ++)
      StringGridUart->Cells[i][k] =  StringGridUart->Cells[i][k+1];
    k ++;
  }
  StringGridUart->RowCount =  StringGridUart->RowCount - 1;
}
//---------------------------------------------------------------------------
void __fastcall TPeripheryForm::ButAddTimerClick(TObject *Sender)
{
  StringGridTimer->RowCount =  StringGridTimer->RowCount + 1;
  StringGridTimer->FixedRows =  1;
  int k = StringGridTimer->RowCount-1;
  while(k > StringGridTimer->Row)
  {
    for(int i = 0; i < StringGridTimer->ColCount; i ++)
      StringGridTimer->Cells[i][k] =  StringGridTimer->Cells[i][k-1];
    k --;
  }
  for(int i = 0; i < StringGridTimer->ColCount; i ++)
    StringGridTimer->Cells[i][StringGridTimer->Row] =  "";
}
//---------------------------------------------------------------------------
void __fastcall TPeripheryForm::BuSubTimerClick(TObject *Sender)
{
  if(StringGridTimer->Row < 1)
    return;
  int k = StringGridTimer->Row;
  while(k < (StringGridTimer->RowCount-1))
  {
    for(int i = 0; i < StringGridTimer->ColCount; i ++)
      StringGridTimer->Cells[i][k] =  StringGridTimer->Cells[i][k+1];
    k ++;
  }
  StringGridTimer->RowCount =  StringGridTimer->RowCount - 1;
}
//---------------------------------------------------------------------------
void __fastcall TPeripheryForm::StringGridTimerSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
  if(!ARow)
    return;
  switch(ACol)
  {
    case 0: case 1: case 2: case 4: StringGridTimer->Options << goEditing; break;
    case 3: StringGridTimer->Options >> goEditing;
  }
}
//---------------------------------------------------------------------------
void __fastcall TPeripheryForm::StringGridTimerSetEditText(TObject *Sender,
      int ACol, int ARow, const AnsiString Value)
{
  if(!ACol)
    StringGridTimer->Cells[3][StringGridTimer->Row] = "IRQ_" + Value;
}
//---------------------------------------------------------------------------
void __fastcall TPeripheryForm::StringGridTimerGetEditMask(TObject *Sender,
      int ACol, int ARow, AnsiString &Value)
{
  if((ACol == 1) || (ACol == 4))
    Value = "00";
}
//---------------------------------------------------------------------------

