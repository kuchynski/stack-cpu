object AboutBox: TAboutBox
  Left = 249
  Top = 274
  BorderStyle = bsDialog
  Caption = #1069#1090#1086
  ClientHeight = 219
  ClientWidth = 298
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 281
    Height = 169
    BevelInner = bvRaised
    BevelOuter = bvLowered
    ParentColor = True
    TabOrder = 0
    object ProductName: TLabel
      Left = 48
      Top = 8
      Width = 64
      Height = 13
      Caption = #1050#1086#1085#1089#1090#1088#1091#1082#1090#1086#1088
      IsControl = True
    end
    object Version: TLabel
      Left = 48
      Top = 72
      Width = 74
      Height = 13
      Caption = #1042#1077#1088#1089#1080#1103' '#1090#1088#1077#1090#1100#1103
      IsControl = True
    end
    object Comments: TLabel
      Left = 112
      Top = 144
      Width = 60
      Height = 13
      Caption = #1052#1080#1085#1089#1082' 2007'
      WordWrap = True
      IsControl = True
    end
    object Label1: TLabel
      Left = 48
      Top = 96
      Width = 85
      Height = 13
      Caption = 'kuchynski@tut.by'
      WordWrap = True
      IsControl = True
    end
    object Label3: TLabel
      Left = 48
      Top = 24
      Width = 204
      Height = 13
      Caption = #1089#1090#1077#1082#1086#1074#1086#1075#1086' '#1087#1088#1086#1094#1077#1089#1089#1086#1088#1072' '#1076#1083#1103' FPGA XILINX'
    end
    object Label4: TLabel
      Left = 48
      Top = 56
      Width = 150
      Height = 13
      Caption = 'Spartan 2, Spartan 3, Spartan 6'
    end
    object Label5: TLabel
      Left = 48
      Top = 40
      Width = 136
      Height = 13
      Caption = 'Vertex-E, Vertex-2, Virtex-2P,'
    end
    object Label2: TLabel
      Left = 48
      Top = 112
      Width = 93
      Height = 13
      Caption = '(+375-44)730-26-07'
      IsControl = True
    end
  end
  object OKButton: TButton
    Left = 56
    Top = 188
    Width = 193
    Height = 25
    Caption = #1057#1082#1086#1088#1077#1077' '#1082#1086#1085#1089#1090#1088#1091#1080#1088#1086#1074#1072#1090#1100'!'
    Default = True
    ModalResult = 1
    TabOrder = 1
  end
end
