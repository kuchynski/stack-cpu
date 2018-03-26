//---------------------------------------------------------------------------
#ifndef CALUH
#define CALUH

#include "CWord.h"
#include "CYY.h"
//---------------------------------------------------------------------------
class CALU
{
  private:
    unsigned kol_bit_data;
    bool znak;
    unsigned data_plus, data_minus, data_mul, data_div, data_not, data_or, data_and, data_xor;
    unsigned data_compare, data_sign, data_min, data_max, data_shr, data_shl;
    unsigned data_1plus, data_1minus, data_2plus, data_2minus;

    vector <CStandartWord> commands;

    unsigned RunPlus(unsigned a, unsigned b);
    unsigned RunMinus(unsigned a, unsigned b);
    unsigned RunMul(unsigned a, unsigned b);
    unsigned RunDiv(unsigned a, unsigned b);
    unsigned RunNot(unsigned a);
    unsigned RunOr(unsigned a, unsigned b);
    unsigned RunAnd(unsigned a, unsigned b);
    unsigned RunXor(unsigned a, unsigned b);
    unsigned RunCompare(AnsiString command, unsigned a, unsigned b);
    unsigned RunMin(unsigned a, unsigned b);
    unsigned RunMax(unsigned a, unsigned b);
    unsigned RunSign(AnsiString command, unsigned a);
    unsigned RunShr(unsigned a);
    unsigned RunShl(unsigned a);
    unsigned Run1Plus(unsigned a);
    unsigned Run1Minus(unsigned a);
    unsigned Run2Plus(unsigned a);
    unsigned Run2Minus(unsigned a);

    AnsiString GetStrCompare();

  public:
    CALU(){}
    ~CALU() {Clear();}

    void Clear() {commands.clear();}
    void SetKolBitData(bool z, unsigned k) {znak = z; kol_bit_data = k;}
    unsigned GetKolBitData() {return kol_bit_data;}
    void SetCommands(vector <CStandartWord> c);
    int GetKolCommands() {return commands.size();}

    unsigned Run(AnsiString command, unsigned a, unsigned b);
    int GetInt(unsigned ch);
    unsigned GetHex(int ch);
    unsigned Norma(unsigned ch);

    void PrintVHDLSignals(TMemo *memo);
    void PrintVHDLProcess(TMemo *memo);
};
extern CALU ALU;
//---------------------------------------------------------------------------
#endif
