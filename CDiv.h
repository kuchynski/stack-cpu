//---------------------------------------------------------------------------
#ifndef CDivH
#define CDivH

#include <vcl.h>
#include <vector>
#include "CUart.h"
#include <Math.hpp>

using namespace std;
//---------------------------------------------------------------------------
class CDiv
{
  private:
    int st;
    vector<AnsiString> name;
    vector<int> del;
    vector<int> real_del;
    vector<int> father;
    vector<bool> exit;

  public:
    CDiv(): st(0){}
    ~CDiv() {Clear();}

    void Clear() {name.clear(); del.clear(); real_del.clear(); exit.clear(); father.clear(); st = 0;}
    void Reset() {st = 0;}
    bool AddDiv(AnsiString n, int k, bool e);
    bool Add4Div(AnsiString n);
    bool IsFrequency(AnsiString f);
    void RunLinker();
    void NewTakt();

    void PrintVHDLPort(TMemo *memo);
    void PrintVHDLSignals(TMemo *memo);
    void PrintVHDLProcess(TMemo *memo);
    int GetSizeSt(int w);
};
extern CDiv div_frequency;
//---------------------------------------------------------------------------
#endif
