object PrintForm: TPrintForm
  Left = 217
  Top = 164
  BorderStyle = bsDialog
  Caption = #1055#1077#1095#1072#1090#1100
  ClientHeight = 287
  ClientWidth = 298
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 8
    Top = 56
    Width = 281
    Height = 225
    Shape = bsFrame
  end
  object CancelBtn: TButton
    Left = 20
    Top = 246
    Width = 253
    Height = 25
    Cancel = True
    Caption = #1054#1090#1084#1077#1085#1072
    ModalResult = 2
    TabOrder = 0
  end
  object ButSetup: TButton
    Left = 24
    Top = 16
    Width = 249
    Height = 25
    Caption = #1059#1089#1090#1072#1085#1086#1074#1082#1080' '#1087#1088#1080#1085#1090#1077#1088#1072
    TabOrder = 1
    OnClick = ButSetupClick
  end
  object ButPrint1: TButton
    Left = 24
    Top = 64
    Width = 249
    Height = 25
    Caption = #1055#1077#1095#1072#1090#1100' '#1089#1090#1088#1072#1085#1080#1094#1099
    TabOrder = 2
    OnClick = ButPrint1Click
  end
  object Button3: TButton
    Left = 24
    Top = 192
    Width = 249
    Height = 25
    Caption = #1054#1047#1059
    TabOrder = 3
  end
  object PrinterSetupDialog1: TPrinterSetupDialog
    Left = 248
    Top = 40
  end
end
