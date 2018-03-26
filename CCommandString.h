//---------------------------------------------------------------------------
#ifndef CCommandStringH
#define CCommandStringH
#include <vcl.h>
//---------------------------------------------------------------------------
class CCommandString
{
  private:
    AnsiString str;
    int page;
    int line;
  public:
    CCommandString();
    CCommandString(AnsiString new_str, int new_page, int new_line):
      str(new_str), page(new_page), line(new_line) {}
    CCommandString(const CCommandString &ks);
    CCommandString operator=(const CCommandString &ks);
  //  ~CCommandString() {str.clear(); n.clear();}
//    bool operator==(const CCommandString &ks)const  {return (str == ks.str) && (n == ks.n);}
//    bool operator!=(const CCommandString &ks)const  {return !(*this == ks);}
    AnsiString GetStr(void) const {return str;}
    int GetLine(void) const {return line;}
    int GetPage(void) const {return page;}
};
//---------------------------------------------------------------------------
#endif
