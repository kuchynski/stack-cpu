//---------------------------------------------------------------------------
#ifndef CROMH
#define CROMH

#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <Math.hpp>
#include "CFunction.h"
#include "CRAM.h"
//---------------------------------------------------------------------------
class CROM
{
  private:
    vector <CFunction> functions;
    unsigned size;
    unsigned kol_bit_data;
    unsigned k1, k2;
    AnsiString last_command;

    TStringGrid *grid_debug_Text2;
    unsigned cur_address;
    int cur_row;
    int cur_page, cur_line, cur_line_real;

    int ramb_size[2];
    int ramb_kol[2];
    AnsiString ranb_hex[2][100][100];

    void RunRAMBHex();
  public:
    CROM(){}
    ~CROM(){ Clear();}

    void Clear();

    int GetKolFunctions() {return functions.size();}
    int GetKolCommandsForth();
    int GetKolCommands();
    int GetSize();

    void AddFunction(CFunction c);

    unsigned GetKolBitData() {return kol_bit_data;}
    unsigned GetKolBitAddress();
    void SetKolBitData(unsigned k1_, unsigned k2_);
    unsigned GetKolBitKodOp() {return k1;}
    unsigned GetKolBitAddrressOp() {return k2;}
    void PrintAll(TStringGrid *sg2);
    int GetCurRow() const {return cur_row;}
    int GetCurLine() const {return cur_line;}
    int GetCurPage() const {return cur_page;}
    int GetCurLineReal() const {return cur_line_real;}
    int GetAddressInLine(int w, int l);
    unsigned GetAddressFunction(AnsiString n);
    CCommandForth* GetCommandFort(unsigned a);

    bool Run(unsigned address, unsigned &command1, unsigned &command2, bool mes);
    void SetState(unsigned a);

    void PrintVHDLSignals(TMemo *memo);
    void PrintVHDLSignalsSpartan2(TMemo *memo);
    void PrintVHDLSignalsSpartan3(TMemo *memo);
    void PrintVHDLSignalsSpartan6(TMemo *memo);
    void PrintVHDLProcess(TMemo *memo);
    void PrintVHDLProcessSpartan2(TMemo *memo);
    void PrintVHDLProcessSpartan3(TMemo *memo);
    void PrintVHDLProcessSpartan6(TMemo *memo);

    void SetlastCommand(AnsiString lc) {last_command = lc;}
};
extern CROM ROM;
//---------------------------------------------------------------------------
#endif
