//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("Main.cpp", MainForm);
USEFORM("UOptionsProcForm.cpp", OptionsProcForm);
USEFORM("UPortsForm.cpp", PortsForm);
USEFORM("UInputsForm.cpp", InputForm);
USEFORM("UDiv.cpp", DivForm);
USEFORM("AboutForm.cpp", AboutBox);
USEFORM("URam.cpp", RamForm);
USEFORM("URunForm.cpp", RunForm);
USEFORM("UPrint.cpp", PrintForm);
USEFORM("AboutForm1.cpp", AboutBox1);
USEFORM("USoproch.cpp", SoprochForm);
USEFORM("UPeriphery.cpp", PeripheryForm);
USEFORM("UModules.cpp", ModulForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TMainForm), &MainForm);
                 Application->CreateForm(__classid(TOptionsProcForm), &OptionsProcForm);
                 Application->CreateForm(__classid(TPortsForm), &PortsForm);
                 Application->CreateForm(__classid(TInputForm), &InputForm);
                 Application->CreateForm(__classid(TDivForm), &DivForm);
                 Application->CreateForm(__classid(TAboutBox), &AboutBox);
                 Application->CreateForm(__classid(TRamForm), &RamForm);
                 Application->CreateForm(__classid(TRunForm), &RunForm);
                 Application->CreateForm(__classid(TPrintForm), &PrintForm);
                 Application->CreateForm(__classid(TAboutBox1), &AboutBox1);
                 Application->CreateForm(__classid(TSoprochForm), &SoprochForm);
                 Application->CreateForm(__classid(TPeripheryForm), &PeripheryForm);
                 Application->CreateForm(__classid(TModulForm), &ModulForm);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        catch (...)
        {
                 try
                 {
                         throw Exception("");
                 }
                 catch (Exception &exception)
                 {
                         Application->ShowException(&exception);
                 }
        }
        return 0;
}
//---------------------------------------------------------------------------
