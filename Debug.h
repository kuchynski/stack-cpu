//---------------------------------------------------------------------------
#ifndef DebugH
#define DebugH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "CYY.h"
#include "CEdit.h"
//---------------------------------------------------------------------------
enum MessageFromDebug {fdBadCommand, fdNextCommands, fdOneTakt};
enum KindOfDebug {kdNon, kdToNextComFort, kdToNextWord, kdToCheck, kdToNextLine, kdToNextLineIn};

class Debug : public TThread
{
private:
        TPageControl *PageControl;
        int prev_line, visiable_line;
        int prev_page, visiable_page;
        int if_chek_line, if_chek_page;
        int kol_call;
        int stop_line, stop_page;
        vector <CEdit> edits;
        TStringGrid *StringGrid;

        bool RunCommand();
        bool IsStopLine();
protected:
        void __fastcall Execute();
public:
        __fastcall Debug(bool CreateSuspended, vector <CEdit> e, TStringGrid *sg);
        KindOfDebug kind_of_debug;
        int row_check;
        int cur_line;

        void Stop();
        void PrintState();
        void SetStopLine(int p, int l){stop_page = p; stop_line = l;}
};
//---------------------------------------------------------------------------
#endif
