//---------------------------------------------------------------------------
#ifndef CRAMH
#define CRAMH

#include <Grids.hpp>
#include <Math.hpp>
#include "CVariableWord.h"
#include "CALU.h"
#include "COptions.h"
//---------------------------------------------------------------------------
class CRAM
{
  private:
    vector <CVariableWord> data;
    vector <unsigned> udata_reset;
    vector <unsigned> udata;
    unsigned size;
    unsigned kol_bit_data;
    int ss;
    int ramb_size[2];
    int ramb_kol[2];
    AnsiString ranb_hex[2][100][64];
    int kol_enabled_ramb;
    AnsiString enabled_ramb[1000];

    TStringGrid *grid_debug_RAM;
    void RunRAMBHex();
  public:
    CRAM(): ss(16), grid_debug_RAM(NULL){}
    ~CRAM(){ Clear();}

    void Clear();

    unsigned GetSize() {return size;}

    void AddData(CVariableWord d);

    unsigned GetKolBitData() {return kol_bit_data;}
    unsigned GetKolBitDataWhithZnak() {return GetKolBitData();}
    void SetKolBitData(unsigned k) {kol_bit_data = k;}
    unsigned GetKolBitAddress();
    bool GetData(unsigned address, unsigned &ch);
    unsigned GetData(unsigned address);
    void SetData(unsigned address, unsigned d);
    void Reset();
    AnsiString GetDataName(unsigned address);

    void PrintAll(TStringGrid *sg);
    void PrintValue(int s = 0);

    void Linker();

    void PrintVHDLSignals(TMemo *memo);
    void PrintVHDLSignalsSpartan2(TMemo *memo);
    void PrintVHDLSignalsSpartan3(TMemo *memo);
    void PrintVHDLSignalsSpartan6(TMemo *memo);
    void PrintVHDLProcess(TMemo *memo);
    void PrintVHDLProcessSpartan2(TMemo *memo);
    void PrintVHDLProcessSpartan3(TMemo *memo);
    void PrintVHDLProcessSpartan6(TMemo *memo);
    unsigned GetSizeRAMB(int sh_d);
    unsigned GetShARAMB(int sh_d);
    void AddEnabledRamB(AnsiString str);
    bool IsEnabledRamB(AnsiString str);
    AnsiString GetNumberBin(unsigned ch, int kol_bit);
    AnsiString GetDownto(int begin, int end);
    AnsiString GetDownto2(int begin, int end);
};
extern CRAM RAM;
//---------------------------------------------------------------------------
#endif
