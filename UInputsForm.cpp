//----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UInputsForm.h"
//----------------------------------------------------------------------------
#pragma resource "*.dfm"
TInputForm *InputForm;
//----------------------------------------------------------------------------
__fastcall TInputForm::TInputForm(TComponent *Owner)
	: TForm(Owner)
{
  AnsiString str_grid_ports[3] = {"Адрес", "Имя", "Вход"};
  for(int i = 0; i < grid_ports->ColCount; i ++)
    grid_ports->Cells[i][0] = str_grid_ports[i];//пишем заголовок таблицы
  AnsiString str_grid_irq[2] = {"Имя", "Состояние"};
  for(int i = 0; i < grid_irq->ColCount; i ++)
    grid_irq->Cells[i][0] = str_grid_irq[i];//пишем заголовок таблицы
}
//----------------------------------------------------------------------------
void TInputForm::Run()
{
  new_ports = ports;
  new_IRQ = IRQ;
  ok = false;
  new_ports.PrintAllInputs(grid_ports);
  IRQ.PrintAll2(grid_irq);
  ShowModal();
  if(ok)
  {
    ports = new_ports;
    IRQ = new_IRQ;
    ports.PrintValue();
    IRQ.PrintValue();
  }
}
//----------------------------------------------------------------------------
void __fastcall TInputForm::grid_portsSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
  if(!ARow)
    return;
  if(ACol == 2)
    grid_ports->Options << goEditing;
  else
    grid_ports->Options >> goEditing;
}
//---------------------------------------------------------------------------
void __fastcall TInputForm::grid_irqSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
  if(!ARow)
    return;
  if(ACol == 1)
    if(grid_irq->Cells[1][ARow] == "активное")
      grid_irq->Cells[1][ARow] = "";
    else
      grid_irq->Cells[1][ARow] = "активное";
}
//---------------------------------------------------------------------------
void __fastcall TInputForm::ButOkClick(TObject *Sender)
{
  unsigned StrToHex(AnsiString str);

  for(int i = 1; i < grid_ports->RowCount; i ++)
  {
    char str[1000];
    strcpy(str, grid_ports->Cells[2][i].c_str());
    int j = 0;
    while((str[j] == ' ') && str[j]) i++;
    if(!str[j]) continue;
    AnsiString v;
    while((str[j] != ' ') && (str[j] != ',') && str[j])
      v += str[j++];
    unsigned c;
    if(!dictionary.GetConstValueHex(v, c))
    {
      AnsiString err = "Непонятное значение в строке." + IntToStr(i);
      MessageBox(0, err.c_str(), "Попытка ввода", MB_ICONSTOP | MB_TASKMODAL | MB_OK);
    }
    else
      new_ports.SetDataIn(StrToHex(grid_ports->Cells[0][i]), c);
  }
  ok = true;

  for(int i = 1; i < grid_irq->RowCount; i ++)
    new_IRQ.SetState(i-1, grid_irq->Cells[1][i] == "активное");
}
unsigned StrToHex(AnsiString str)
{
  unsigned ret = 0;
  for(int i = 1; i <= str.Length(); i ++)
  {
    if((str[i] >= '0') && (str[i] <= '9'))
      ret = ret * 16 + (str[i] - '0');
    else if((str[i] >= 'a') && (str[i] <= 'f'))
      ret = ret * 16 + (str[i] - 'a' + 10);
    else if((str[i] >= 'A') && (str[i] <= 'F'))
      ret = ret * 16 + (str[i] - 'A' + 10);
  }
  return ret;
}
//---------------------------------------------------------------------------

