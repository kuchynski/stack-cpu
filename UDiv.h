//----------------------------------------------------------------------------
#ifndef UDivH
#define UDivH
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
#include <Grids.hpp>
#include <Buttons.hpp>
#include <Mask.hpp>
#include <math.h>

#include "COptions.h"
#include "CDiv.h"
//----------------------------------------------------------------------------
class TDivForm : public TForm
{
__published:
        TButton *ButOk;
        TButton *ButCancel;
        TPanel *Panel1;
        TStringGrid *StringGridDiv;
        TBitBtn *ButAdd;
        TBitBtn *ButSubPort;
        TButton *ButRun;
        TMaskEdit *EditInputClk;
        TLabel *Label1;
        TLabel *Label2;
        void __fastcall ButAddClick(TObject *Sender);
        void __fastcall ButSubPortClick(TObject *Sender);
        void __fastcall StringGridDivGetEditMask(TObject *Sender, int ACol,
          int ARow, AnsiString &Value);
        void __fastcall ButRunClick(TObject *Sender);
        void __fastcall StringGridDivSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
        void __fastcall ButOkClick(TObject *Sender);
        void __fastcall EditInputClkChange(TObject *Sender);
private:
        bool result;
        int input_div, new_input_div;
        AnsiString str_input_div;
public:
	virtual __fastcall TDivForm(TComponent *Owner);
        bool Run(CTextOfGrid &op);
        AnsiString GetInputDiv() {return EditInputClk->Text;}
        void SetInputDiv(AnsiString str) {EditInputClk->Text = str; str_input_div = str;}
        TStringGrid* GetStringGridPorts() {return StringGridDiv;}
        bool Linker();
        int GetInputDivInt() {return input_div;}
};
//----------------------------------------------------------------------------
extern TDivForm *DivForm;
//----------------------------------------------------------------------------
#endif
