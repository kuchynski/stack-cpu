//----------------------------------------------------------------------------
#ifndef UOptionsProcFormH
#define UOptionsProcFormH
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
#include <Grids.hpp>

#include "COptions.h"
#include "CSPIN.h"
//----------------------------------------------------------------------------
class TOptionsProcForm : public TForm
{
__published:
        TButton *ButtonOk;
        TButton *ButtonCancel;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TTabSheet *TabSheet3;
        TLabel *Label2;
        TCSpinEdit *CSpinEditRazrad;
        TCheckBox *CheckBoxZnak;
        TCSpinEdit *CSpinEditKolRegStackData;
        TLabel *Label3;
        TCSpinEdit *CSpinEditKolRegStackReturn;
        TLabel *Label1;
        TLabel *Label4;
        TLabel *Label5;
        TComboBox *ComboBoxChip;
        void __fastcall ButtonOkClick(TObject *Sender);
        void __fastcall CheckBoxZnakClick(TObject *Sender);
private:
        bool result;
public:
	virtual __fastcall TOptionsProcForm(TComponent *Owner);
        bool Run(COptionsProc &op);
};
//----------------------------------------------------------------------------
extern TOptionsProcForm *OptionsProcForm;
//----------------------------------------------------------------------------
#endif
