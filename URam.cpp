//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "URam.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TRamForm *RamForm;
//---------------------------------------------------------------------------
__fastcall TRamForm::TRamForm(TComponent* Owner)
        : TForm(Owner)
{
  AnsiString str_grid_debug_RAM[3] = {"Адрес", "Имя", "Значение"};
  for(int i = 0; i < grid_debug_RAM->ColCount; i ++)
    grid_debug_RAM->Cells[i][0] = str_grid_debug_RAM[i];//пишем заголовок таблицы
}
//---------------------------------------------------------------------------
void TRamForm::LoadPosition(FILE *f)
{
}
//---------------------------------------------------------------------------
void TRamForm::SavePosition(FILE *f)
{
}
//---------------------------------------------------------------------------
void __fastcall TRamForm::ButRAMSSClick(TObject *Sender)
{
  ButRAMSS->Caption = (ButRAMSS->Caption == "dec")? "hex" : "dec";
  RAM.PrintValue((ButRAMSS->Caption == "dec")? 10 : 16);
}
//---------------------------------------------------------------------------
