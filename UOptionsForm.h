//----------------------------------------------------------------------------
#ifndef UOptionsFormH
#define UOptionsFormH
//----------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Windows.hpp>
#include <Messages.hpp>
#include <Classes.hpp>
#include <Graphics.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "CSPIN.h"
#include <Grids.hpp>
//----------------------------------------------------------------------------
class TOptionsForm : public TForm
{
__published:
	TButton *Button1;
	TButton *Button2;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TTabSheet *TabSheet3;
        TCSpinEdit *CSpinEdit2;
        TLabel *Label1;
        TTabSheet *TabSheet4;
        TCSpinEdit *CSpinEdit3;
        TLabel *Label3;
        TRadioButton *RadioButton1;
        TRadioButton *RadioButton2;
        TLabel *Label2;
        TCSpinEdit *CSpinEdit1;
        TCheckBox *CheckBox1;
private:
       // CParam param;
public:
	virtual __fastcall TOptionsForm(TComponent *Owner);
        void Run();
};
//----------------------------------------------------------------------------
extern TOptionsForm *OptionsForm;
//----------------------------------------------------------------------------
#endif
