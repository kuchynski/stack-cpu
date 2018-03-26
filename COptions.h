//---------------------------------------------------------------------------
#ifndef COptionsH
#define COptionsH

#include "CString.h"
#include <Grids.hpp>
#include <vector>

using namespace std;
//----------------------------------------------------------------------------
class COptionsProc
{
    bool znak;
    long chip, razrad, kol_reg_stackdata, kol_reg_stackreturn;
    long zapas[1000];
  public:
    COptionsProc();
    COptionsProc(long c, bool z, long r, long krsd, long krsr);
    COptionsProc(FILE *f);
    COptionsProc(const COptionsProc &op);
    COptionsProc operator=(const COptionsProc &op);

    bool SaveToFile(FILE *f)const;
    bool GetZnakData()const {return znak;}
    long GetaRazradData()const {return razrad;}
    long GetKolRegStackData()const {return kol_reg_stackdata;}
    long GetKolRegStackReturn()const {return kol_reg_stackreturn;}
    long GetChip()const {return chip;}
};
extern COptionsProc options_proc;
//----------------------------------------------------------------------------
#define max
class CTextOfGrid
{
    struct SStroka
    {
      CString str[10];
    };
    int size;
    AnsiString name;
    vector <SStroka>stroki;
  public:
    CTextOfGrid(AnsiString str, int s): size(s), name(str){};
    CTextOfGrid(AnsiString str, int s, FILE *f);
    CTextOfGrid(AnsiString str, TStringGrid *sg, bool fixed_row = true);
    CTextOfGrid(const CTextOfGrid &op);
    CTextOfGrid operator=(const CTextOfGrid &op);
    ~CTextOfGrid(){stroki.erase(stroki.begin(), stroki.end());}
    bool SaveToFile(FILE *f);
    void PaintOnStringGrid(TStringGrid *sg, bool fixed_row = true);
    vector <SStroka> GetStr() {return stroki;}
    AnsiString GetPosition(int x, int y);
    int GetSize()const {return size;}
};
//---------------------------------------------------------------------------
#endif
