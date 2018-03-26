object RamForm: TRamForm
  Left = 249
  Top = 103
  Width = 223
  Height = 252
  BorderIcons = []
  Caption = 'RAM'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 215
    Height = 33
    Align = alTop
    TabOrder = 0
    object ButRAMSS: TSpeedButton
      Left = 8
      Top = 4
      Width = 25
      Height = 25
      Caption = 'hex'
      OnClick = ButRAMSSClick
    end
  end
  object grid_debug_RAM: TStringGrid
    Left = 0
    Top = 33
    Width = 215
    Height = 192
    Align = alClient
    ColCount = 3
    DefaultColWidth = 72
    DefaultRowHeight = 16
    FixedCols = 0
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goRowSelect]
    TabOrder = 1
    ColWidths = (
      48
      83
      56)
  end
end
