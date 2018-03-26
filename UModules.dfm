object ModulForm: TModulForm
  Left = 231
  Top = 189
  BorderStyle = bsDialog
  Caption = #1052#1086#1076#1091#1083#1080
  ClientHeight = 674
  ClientWidth = 854
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Panel3: TPanel
    Left = 0
    Top = 168
    Width = 377
    Height = 505
    TabOrder = 0
    object Memo2: TMemo
      Left = 1
      Top = 73
      Width = 375
      Height = 32
      Align = alTop
      BevelInner = bvNone
      BevelOuter = bvNone
      Color = clBtnFace
      Lines.Strings = (
        'entity lcd_controller is '
        'Port (')
      ReadOnly = True
      TabOrder = 0
    end
    object grid_entity: TStringGrid
      Left = 1
      Top = 105
      Width = 335
      Height = 319
      Align = alClient
      ColCount = 4
      DefaultColWidth = 24
      DefaultRowHeight = 16
      FixedCols = 0
      RowCount = 2
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
      TabOrder = 1
      OnGetEditMask = grid_entityGetEditMask
      OnSelectCell = grid_entitySelectCell
      OnSetEditText = grid_entitySetEditText
      ColWidths = (
        107
        75
        34
        83)
    end
    object Memo7: TMemo
      Left = 1
      Top = 424
      Width = 375
      Height = 39
      Align = alBottom
      Color = clBtnFace
      Lines.Strings = (
        '); '
        'end lcd;')
      ReadOnly = True
      TabOrder = 2
    end
    object Panel4: TPanel
      Left = 336
      Top = 105
      Width = 40
      Height = 319
      Align = alRight
      TabOrder = 3
      object BitBtn1: TBitBtn
        Left = 7
        Top = 12
        Width = 25
        Height = 25
        Hint = #1044#1086#1073#1072#1074#1080#1090#1100
        TabOrder = 0
        OnClick = BitBtn1Click
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          04000000000000010000120B0000120B00001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333303333
          333333333337F33333333333333033333333333333373F3333333333330A0333
          33333333337F7F3333333333330A033333333333337373F33333333330AAA033
          3333333337F337F33333333330AAA033333333333733373F333333330AAAAA03
          333333337F33337F333333330AAAAA033333333373333373F3333330AAAAAAA0
          33333337FFFF3FF7F3333330000A000033333337777F777733333333330A0333
          33333333337F7F3333333333330A033333333333337F7F3333333333330A0333
          33333333337F7F3333333333330A033333333333337F7F3333333333330A0333
          33333333337F7F33333333333300033333333333337773333333}
        NumGlyphs = 2
      end
      object BitBtn2: TBitBtn
        Left = 7
        Top = 44
        Width = 25
        Height = 25
        Hint = #1059#1076#1072#1083#1080#1090#1100
        TabOrder = 1
        OnClick = BitBtn2Click
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          04000000000000010000130B0000130B00001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
          3333333333333333333333333333333333333333333333333333399933333333
          3333388833333333333333399333333333333338833333333333333999333333
          9933333888333333883333333993333993333333388333388333333333999999
          3333333333888888333333333339999333333333333888833333333333339999
          3333333333338888333333333399999933333333338888883333333399993399
          9333333388883388833333399933333999333338883333388833339393333333
          9993338383333333888339393333333339933838333333333883393933333333
          3333383833333333333333333333333333333333333333333333}
        NumGlyphs = 2
      end
    end
    object Memo8: TMemo
      Left = 1
      Top = 1
      Width = 375
      Height = 72
      Align = alTop
      BevelInner = bvNone
      BevelOuter = bvNone
      Color = clBtnFace
      Lines.Strings = (
        'library IEEE;'
        'use IEEE.STD_LOGIC_1164.ALL;'
        'use IEEE.STD_LOGIC_ARITH.ALL;'
        'use IEEE.STD_LOGIC_UNSIGNED.ALL;'
        'library unisim;'
        'use unisim.vcomponents.all;')
      ReadOnly = True
      TabOrder = 4
    end
    object Panel1: TPanel
      Left = 1
      Top = 463
      Width = 375
      Height = 41
      Align = alBottom
      TabOrder = 5
      object OKBtn: TButton
        Left = 12
        Top = 8
        Width = 75
        Height = 25
        Caption = 'OK'
        Default = True
        ModalResult = 1
        TabOrder = 0
        OnClick = OKBtnClick
      end
      object CancelBtn: TButton
        Left = 100
        Top = 8
        Width = 75
        Height = 25
        Cancel = True
        Caption = 'Cancel'
        ModalResult = 2
        TabOrder = 1
        OnClick = CancelBtnClick
      end
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 153
    Height = 169
    TabOrder = 1
    object StringGridModul: TStringGrid
      Left = 1
      Top = 1
      Width = 151
      Height = 145
      Align = alTop
      ColCount = 1
      DefaultColWidth = 24
      DefaultRowHeight = 16
      FixedCols = 0
      RowCount = 2
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
      TabOrder = 0
      OnSelectCell = StringGridModulSelectCell
      OnSetEditText = StringGridModulSetEditText
      ColWidths = (
        118)
    end
  end
  object ButAdd: TBitBtn
    Left = 160
    Top = 4
    Width = 25
    Height = 25
    Hint = #1044#1086#1073#1072#1074#1080#1090#1100
    TabOrder = 2
    OnClick = ButAddClick
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000120B0000120B00001000000000000000000000000000
      8000008000000080800080000000800080008080000080808000C0C0C0000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333303333
      333333333337F33333333333333033333333333333373F3333333333330A0333
      33333333337F7F3333333333330A033333333333337373F33333333330AAA033
      3333333337F337F33333333330AAA033333333333733373F333333330AAAAA03
      333333337F33337F333333330AAAAA033333333373333373F3333330AAAAAAA0
      33333337FFFF3FF7F3333330000A000033333337777F777733333333330A0333
      33333333337F7F3333333333330A033333333333337F7F3333333333330A0333
      33333333337F7F3333333333330A033333333333337F7F3333333333330A0333
      33333333337F7F33333333333300033333333333337773333333}
    NumGlyphs = 2
  end
  object ButSub: TBitBtn
    Left = 160
    Top = 36
    Width = 25
    Height = 25
    Hint = #1059#1076#1072#1083#1080#1090#1100
    TabOrder = 3
    OnClick = ButSubClick
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000130B0000130B00001000000000000000000000000000
      8000008000000080800080000000800080008080000080808000C0C0C0000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
      3333333333333333333333333333333333333333333333333333399933333333
      3333388833333333333333399333333333333338833333333333333999333333
      9933333888333333883333333993333993333333388333388333333333999999
      3333333333888888333333333339999333333333333888833333333333339999
      3333333333338888333333333399999933333333338888883333333399993399
      9333333388883388833333399933333999333338883333388833339393333333
      9993338383333333888339393333333339933838333333333883393933333333
      3333383833333333333333333333333333333333333333333333}
    NumGlyphs = 2
  end
  object Panel5: TPanel
    Left = 381
    Top = 0
    Width = 473
    Height = 674
    Align = alRight
    TabOrder = 4
    object memo_arch: TMemo
      Left = 1
      Top = 120
      Width = 471
      Height = 528
      Align = alClient
      ScrollBars = ssBoth
      TabOrder = 0
    end
    object memo_signals: TMemo
      Left = 1
      Top = 25
      Width = 471
      Height = 72
      Align = alTop
      ScrollBars = ssBoth
      TabOrder = 1
    end
    object Memo5: TMemo
      Left = 1
      Top = 1
      Width = 471
      Height = 24
      Align = alTop
      Color = clBtnFace
      Lines.Strings = (
        'architecture arch of lcd is')
      ReadOnly = True
      TabOrder = 2
    end
    object Memo6: TMemo
      Left = 1
      Top = 97
      Width = 471
      Height = 23
      Align = alTop
      Color = clBtnFace
      Lines.Strings = (
        'begin')
      ReadOnly = True
      TabOrder = 3
    end
    object Memo3: TMemo
      Left = 1
      Top = 648
      Width = 471
      Height = 25
      Align = alBottom
      Color = clBtnFace
      Lines.Strings = (
        'end arch;')
      ReadOnly = True
      TabOrder = 4
    end
  end
end
