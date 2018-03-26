//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AboutForm.h"
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TAboutBox *AboutBox;
//--------------------------------------------------------------------- 
__fastcall TAboutBox::TAboutBox(TComponent* AOwner)
	: TForm(AOwner)
{
  union abcd
  {
    DWORD dw;
    WORD w[2];
    BYTE b[4];
  }uabcd;
  DWORD abcd = 0x000101FF;
  uabcd.dw = abcd;
  uabcd.b[2] += uabcd.b[0];
  uabcd.w[1] = uabcd.w[0] - uabcd.b[2];
  //  uabcd.w[1] = uabcd.w[0] - static_cast<BYTE>(uabcd.b[2] + uabcd.b[0]);
  abcd = uabcd.dw;
}
//---------------------------------------------------------------------
