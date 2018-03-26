//---------------------------------------------------------------------------
#ifndef URamH
#define URamH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <stdio.h>

#include "CRam.h"
//---------------------------------------------------------------------------
class TRamForm : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TStringGrid *grid_debug_RAM;
        TSpeedButton *ButRAMSS;
        void __fastcall ButRAMSSClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TRamForm(TComponent* Owner);
        void LoadPosition(FILE *f);
        void SavePosition(FILE *f);
        TStringGrid* GetGrid() {return grid_debug_RAM;}
};
//---------------------------------------------------------------------------
extern PACKAGE TRamForm *RamForm;
//---------------------------------------------------------------------------
#endif
