//---------------------------------------------------------------------------

#ifndef AboutForm1H
#define AboutForm1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AboutForm.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TAboutBox1 : public TAboutBox
{
__published:	// IDE-managed Components
        TMemo *Memo1;
private:	// User declarations
public:		// User declarations
        __fastcall TAboutBox1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAboutBox1 *AboutBox1;
//---------------------------------------------------------------------------
#endif
