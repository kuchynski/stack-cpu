//----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UOptionsProcForm.h"
//----------------------------------------------------------------------------
#pragma link "CSPIN"
#pragma link "CSPIN"
#pragma link "CSPIN"
#pragma resource "*.dfm"
TOptionsProcForm *OptionsProcForm;
//----------------------------------------------------------------------------
__fastcall TOptionsProcForm::TOptionsProcForm(TComponent *Owner)
	: TForm(Owner)
{
}
//----------------------------------------------------------------------------
bool TOptionsProcForm::Run(COptionsProc &op)
{
  ComboBoxChip->ItemIndex = op.GetChip();
  CheckBoxZnak->Checked = op.GetZnakData();
  CSpinEditRazrad->Value = op.GetaRazradData();
  CSpinEditKolRegStackData->Value = op.GetKolRegStackData();
  CSpinEditKolRegStackReturn->Value = op.GetKolRegStackReturn();
  result = false;
  ShowModal();
  if(result)
    op = COptionsProc(ComboBoxChip->ItemIndex, CheckBoxZnak->Checked, CSpinEditRazrad->Value,
                      CSpinEditKolRegStackData->Value, CSpinEditKolRegStackReturn->Value);
  return result;
}
//----------------------------------------------------------------------------
void __fastcall TOptionsProcForm::ButtonOkClick(TObject *Sender)
{
  result = true;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsProcForm::CheckBoxZnakClick(TObject *Sender)
{
//  CSpinEditRazrad->Value = (CheckBoxZnak->Checked)? 32 : 31;
}
//---------------------------------------------------------------------------


