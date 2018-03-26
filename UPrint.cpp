//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UPrint.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TPrintForm *PrintForm;
//--------------------------------------------------------------------- 
__fastcall TPrintForm::TPrintForm(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void __fastcall TPrintForm::ButSetupClick(TObject *Sender)
{
  PrinterSetupDialog1->Execute();
}
//---------------------------------------------------------------------------
void TPrintForm::Set(vector <CEdit> e, TStringGrid *gr)
{
  edits = e;
  grid_RAM = gr;
}
//---------------------------------------------------------------------------
void __fastcall TPrintForm::ButPrint1Click(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------

