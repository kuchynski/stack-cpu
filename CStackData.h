//---------------------------------------------------------------------------
#ifndef CStackDataH
#define CStackDataH

#include <Grids.hpp>
#include "CALU.h"
#include "CYY.h"
//---------------------------------------------------------------------------
enum CommandStackData {sdNon, sdPop, sdPush};
class CStackData
{
  private:
    unsigned data[100];
    unsigned prev_data0;
    unsigned prev_data1;
    unsigned size;
    unsigned kol_bit_data;
    int ss;

    TStringGrid *grid;
    
    unsigned Norma(unsigned ch);
  public:
    CStackData(): size(8), ss(16){}

    void Clear();

    unsigned GetSize() {return size;}
    void SetSize(unsigned s) {size = s;}
    void SetKolBitData(unsigned k) {kol_bit_data = k;}
    unsigned GetKolBitDataWhithZnak() {return kol_bit_data;}

    unsigned GetData0() {return data[0];}
    unsigned GetData1() {return data[1];}
    unsigned GetPrevData0() {return prev_data0;}
    unsigned GetPrevData1() {return prev_data1;}

    void Run(AnsiString command, unsigned alu, unsigned yy, unsigned r, unsigned bus_data, bool print_state);
    void Reset();

    void PrintAll(TStringGrid *sg);
    void PrintValue(int s = 0);

    void PrintVHDLSignals(TMemo *memo);
    void PrintVHDLProcess(TMemo *memo);
};
extern CStackData stack_data;
//---------------------------------------------------------------------------
//enum CommandStackReturn {srNon, srPop, srPush};
class CStackReturn
{
  private:
    unsigned data[100];
    unsigned prev_data0;
    unsigned size;
    unsigned kol_bit_data;
    unsigned kol_bit_irq;

    TStringGrid *grid;

    unsigned Norma(unsigned ch);
  public:
    CStackReturn(): size(8){}

    void Clear();

    unsigned GetSize() {return size;}
    void SetSize(unsigned s) {size = s;}
    void SetKolBitData(unsigned k) {kol_bit_data = k;}
    void SetKolBitIrq(unsigned k) {kol_bit_irq = k;}
    unsigned GetKolBitData() { return kol_bit_data;}
    unsigned GetKolBitIrq() { return kol_bit_irq;}

    unsigned GetData0() {return data[0];}
    unsigned GetPrevData0() {return prev_data0;}
    unsigned GetDataAddress0();
    unsigned GetPrevDataAddress0();
    unsigned GetDataIrq0();
    unsigned GetPrevDataIrq0();

    void Pop(bool print_state);
    void Push(unsigned d, unsigned i, bool print_state);
    void Reset();

    void PrintAll(TStringGrid *sg);
    void PrintValue();
    
    void PrintVHDLSignals(TMemo *memo);
    void PrintVHDLProcess(TMemo *memo);
    AnsiString GetChNULL(int ch);
};
extern CStackReturn stack_return;
//---------------------------------------------------------------------------
#endif
