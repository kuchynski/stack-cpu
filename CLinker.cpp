//---------------------------------------------------------------------------
#pragma hdrstop
#include "CLinker.h"

CLinker linker;
//---------------------------------------------------------------------------
void CLinker::Clear()
{
  words.clear();
  ALU.Clear();
  ROM.Clear();
  RAM.Clear();
  stack_data.Clear();
  stack_return.Clear();
  ports.Clear();
  str_mail.clear();
}
//---------------------------------------------------------------------------
void CLinker::AddMailWord(vector <CCommandString> s)
{
  str_mail = s;
}
//---------------------------------------------------------------------------
bool CLinker::Run(COptionsProc &options_proc)
{
  //options_proc = op;

  CMyWord c = CMyWord("main", str_mail);
  c.AddUse();
  IRQ.AddUseToFunctions();
  k1 = dictionary.RunTaskKodOp();

  RunWords();
  RunRAM();
  RunROM();
  IRQ.Run();
  RunEnabledIRQ();
  size_shina_data = ports.GetSize() + RAM.GetSize();
  k2 = (ROM.GetSize() > size_shina_data)? ROM.GetSize() : size_shina_data;
  if(k2)
    for(int i = 1; i < 33; i++)
      if(IntPower(2, i) >= k2)
      {
        k2 = i;
        break;
      }
  if(size_shina_data)
    for(int i = 1; i < 33; i++)
      if(IntPower(2, i) >= size_shina_data)
      {
        size_shina_data = i;
        break;
      }
//  ALU.SetKolBitData(options_proc.GetZnakData(), (size_shina_data > options_proc.GetaRazradData())? size_shina_data : options_proc.GetaRazradData());
  ALU.SetKolBitData(options_proc.GetZnakData(), options_proc.GetaRazradData());
  ALU.SetCommands(dictionary.GetStandartWords());

  YY.SetCommands(dictionary.GetStandartWords());
  ROM.SetKolBitData(k1, k2);
  RAM.SetKolBitData(options_proc.GetaRazradData());
  RAM.Linker();

  stack_data.SetSize(options_proc.GetKolRegStackData());
  stack_data.SetKolBitData((size_shina_data > options_proc.GetaRazradData())? size_shina_data : options_proc.GetaRazradData());

  int kol_bit_irq = IRQ.GetSize();
  if(kol_bit_irq)
  {
    kol_bit_irq ++;
    for(int i = 1; i < 33; i++)
      if(IntPower(2, i) >= kol_bit_irq)
      {
        kol_bit_irq = i;
        break;
      }
  }
  stack_return.SetSize(options_proc.GetKolRegStackReturn());
  unsigned size_sr;
  if(dictionary.GetKolUse(">R") || dictionary.GetKolUse("R>"))
    size_sr =  (stack_data.GetKolBitDataWhithZnak() > (ROM.GetKolBitAddress() + kol_bit_irq))?
                stack_data.GetKolBitDataWhithZnak() : (ROM.GetKolBitAddress() + kol_bit_irq);
  else
    size_sr =  ROM.GetKolBitAddress() + kol_bit_irq;
  stack_return.SetKolBitData(size_sr);
  stack_return.SetKolBitIrq(kol_bit_irq);

  div_frequency.RunLinker();
  return RunOtchet();
}
//---------------------------------------------------------------------------
void CLinker::RunWords()
{
  words.clear();
  if(!str_mail.size())
    return;
  words.push_back(CMyWord("main", str_mail));
  vector<CMyWord>:: iterator p = words.end() - 1;
  p->SetKod();
  vector <CMyWord> w = dictionary.GetMyWords();
  if(!w.size())
    return;
  for(vector <CMyWord>:: iterator p = w.end() - 1; p >= w.begin(); p --)
  {
    if(p->GetKolKod())
      if(p->GetKolUse())
        if(p->GetInLine())
          edit_error->Lines->Add("Уведомляю, что слово '" + p->GetName() + "' используется мало, потому будет встраевомым");
        else
          words.push_back(*p);
      else
        edit_error->Lines->Add("Уведомляю, что слово '" + p->GetName() + "' будет отсутствовать в ПЗУ, т.к. никому не нужно");
    else
      if(p->GetKolUse())
        edit_error->Lines->Add("Уведомляю, что слово '" + p->GetName() + "' будет отсутствовать в ПЗУ по причине безполезности");
      else
        edit_error->Lines->Add("Уведомляю, что слово '" + p->GetName() + "' будет отсутствовать в ПЗУ из-за своей безполезности и невостребованости");
  }  
}
//---------------------------------------------------------------------------
void CLinker::RunRAM()
{
  RAM.Clear();
  vector <CVariableWord> variables = dictionary.GetMyVariables();
  for(vector <CVariableWord>:: iterator p = variables.begin(); p < variables.end(); p ++)
    if(p->GetKolUse())
      RAM.AddData(*p);
    else
      if(p->IsFromGrid())
        edit_error->Lines->Add("Предупреждаю, что ячейка памяти '" + p->GetName() + "' удалена из-за нечастого использования");
}
//---------------------------------------------------------------------------
void CLinker::RunEnabledIRQ()
{
  if(!ports.IsUse("irq_en"))
    ports.DeletePort("irq_en");
  if(!ports.IsUse("irq_en_low"))
    ports.DeletePort("irq_en_low");
}
//---------------------------------------------------------------------------
void CLinker::RunROM()
{
  ROM.Clear();
  if(!words.size())
    return;
  CFunction c = CFunction("main", words.begin()->GetAllKod());
//  c.AddCommandForth(dictionary.GetKod("QUIT"));
  ROM.AddFunction(c);
  for(vector <CMyWord>:: iterator p = words.begin() + 1; p < words.end(); p ++)
    ROM.AddFunction(CFunction(p->GetName(), p->GetAllKod()));
}
//---------------------------------------------------------------------------
bool CLinker::RunOtchet()
{
  edit_error->Lines->Add("\nОтчитываюсь:");
  edit_error->Lines->Add("Стек данных:\t\t" + IntToStr(stack_data.GetSize()) + " слов");
  edit_error->Lines->Add("Стек возвратов:\t" + IntToStr(stack_return.GetSize()) + " слов");
  edit_error->Lines->Add("АЛУ:\t\tсчитает родными " + IntToStr(ALU.GetKolCommands()) + " команд");
  edit_error->Lines->Add("Устройство управления:\tвоспринимает " + IntToStr(YY.GetKolCommands()) + " команд");
  edit_error->Lines->Add("Размер ОЗУ вот такой - " + IntToStr(RAM.GetSize()) + " слов");
  edit_error->Lines->Add("Количество портов - " + IntToStr(ports.GetSize()));
  edit_error->Lines->Add("\t\tРазрядность шины адреса данных - " + IntToStr(size_shina_data));
  edit_error->Lines->Add("\t\tРазрядность слова ОЗУ - " + IntToStr(RAM.GetKolBitDataWhithZnak()));
  edit_error->Lines->Add("Далее, по ПЗУ:\tзаписано " + IntToStr(ROM.GetKolCommandsForth()) + " команд ФОРТ,");
  edit_error->Lines->Add("\t\tчто составило " + IntToStr(ROM.GetKolCommands()) + " команд процессора.");
  edit_error->Lines->Add("\t\tИспользуется " + IntToStr(ROM.GetSize()) + " слов ПЗУ.");
  edit_error->Lines->Add("\t\tРазрядность шины адреса ПЗУ - " + IntToStr(ROM.GetKolBitAddress()));
  edit_error->Lines->Add("\t\tРазрядность слова ПЗУ - " + IntToStr(ROM.GetKolBitData()));

  if(ROM.GetSize() > 1)
    edit_error->Lines->Add("и ладно");
  else
  {
    edit_error->Lines->Add("что писал, всё зря");
    return false;
  }
  return true;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
