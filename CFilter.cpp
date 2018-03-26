//---------------------------------------------------------------------------
#pragma hdrstop
#include "CFilter.h"
#pragma package(smart_init)

CFilters filters;
//---------------------------------------------------------------------------
SFilter::SFilter(): mantisa(8), por(8), kol_coof(2),// name("filter"), frequency("clk"),
                    center(1), begin(0)
{}
//---------------------------------------------------------------------------
SFilter::SFilter(AnsiString n, AnsiString f, long m, long p, int kc,
                 double *ca, double *cb, int fd_, int *fr_, int *a_, int sm_):
            mantisa(m), por(p), kol_coof(kc), //name(n), frequency(f), 
            fd(fd_), sm(sm_), center(1), begin(0)
{
  for(int i = 0; i < 3; i++)
  {
    fr[i] = fr_[i];
    a[i] = a_[i];
  }
  for(int i = 0; i < kc; i++)
  {
    coof_a[i] = ca[i];
    coof_a[i] = cb[i];
  }
}
//---------------------------------------------------------------------------
AnsiString SFilter::GetCoofA()
{
  AnsiString ret = "";
  for(int i = 0; i < kol_coof;i ++)
    ret += (FloatToStr(coof_a[i]) + " ");
  return ret;
}
//---------------------------------------------------------------------------
AnsiString SFilter::GetCoofB()
{
  AnsiString ret = "";
  for(int i = 0; i < kol_coof;i ++)
    ret += (FloatToStr(coof_b[i]) + " ");
  return ret;
}
//---------------------------------------------------------------------------
CFilters::~CFilters()
{
   Clear();
}
//---------------------------------------------------------------------------
void CFilters::Clear()
{
  modules.clear();
}
//---------------------------------------------------------------------------
bool CFilters::LoadFromFile(FILE *f)
{
  modules.clear();
  int size;
  if(fread(&size, sizeof(size), 1, f) != 1)
    return false;
  SFilter sf;
  for(int i = 0; i < size; i++)
  {
    if(fread(&sf, sizeof(sf), 1, f) != 1)
      return false;
    modules.push_back(sf);
  }
}
//---------------------------------------------------------------------------
bool CFilters::SaveToFile(FILE *f)
{
  int size = modules.size();
  if(fwrite(&size, sizeof(size), 1, f) != 1)
    return false;
  for(vector <SFilter>:: iterator p = modules.begin(); p < modules.end(); p ++)
  {
    SFilter sf = *p;
    if(fwrite(&sf, sizeof(sf), 1, f) != 1)
      return false;
  }
}
//---------------------------------------------------------------------------
bool CFilters::Sub(int n)
{
  if((n >= modules.size()) || (n < 0))
    return false;
  vector <SFilter>:: iterator p = modules.begin() + n;
  modules.erase(p);
  return true;
}
//---------------------------------------------------------------------------
bool CFilters::Add(int n, SFilter &f)
{
  if((n > modules.size()) || (n < 0))
    return false;
  vector <SFilter>:: iterator p = modules.begin() + n;
  int s1= modules.size();
  modules.insert(p, f);
  int s2= modules.size();
}
//---------------------------------------------------------------------------
bool CFilters::Set(int n, SFilter &f)
{
  if((n >= modules.size()) || (n < 0))
    return false;
  vector <SFilter>:: iterator p = modules.begin() + n;
  Sub(n);
  Add(n, f);
}
//---------------------------------------------------------------------------
SFilter CFilters::GetFilter(int n)
{
  if((n >= modules.size()) || (n < 0))
    return SFilter();
  vector <SFilter>:: iterator p = modules.begin() + n;
  return *p;
}
//---------------------------------------------------------------------------
void CFilters::RunAll()
{
  for(int i = 0; i < modules.size(); i ++)
    Run(i);
}
//---------------------------------------------------------------------------
void CFilters::Run(int n)
{
  int s= modules.size();
  if((n >= modules.size()) || (n < 0))
    return;
  vector <SFilter>:: iterator p = modules.begin() + n;
  for(int i = 0; i < 10000; i ++)
  {
    p->data_in[i] =
    (int)((double)p->sm + (double)p->a[0] * sin(2.0*M_PI*(double)p->fr[0]*(double)i/(double)p->fd)) +
    (int)((double)p->a[1] * sin(2.0*M_PI*(double)p->fr[1]*(double)i/(double)p->fd)) +
    (int)((double)p->a[2] * sin(2.0*M_PI*(double)p->fr[2]*(double)i/(double)p->fd));
  }
  double y[100], x[100];
  for(int i = 0; i < p->kol_coof; i ++)
  {
    y[i] = 0;
    x[i] = 0;
  }
  for(int j = 0; j < 10000; j ++)
  {
    x[p->kol_coof] = p->data_in[j];
    y[p->kol_coof] = 0;
    for(int i = 1; i < p->kol_coof; i ++)
    {
      y[p->kol_coof] = y[p->kol_coof] + p->coof_b[i] * x[p->kol_coof-i];
      y[p->kol_coof] = y[p->kol_coof] - p->coof_a[i] * y[p->kol_coof-i];
    }
    y[p->kol_coof] = y[p->kol_coof] + p->coof_b[0] * x[p->kol_coof];
    for(int i = 1; i < p->kol_coof; i ++)
    { x[i] = x[i+1]; y[i] = y[i+1]; }
    p->data_midle[j] = y[p->kol_coof];
  }
  CDataFilter xd[100], yd[100];
  for(int i = 0; i < p->kol_coof; i ++)
  {
    xd[i].SetMP(p->mantisa, p->por);
    yd[i] = 0.0;
    xd[i] = 0.0;
    p->coof_bd[i] = (double)p->coof_b[i];
    p->coof_ad[i] = (double)-p->coof_a[i];
  }
  for(int j = 0; j < 10000; j ++)
  {
    xd[p->kol_coof] = p->data_in[j];
//    xd[kol_coof] = -10000;
    yd[p->kol_coof] = 0;
    for(int i = 1; i < p->kol_coof; i ++)
    {
      yd[p->kol_coof] = yd[p->kol_coof] + p->coof_bd[i] * xd[p->kol_coof-i];
      yd[p->kol_coof] = yd[p->kol_coof] + p->coof_ad[i] * yd[p->kol_coof-i];
    }
    yd[p->kol_coof] = yd[p->kol_coof] + p->coof_bd[0] * xd[p->kol_coof];
    for(int i = 1; i < p->kol_coof; i ++)
    { xd[i] = xd[i+1]; yd[i] = yd[i+1]; }
    p->data_out[j] = yd[p->kol_coof].GetInt();
  }
}
//---------------------------------------------------------------------------
void CFilters::PrintVHDLSignals(TMemo *memo)
{
  RunAll();
  for(vector <SFilter>:: iterator p = modules.begin(); p < modules.end(); p ++)
  {
    long mantisa = p->mantisa, por = p->por;
    int kol_coof = p->kol_coof;
    AnsiString str = "filter"+AnsiString(p->name);
  //  memo->Lines->Add("   signal "+str+"_in, "+str+"_out : std_logic_vector"+DownTo(mantisa-1,0)+";");

    memo->Lines->Add("   signal "+str+"inm, "+str+"outm : std_logic_vector"+DownTo(mantisa-2,0)+";");
    memo->Lines->Add("   signal "+str+"inp : std_logic_vector"+DownTo(por-1,0)+";");
    memo->Lines->Add("   signal "+str+"inz, "+str+"outz : std_logic;");
  //ADD
    memo->Lines->Add("   signal "+str+"addm1, "+str+"addm2, "+str+"addm3, "+str+"addm4, "+str+"addm5, "+str+"addm7, "+str+"addm : std_logic_vector"+DownTo(mantisa-2,0)+";");
    memo->Lines->Add("   signal "+str+"addm6, "+str+"addm61, "+str+"addm62, "+str+"addm8 : std_logic_vector"+DownTo(mantisa-1,0)+";");
    memo->Lines->Add("   signal "+str+"addp1, "+str+"addp2, "+str+"addp3, "+str+"addp : std_logic_vector"+DownTo(por-1,0)+";");
    memo->Lines->Add("   signal "+str+"addpbm, "+str+"addz1, "+str+"addz2, "+str+"addz3, "+str+"addz4, "+str+"addz : std_logic;");
  //MUL
    memo->Lines->Add("   signal "+str+"mulm1 : std_logic_vector"+DownTo(2*(mantisa-1)-1,0)+";");
    memo->Lines->Add("   signal "+str+"mulmi1, "+str+"mulmi2, "+str+"mulm2, "+str+"mulm : std_logic_vector"+DownTo(mantisa-2,0)+";");
    memo->Lines->Add("   signal "+str+"mulpi1, "+str+"mulpi2, "+str+"mulp1, "+str+"mulp2, "+str+"mulp : std_logic_vector"+DownTo(por-1,0)+";");
    memo->Lines->Add("   signal "+str+"mulzi1, "+str+"mulzi2, "+str+"mulz : std_logic;");
  //AB REG
    for(int i = 1; i <= kol_coof; i ++)
    {
      memo->Lines->Add("   constant "+str+"am"+IntToStr(i)+" : std_logic_vector"+DownTo(mantisa-2,0)+" := "+p->coof_ad[i-1].GetMon()+";");
      memo->Lines->Add("   constant "+str+"az"+IntToStr(i)+" : std_logic := "+p->coof_ad[i-1].GetZnak()+";");
      memo->Lines->Add("   constant "+str+"ap"+IntToStr(i)+" : std_logic_vector"+DownTo(por-1,0)+" := "+p->coof_ad[i-1].GetPor()+";");
      memo->Lines->Add("   constant "+str+"bm"+IntToStr(i)+" : std_logic_vector"+DownTo(p->mantisa-2,0)+" := "+p->coof_bd[i-1].GetMon()+";");
      memo->Lines->Add("   constant "+str+"bz"+IntToStr(i)+" : std_logic := "+p->coof_bd[i-1].GetZnak()+";");
      memo->Lines->Add("   constant "+str+"bp"+IntToStr(i)+" : std_logic_vector"+DownTo(por-1,0)+" := "+p->coof_bd[i-1].GetPor()+";");
      memo->Lines->Add("   signal "+str+"xm"+IntToStr(i)+", "+str+"ym"+IntToStr(i)+" : std_logic_vector"+DownTo(mantisa-2,0)+";");
      memo->Lines->Add("   signal "+str+"xz"+IntToStr(i)+", "+str+"yz"+IntToStr(i)+" : std_logic;");
      memo->Lines->Add("   signal "+str+"xp"+IntToStr(i)+", "+str+"yp"+IntToStr(i)+" : std_logic_vector"+DownTo(por-1,0)+";");
    }
  //YY
    memo->Lines->Add("   signal "+str+"beg, "+str+"work, "+str+"work_end : std_logic;");
    int kol_bit_styy;
    switch(kol_coof)
    {
      case 1:kol_bit_styy = 1;break;
      case 2:kol_bit_styy = 2;break;
      case 3:case 4:kol_bit_styy = 3;break;
      case 5:case 6:case 7:case 8:kol_bit_styy = 4;break;
      case 9:case 10:case 11:case 12:case 13:case 14:case 15:case 16:kol_bit_styy = 5;break;
      default:kol_bit_styy = 6;break;
    }
    memo->Lines->Add("   signal "+str+"st : std_logic_vector"+DownTo(kol_bit_styy-1,0)+";");
  }
}
//---------------------------------------------------------------------------
void CFilters::PrintVHDLProcess(TMemo *memo)
{
  for(vector <SFilter>:: iterator p = modules.begin(); p < modules.end(); p ++)
  {
    int kol_bit_styy;
    long mantisa = p->mantisa, por = p->por;
    int kol_coof = p->kol_coof;
    AnsiString frequency = "sc_clk_" + AnsiString(p->frequency);
    switch(p->kol_coof)
    {
      case 1:kol_bit_styy = 1;break;
      case 2:kol_bit_styy = 2;break;
      case 3:case 4:kol_bit_styy = 3;break;
      case 5:case 6:case 7:case 8:kol_bit_styy = 4;break;
      case 9:case 10:case 11:case 12:case 13:case 14:case 15:case 16:kol_bit_styy = 5;break;
      default:kol_bit_styy = 6;break;
    }
    AnsiString st = "filter"+AnsiString(p->name);
    AnsiString name = AnsiString(p->name);
    memo->Lines->Add("--Filters"+st); //begin________________________________________________________________

    if(options_proc.GetZnakData())
    {
      memo->Lines->Add("  "+st+"inz <= "+name+"_out("+IntToStr(options_proc.GetaRazradData()-1)+");");
      if(p->mantisa <= options_proc.GetaRazradData())
        memo->Lines->Add("  with "+st+"inz select "+st+"inm <= "+name+"_out"+DownTo(mantisa-2,0)+" when '0'"+
                         ", ((not "+name+"_out"+DownTo(mantisa-2,0)+")+"+GetNumberBin(1, mantisa-1)+") when others;");
      else
        memo->Lines->Add("  with "+st+"inz select "+st+"inm <= "+GetNumberBin(0, mantisa-options_proc.GetaRazradData())+" & "+name+"_out"+DownTo(options_proc.GetaRazradData()-2,0)+" when '0'"+
                         ", (("+GetNumberBin(0, mantisa-options_proc.GetaRazradData())+" & (not "+name+"_out"+DownTo(options_proc.GetaRazradData()-2,0)+"))+"+GetNumberBin(1, options_proc.GetaRazradData()-1)+") when others;");
    }
    else
    {
      return;
      memo->Lines->Add("  "+st+"inz <= '0';");
      if(p->mantisa <= options_proc.GetaRazradData())
        memo->Lines->Add("  "+st+"inm <= "+name+"_out"+DownTo(mantisa-2,0)+";");
      else
        memo->Lines->Add("  "+st+"inm <= "+GetNumberBin(0, mantisa-options_proc.GetaRazradData())+" & "+name+"_out;");
    }
    memo->Lines->Add("  with "+name+"_out select "+st+"inp <= (others=>'0') when "+GetNumberBin(0, options_proc.GetaRazradData())+
                     ", "+GetNumberBin(mantisa-1, por)+" when others;");
    AnsiString str = "    ";

    memo->Lines->Add("  with "+st+"yp"+IntToStr(kol_coof)+" select "+st+"outm <= "+st+"ym"+IntToStr(kol_coof)+" when "+GetNumberBin(mantisa-1, por)+", ");
    for(int i = 1; i < (mantisa-1); i++)
      str = str+GetNumberBin(0,i)+"&"+""+st+"ym"+IntToStr(kol_coof)+DownTo(mantisa-2,i)+" when "+GetNumberBin((mantisa-1)-i, por)+", ";
    memo->Lines->Add(str+GetNumberBin(0,mantisa-1)+" when others;");
    memo->Lines->Add("  with "+st+"outm select "+st+"outz <= '0' when "+GetNumberBin(0, mantisa-1)+", "+st+"yz"+IntToStr(kol_coof)+" when others;");

    if(p->mantisa < options_proc.GetaRazradData())
      memo->Lines->Add("  with "+st+"outz select "+name+"_in"+DownTo(options_proc.GetaRazradData()-2,0)+" <= "+GetNumberBin(0, options_proc.GetaRazradData()-mantisa)+" & "+st+"outm when '0', "+
                       "((not ("+GetNumberBin(0, options_proc.GetaRazradData()-mantisa)+" & "+st+"outm))+ "+GetNumberBin(1, mantisa-1)+") when others;");
    else
      memo->Lines->Add("  with "+st+"outz select "+name+"_in"+DownTo(options_proc.GetaRazradData()-2,0)+" <= "+st+"outm"+DownTo(options_proc.GetaRazradData()-2,0)+" when '0', "+
                       "((not "+st+"outm"+DownTo(options_proc.GetaRazradData()-2,0)+")+ "+GetNumberBin(1, options_proc.GetaRazradData()-2)+") when others;");
    memo->Lines->Add("  "+name+"_in("+IntToStr(options_proc.GetaRazradData()-1)+") <= "+st+"outz;");

  //ADD
    memo->Lines->Add("  "+st+"addpbm <= '1' when ((not "+st+"mulp("+IntToStr(por-1)+")&"+st+"mulp) > (not "+st+"yp"+IntToStr(kol_coof)+"("+IntToStr(por-1)+") & "+st+"yp"+IntToStr(kol_coof)+")) else '0';");
    memo->Lines->Add("p_addin"+st+": process ("+st+"addpbm, "+st+"mulm, "+st+"mulp, "+st+"mulz, "+st+"ym"+IntToStr(kol_coof)+", "+st+"yp"+IntToStr(kol_coof)+", "+st+"yz"+IntToStr(kol_coof)+") begin");
    memo->Lines->Add("  if "+st+"addpbm = '1' then "+st+"addm1 <= "+st+"mulm; "+st+"addp1 <= "+st+"mulp; "+st+"addz1 <= "+st+"mulz;");
    memo->Lines->Add("    "+st+"addm2 <= "+st+"ym"+IntToStr(kol_coof)+"; "+st+"addp2 <= "+st+"yp"+IntToStr(kol_coof)+"; "+st+"addz2 <= "+st+"yz"+IntToStr(kol_coof)+";");
    memo->Lines->Add("  else "+st+"addm2 <= "+st+"mulm; "+st+"addp2 <= "+st+"mulp; "+st+"addz2 <= "+st+"mulz;");
    memo->Lines->Add("    "+st+"addm1 <= "+st+"ym"+IntToStr(kol_coof)+"; "+st+"addp1 <= "+st+"yp"+IntToStr(kol_coof)+"; "+st+"addz1 <= "+st+"yz"+IntToStr(kol_coof)+";");
    memo->Lines->Add("  end if;");
    memo->Lines->Add("end process;");
    memo->Lines->Add("  "+st+"addp3 <= "+st+"addp1 - "+st+"addp2;");
    memo->Lines->Add("p_add1"+st+": process ("+st+"addp3, "+st+"addm2) begin");
    memo->Lines->Add("  case "+st+"addp3 is");
    for(int i = (mantisa-2); i > 0; i--)
      memo->Lines->Add("    when "+GetNumberBin(i, por)+" => "+st+"addm3 <= "+GetNumberBin(0,i)+" & "+st+"addm2"+DownTo(mantisa-2, i)+";");
    memo->Lines->Add("    when "+GetNumberBin(0, por)+" => "+st+"addm3 <= "+st+"addm2;");
    memo->Lines->Add("    when others => "+st+"addm3 <= (others => '0');");
    memo->Lines->Add("    end case;");
    memo->Lines->Add("end process;");
    memo->Lines->Add("  "+st+"addz3 <= '0' when "+st+"addm3 = "+GetNumberBin(0, mantisa-1)+" else "+st+"addz2;");
    memo->Lines->Add("p_add2"+st+": process ("+st+"addm1, "+st+"addm3, "+st+"addz1, "+st+"addz3) begin");
    memo->Lines->Add("  if "+st+"addm1 > "+st+"addm3 then "+st+"addm4 <= "+st+"addm1; "+st+"addm5 <= "+st+"addm3; "+st+"addz4 <= "+st+"addz1;");
    memo->Lines->Add("  else "+st+"addm4 <= "+st+"addm3; "+st+"addm5 <= "+st+"addm1; "+st+"addz4 <= "+st+"addz3; end if;");
    memo->Lines->Add("end process;");
    memo->Lines->Add("  "+st+"addm61 <= '0'&"+st+"addm4;");
    memo->Lines->Add("  "+st+"addm62 <= '0'&"+st+"addm5;");
    memo->Lines->Add("  "+st+"addm6 <= "+st+"addm61 + "+st+"addm62;");
    memo->Lines->Add("  "+st+"addm7 <= "+st+"addm4 - "+st+"addm5;");
    memo->Lines->Add("  "+st+"addm8 <= "+st+"addm6 when "+st+"addz1 = "+st+"addz3 else '0' & "+st+"addm7;");

    memo->Lines->Add("p_add"+st+": process ("+st+"addm8, "+st+"addz4,"+st+"addp1) begin");
    memo->Lines->Add("  if "+st+"addm8 = "+GetNumberBin(0,mantisa-1)+" then");
    memo->Lines->Add("    "+st+"addz <= '0';");
    memo->Lines->Add("    "+st+"addm <= (others => '0');");
    memo->Lines->Add("    "+st+"addp <= (others => '0');");
    memo->Lines->Add("  else");
    memo->Lines->Add("    "+st+"addz <= "+st+"addz4;");
    memo->Lines->Add("    if "+st+"addm8("+IntToStr(mantisa-1)+") = '1' then "+st+"addm <= "+st+"addm8"+DownTo(mantisa-1,1)+"; "+st+"addp <= "+st+"addp1 + "+GetNumberBin(1,por)+";");
    memo->Lines->Add("    else if "+st+"addm8("+IntToStr(mantisa-2)+") = '1' then "+st+"addm <= "+st+"addm8"+DownTo(mantisa-2,0)+"; "+st+"addp <= "+st+"addp1;");
    str = "end if; end if; ";
    for(int i = mantisa-3; i > 0; i--, str = str + "end if; ")
      memo->Lines->Add("    else if "+st+"addm8("+IntToStr(i)+") = '1' then "+st+"addm <= "+st+"addm8"+DownTo(i, 0)+"&"+GetNumberBin(0,mantisa-2-i)+"; "+st+"addp <= "+st+"addp1 - "+GetNumberBin(mantisa-2-i,por)+";");
    memo->Lines->Add("    else "+st+"addm <= "+st+"addm8(0)&"+GetNumberBin(0,mantisa-2)+"; "+st+"addp <= "+st+"addp1 - "+GetNumberBin(mantisa-2,por)+";");
    memo->Lines->Add("    " + str);
    memo->Lines->Add("  end if;");
    memo->Lines->Add("end process;");
  //MUL
    str = "";
    for(int i = 1; i <= kol_coof; i ++)
      str = str +st+"xm"+IntToStr(i)+", "+st+"xp"+IntToStr(i)+", "+st+"xz"+IntToStr(i)+", "+st+"ym"+IntToStr(i)+", "+st+"yp"+IntToStr(i)+", "+st+"yz"+IntToStr(i)+", ";
    memo->Lines->Add("p_mulin"+st+": process ("+str+""+st+"st) begin");
    memo->Lines->Add("  case "+st+"st is");
    for(int i = 1; i < kol_coof; i ++)
    {
      memo->Lines->Add("    when "+GetNumberBin(2*i-1, kol_bit_styy)+" => "+st+"mulmi1 <= "+st+"bm"+IntToStr(kol_coof-i+1)+"; "+st+"mulpi1 <="+st+"bp"+IntToStr(kol_coof-i+1)+"; "+st+"mulzi1 <= "+st+"bz"+IntToStr(kol_coof-i+1)+
                       "; "+st+"mulmi2 <= "+st+"xm"+IntToStr(i)+"; "+st+"mulpi2 <= "+st+"xp"+IntToStr(i)+"; "+st+"mulzi2 <= "+st+"xz"+IntToStr(i)+";");
      memo->Lines->Add("    when "+GetNumberBin(2*i, kol_bit_styy)+" => "+st+"mulmi1 <= "+st+"am"+IntToStr(kol_coof-i+1)+"; "+st+"mulpi1 <= "+st+"ap"+IntToStr(kol_coof-i+1)+"; "+st+"mulzi1 <= "+st+"az"+IntToStr(kol_coof-i+1)+
                       "; "+st+"mulmi2 <= "+st+"ym"+IntToStr(i)+"; "+st+"mulpi2 <= "+st+"yp"+IntToStr(i)+"; "+st+"mulzi2 <= "+st+"yz"+IntToStr(i)+";");
    }
    memo->Lines->Add("    when others => "+st+"mulmi1 <= "+st+"bm"+IntToStr(1)+"; "+st+"mulpi1 <= "+st+"bp"+IntToStr(1)+"; "+st+"mulzi1 <= "+st+"bz"+IntToStr(1)+
                     "; "+st+"mulmi2 <= "+st+"xm"+IntToStr(kol_coof)+"; "+st+"mulpi2 <= "+st+"xp"+IntToStr(kol_coof)+"; "+st+"mulzi2 <= "+st+"xz"+IntToStr(kol_coof)+";");
    memo->Lines->Add("    end case;");
    memo->Lines->Add("end process;");
    memo->Lines->Add("  "+st+"mulm1 <= "+st+"mulmi1 * "+st+"mulmi2;");
    memo->Lines->Add("  "+st+"mulp1 <= "+st+"mulpi1 + "+st+"mulpi2;");
    str = "";
    memo->Lines->Add("p_mul1"+st+": process ("+st+"mulm1, "+st+"mulm2, "+st+"mulp2) begin");
    for(int i = 2*(mantisa-1)-1; i >= mantisa-1; i--, str = str + "end if; ")
    {
      if(i == 2*(mantisa-1)-1)
        memo->Lines->Add("  if "+st+"mulm1("+IntToStr(i)+") = '1' then "+st+"mulm2 <= "+st+"mulm1"+DownTo(i, i-mantisa+2)+"; "+st+"mulp2 <= "+GetNumberBin(2*(mantisa-1)-1-i,por)+";");
      else
        memo->Lines->Add("  else if "+st+"mulm1("+IntToStr(i)+") = '1' then "+st+"mulm2 <= "+st+"mulm1"+DownTo(i, i-mantisa+2)+"; "+st+"mulp2 <= "+GetNumberBin(2*(mantisa-1)-1-i,por)+";");
    }
    memo->Lines->Add("  else "+st+"mulm2 <= "+st+"mulm1"+DownTo(mantisa-2, 0)+"; "+st+"mulp2 <= "+GetNumberBin(mantisa-1,por)+";");
    memo->Lines->Add("  " + str);
    memo->Lines->Add("end process;");
    memo->Lines->Add("p_mul"+st+": process ("+st+"mulm2, "+st+"mulzi1, "+st+"mulzi2,"+st+"mulp1, "+st+"mulp2) begin");
    memo->Lines->Add("  if "+st+"mulm2 = "+GetNumberBin(0,(mantisa-1))+" then");
    memo->Lines->Add("    "+st+"mulz <= '0';");
    memo->Lines->Add("    "+st+"mulm <= (others => '0');");
    memo->Lines->Add("    "+st+"mulp <= (others => '0');");
    memo->Lines->Add("  else");
    memo->Lines->Add("    "+st+"mulz <= "+st+"mulzi1 xor "+st+"mulzi2;");
    memo->Lines->Add("    "+st+"mulm <= "+st+"mulm2;");
    memo->Lines->Add("    "+st+"mulp <= "+st+"mulp1 - "+st+"mulp2;");
    memo->Lines->Add("  end if;");
    memo->Lines->Add("end process;");
  //REG
    for(int i = 1; i < kol_coof; i ++)
    {
      memo->Lines->Add("p_regx"+st+IntToStr(i)+": process ("+frequency+", sc_rst) begin");
      memo->Lines->Add("  if sc_rst = '1' then "+st+"xm"+IntToStr(i)+" <= (others=>'0'); "+st+"xp"+IntToStr(i)+" <= (others=>'0'); "+st+"xz"+IntToStr(i)+" <= '0';");
      memo->Lines->Add("  else if "+frequency+" = '1' and "+frequency+"'event then if "+st+"work='1' and "+st+"st="+GetNumberBin(2*i-1, kol_bit_styy)+" then");
      memo->Lines->Add("    "+st+"xm"+IntToStr(i)+" <= "+st+"xm"+IntToStr(i+1)+"; "+st+"xp"+IntToStr(i)+" <= "+st+"xp"+IntToStr(i+1)+"; "+st+"xz"+IntToStr(i)+" <= "+st+"xz"+IntToStr(i+1)+";");
      memo->Lines->Add("  end if; end if; end if;");
      memo->Lines->Add("end process;");

      memo->Lines->Add("p_regy"+st+IntToStr(i)+": process ("+frequency+", sc_rst) begin");
      memo->Lines->Add("  if sc_rst = '1' then "+st+"ym"+IntToStr(i)+" <= (others=>'0'); "+st+"yp"+IntToStr(i)+" <= (others=>'0'); "+st+"yz"+IntToStr(i)+" <= '0';");
      if(i == (kol_coof-1))
      {
        memo->Lines->Add("  else if "+frequency+" = '1' and "+frequency+"'event then if "+st+"work='1' and "+st+"st="+GetNumberBin(0, kol_bit_styy)+" then");
        memo->Lines->Add("    "+st+"ym"+IntToStr(i)+" <= "+st+"ym"+IntToStr(i+1)+"; "+st+"yp"+IntToStr(i)+" <= "+st+"yp"+IntToStr(i+1)+"; "+st+"yz"+IntToStr(i)+" <= "+st+"yz"+IntToStr(i+1)+";");
      }
      else
      {
        memo->Lines->Add("  else if "+frequency+" = '1' and "+frequency+"'event then if "+st+"work='1' and "+st+"st="+GetNumberBin(2*i, kol_bit_styy)+" then");
        memo->Lines->Add("    "+st+"ym"+IntToStr(i)+" <= "+st+"ym"+IntToStr(i+1)+"; "+st+"yp"+IntToStr(i)+" <= "+st+"yp"+IntToStr(i+1)+"; "+st+"yz"+IntToStr(i)+" <= "+st+"yz"+IntToStr(i+1)+";");
      }
      memo->Lines->Add("  end if; end if; end if;");
      memo->Lines->Add("end process;");
    }
    memo->Lines->Add("p_regx"+st+""+IntToStr(kol_coof)+": process ("+frequency+", sc_rst) begin");
    memo->Lines->Add("  if sc_rst = '1' then "+st+"xm"+IntToStr(kol_coof)+" <= (others=>'0'); "+st+"xp"+IntToStr(kol_coof)+" <= (others=>'0'); "+st+"xz"+IntToStr(kol_coof)+" <= '0';");
    memo->Lines->Add("  else if "+frequency+" = '1' and "+frequency+"'event then if "+st+"work='1' and "+st+"st="+GetNumberBin(1, kol_bit_styy)+" then");
    memo->Lines->Add("    "+st+"xm"+IntToStr(kol_coof)+" <= "+st+"inm; "+st+"xp"+IntToStr(kol_coof)+" <= "+st+"inp; "+st+"xz"+IntToStr(kol_coof)+" <= "+st+"inz;");
    memo->Lines->Add("  end if; end if; end if;");
    memo->Lines->Add("end process;");
    memo->Lines->Add("p_regy"+st+IntToStr(kol_coof)+": process ("+frequency+", sc_rst) begin");
    memo->Lines->Add("  if sc_rst = '1' then "+st+"ym"+IntToStr(kol_coof)+" <= (others=>'0'); "+st+"yp"+IntToStr(kol_coof)+" <= (others=>'0'); "+st+"yz"+IntToStr(kol_coof)+" <= '0';");
    memo->Lines->Add("  else if "+frequency+" = '1' and "+frequency+"'event then if "+st+"work='1' then");
    memo->Lines->Add("    if "+st+"st="+GetNumberBin(0, kol_bit_styy)+" then "+st+"ym"+IntToStr(kol_coof)+" <= "+GetNumberBin(0, mantisa-1)+"; "+st+"yp"+IntToStr(kol_coof)+" <= "+GetNumberBin(0, por)+"; "+st+"yz"+IntToStr(kol_coof)+" <= '0';"+
                     "else "+st+"ym"+IntToStr(kol_coof)+" <= "+st+"addm; "+st+"yp"+IntToStr(kol_coof)+" <= "+st+"addp; "+st+"yz"+IntToStr(kol_coof)+" <= "+st+"addz; end if;");
    memo->Lines->Add("  end if; end if; end if;");
    memo->Lines->Add("end process;");
  //YY
    memo->Lines->Add("p_beg"+st+": process ("+name+"_out_irq, "+st+"work_end, sc_rst) begin");
    memo->Lines->Add("  if sc_rst = '1' or "+st+"work_end = '1' then "+st+"beg <= '0';");
    memo->Lines->Add("  else if "+name+"_out_irq = '1' and "+name+"_out_irq'event then");
    memo->Lines->Add("    "+st+"beg <= '1';");
    memo->Lines->Add("  end if; end if;");
    memo->Lines->Add("end process;");
    memo->Lines->Add("p_work"+st+": process ("+frequency+", "+st+"work_end, sc_rst) begin");
    memo->Lines->Add("  if sc_rst = '1' or "+st+"work_end = '1' then "+st+"work <= '0';");
    memo->Lines->Add("  else if "+frequency+" = '1' and "+frequency+"'event then");
    memo->Lines->Add("    if "+st+"beg = '1' then "+st+"work <= '1'; end if;");
    memo->Lines->Add("  end if; end if;");
    memo->Lines->Add("end process;");
    memo->Lines->Add("p_work_end"+st+": process ("+frequency+", "+st+"work) begin");
    memo->Lines->Add("  if "+st+"work = '0' then "+st+"work_end <= '0';");
    memo->Lines->Add("  else if "+frequency+" = '1' and "+frequency+"'event then");
    memo->Lines->Add("    if "+st+"st = "+GetNumberBin(2*kol_coof-1, kol_bit_styy)+" then "+st+"work_end <= '1'; end if;");
    memo->Lines->Add("  end if; end if;");
    memo->Lines->Add("end process;");
    memo->Lines->Add("p_st"+st+": process ("+frequency+", "+st+"work) begin");
    memo->Lines->Add("  if "+st+"work = '0' then "+st+"st <= (others => '0');");
    memo->Lines->Add("  else if "+frequency+" = '1' and "+frequency+"'event then");
    memo->Lines->Add("    "+st+"st <= "+st+"st + 1;");
    memo->Lines->Add("  end if; end if;");
    memo->Lines->Add("end process;");
  }
}  
//---------------------------------------------------------------------------
AnsiString CFilters::DownTo(int end, int begin)
{
  if(end != begin)
    return "(" + IntToStr(end) + " downto " + IntToStr(begin) + ")";
  return "(" + IntToStr(end) + ")";
}
//---------------------------------------------------------------------------
AnsiString CFilters::GetNumberBin(int n, int kol_bit)
{
  AnsiString ret = "";
  n = abs(n);
  for(int i = 0; i < kol_bit; i ++, n /= 2)
    ret = ((n % 2)? "1" : "0") + ret;
  return ((kol_bit > 1)? "\"" : "'") + ret + ((kol_bit > 1)? "\"" : "'");
}
//---------------------------------------------------------------------------
void CFilters::Paint(int n, TImage *image, int s)
{
  if((n >= modules.size()) || (n < 0))
    return;   \
  vector <SFilter>:: iterator p = modules.begin() + n;
  if(!p->begin)
    p->begin ++;
  if(s)
    p->center = s;
  int cen = 0;
  image->Canvas->Brush->Color = clBlack;  //очищаем
  image->Canvas->FillRect(Rect(0, 0, image->Width, image->Height));
  if(p->center == 1)
  {
    for(int i = 0; i < image->Width; i += 10)
      cen += p->data_in[i+p->begin];
    cen /= (image->Width/10);
    Paint(n, image, p->data_midle, clRed, cen);
    Paint(n, image, p->data_out, clLime, cen);
    Paint(n, image, p->data_in, clBlue, cen);
  }
  else if(p->center == 2)
  {
    for(int i = 0; i < image->Width; i += 10)
      cen += p->data_midle[i+p->begin];
    cen /= (image->Width/10);
    Paint(n, image, p->data_in, clBlue, cen);
    Paint(n, image, p->data_out, clLime, cen);
    Paint(n, image, p->data_midle, clRed, cen);
  }
  else
  {
    for(int i = 0; i < image->Width; i += 10)
      cen += p->data_out[i+p->begin];
    cen /= (image->Width/10);
    Paint(n, image, p->data_in, clBlue, cen);
    Paint(n, image, p->data_midle, clRed, cen);
    Paint(n, image, p->data_out, clLime, cen);
  }
}
//---------------------------------------------------------------------------
void CFilters::Paint(int n, TImage *image, int *data, TColor color, int cen)
{
  if((n >= modules.size()) || (n < 0))
    return;
  vector <SFilter>:: iterator p = modules.begin() + n;
  image->Canvas->Pen->Color = color;
  if(p->sz <= 0)
    p->sz = 1;
  image->Canvas->MoveTo(0,  (cen-data[p->begin])/p->sz + image->Height/2);
  for(int i = 1; i < image->Width; i ++)
    image->Canvas->LineTo(i, (cen-data[i+p->begin])/p->sz + image->Height/2);
  image->Canvas->Font->Size = 12;
  image->Canvas->Font->Color = color;
  image->Canvas->TextOutA(5, image->Height/2-12, IntToStr(cen));
}
//---------------------------------------------------------------------------
void CFilters::RunFFT(int n)
{
  if((n >= modules.size()) || (n < 0))
    return;
  vector <SFilter>:: iterator p = modules.begin() + n;
  FFT(n, p->data_in, p->xfft1, p->yfft1);
  FFT(n, p->data_midle, p->xfft2, p->yfft2);
  FFT(n, p->data_out, p->xfft3, p->yfft3);
}
//---------------------------------------------------------------------------
void CFilters::FFT(int n, int *in, double *x, double *y)
{
  if((n >= modules.size()) || (n < 0))
    return;
  vector <SFilter>:: iterator p = modules.begin() + n;
  int m = 9;
  int size = 512;
  for(int i = 0; i < size; i ++)
  {
    y[i] = 0;
    x[i] = in[i+p->begin];
  }
  for(int b = 0; b < m; b ++)
  {
    double e = 1 << (m-b);
    double f = e / 2;
    double u = 1;
    double v = 0;
    double z = 3.14159 / f;
    double c = cos(z);
    double s = -sin(z);
    for(int j = 0; j < f; j ++)
    {
      for(int i = j; i < size; i += e)
      {
        int g = i + f;
        double d1 = x[i] + x[g];
        double d2 = y[i] + y[g];
        double r = x[i] - x[g];
        double t = y[i] - y[g];
        x[i] = d1;
        y[i] = d2;
        x[g] = r*u - t*v;
        y[g] = t*u + r*v;
      }
      double w = u*c - v*s;
      v = (v*c + u*s);
      u = w;
    }
  }
  for(unsigned i = 1; i < size - 1; i++)
  {
    unsigned j = 0;
    for(unsigned k = 0; k < m; k++)
      if(i & (0x01 << k))
        j += (size >> k+1);
    if((i >= size) || (j >= size) || (j >= i))
      continue;
    double d = x[j]; x[j] = x[i]; x[i] = d;
    d = y[j]; y[j] = y[i]; y[i] = d;
  }
  for(int i = 0; i < size; i ++)
  {
    x[i] /= size;
    y[i] /= size;
  }
}
//---------------------------------------------------------------------------
void CFilters::PaintFFT(int n, TImage *image, int s)
{
  if((n >= modules.size()) || (n < 0))
    return;
  vector <SFilter>:: iterator p = modules.begin() + n;
  RunFFT(n);
  if(s)
    p->center = s;
  image->Canvas->Brush->Color = clBlack;  //очищаем
  image->Canvas->FillRect(Rect(0, 0, image->Width, image->Height));
  if(p->center == 1)
  {
    PaintFFT(n, image, p->xfft2, p->yfft2, clRed);
    PaintFFT(n, image, p->xfft3, p->yfft3, clLime);
    PaintFFT(n, image, p->xfft1, p->yfft1, clBlue);
  }
  else if(p->center == 2)
  {
    PaintFFT(n, image, p->xfft1, p->yfft1, clBlue);
    PaintFFT(n, image, p->xfft3, p->yfft3, clLime);
    PaintFFT(n, image, p->xfft2, p->yfft2, clRed);
  }
  else
  {
    PaintFFT(n, image, p->xfft1, p->yfft1, clBlue);
    PaintFFT(n, image, p->xfft2, p->yfft2, clRed);
    PaintFFT(n, image, p->xfft3, p->yfft3, clLime);
  }
}
//---------------------------------------------------------------------------
void CFilters::PaintFFT(int n, TImage *image, double *x, double *y, TColor color)
{
  if((n >= modules.size()) || (n < 0))
    return;
  vector <SFilter>:: iterator p = modules.begin() + n;
  image->Canvas->Pen->Color = color;
  if(p->sz2 == 0)
    p->sz2 = 1;
  double sz3 = (p->sz2 <= 0)? 1/fabs(p->sz2) : p->sz2;
  for(int i = 1; i < image->Width/2; i ++)
  {
    double s = 2*sqrt(x[i]*x[i] + y[i]*y[i]);
    image->Canvas->MoveTo(2*i, image->Height);
    image->Canvas->LineTo(2*i, -(double)s/sz3 + image->Height);
    image->Canvas->MoveTo(2*i+1, image->Height);
    image->Canvas->LineTo(2*i+1, -(double)s/sz3 + image->Height);
  }
  image->Canvas->Font->Size = 8;
  image->Canvas->Font->Color = color;
  image->Canvas->TextOut(image->Width/4-10,image->Height-12,FloatToStrF((double)p->fd*1/8, ffFixed, 9, 2));
  image->Canvas->TextOut(image->Width/2-20,image->Height-12,FloatToStrF((double)p->fd*2/8, ffFixed, 9, 2));
  image->Canvas->TextOut(image->Width*3/4-20,image->Height-12,FloatToStrF((double)p->fd*3/8, ffFixed, 9, 2));
  image->Canvas->TextOut(image->Width-30,image->Height-12,FloatToStrF((double)p->fd*4/8, ffFixed, 9, 2));
}
//---------------------------------------------------------------------------
void CFilters::SetBegin(int n, int b)
{
  if((n >= modules.size()) || (n < 0))
    return;
  vector <SFilter>:: iterator p = modules.begin() + n;
  p->begin = b;
}
//---------------------------------------------------------------------------
void CFilters::SetSz(int n, int b)
{
  if((n >= modules.size()) || (n < 0))
    return;
  vector <SFilter>:: iterator p = modules.begin() + n;
  p->sz = b;
}
//---------------------------------------------------------------------------
void CFilters::SetSz2(int n, int b)
{
  if((n >= modules.size()) || (n < 0))
    return;
  vector <SFilter>:: iterator p = modules.begin() + n;
  p->sz2 = b;
}
//---------------------------------------------------------------------------
