//----------------------------------------------------------------------------
#ifndef UInputsFormH
#define UInputsFormH
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

#include "CPorts.h"
#include "CDictionary.h"
#include "CIRQ.h"
//----------------------------------------------------------------------------
class TInputForm : public TForm
{
__published:
        TButton *ButOk;
        TButton *ButCancel;
        TPanel *Panel1;
        TGroupBox *GroupBox1;
        TGroupBox *GroupBox2;
        TStringGrid *grid_ports;
        TStringGrid *grid_irq;
        void __fastcall grid_portsSelectCell(TObject *Sender,
          int ACol, int ARow, bool &CanSelect);
        void __fastcall ButOkClick(TObject *Sender);
        void __fastcall grid_irqSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
private:
        CPorts new_ports;
        CIRQ new_IRQ;
        bool ok;
public:
	virtual __fastcall TInputForm(TComponent *Owner);

        void Run();
};
//----------------------------------------------------------------------------
extern TInputForm *InputForm;
//----------------------------------------------------------------------------
#endif
