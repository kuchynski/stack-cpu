//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AboutForm1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AboutForm"
#pragma resource "*.dfm"
TAboutBox1 *AboutBox1;
//---------------------------------------------------------------------------
__fastcall TAboutBox1::TAboutBox1(TComponent* Owner)
        : TAboutBox(Owner)
{
}
//---------------------------------------------------------------------------
