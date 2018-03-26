object SoprochForm: TSoprochForm
  Left = 89
  Top = 291
  BorderStyle = bsDialog
  Caption = #1057#1086#1087#1088#1086#1094#1077#1089#1089#1086#1088
  ClientHeight = 635
  ClientWidth = 819
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object OKBtn: TButton
    Left = 740
    Top = 512
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
    OnClick = OKBtnClick
  end
  object CancelBtn: TButton
    Left = 740
    Top = 550
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
    OnClick = CancelBtnClick
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 721
    Height = 635
    ActivePage = TabSheet2
    Align = alLeft
    TabIndex = 1
    TabOrder = 2
    object TabSheet1: TTabSheet
      Caption = #1044#1077#1083#1080#1090#1077#1083#1100
      object ButAddDelit: TBitBtn
        Left = 352
        Top = 236
        Width = 25
        Height = 25
        Hint = #1044#1086#1073#1072#1074#1080#1090#1100
        TabOrder = 0
        OnClick = ButAddDelitClick
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
      object ButSubDelit: TBitBtn
        Left = 456
        Top = 244
        Width = 25
        Height = 25
        Hint = #1059#1076#1072#1083#1080#1090#1100
        TabOrder = 1
        OnClick = ButSubDelitClick
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
      object StringGridDelit: TStringGrid
        Left = 0
        Top = 0
        Width = 713
        Height = 145
        Align = alTop
        ColCount = 2
        DefaultColWidth = 24
        DefaultRowHeight = 16
        FixedCols = 0
        RowCount = 2
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
        TabOrder = 2
        OnSelectCell = StringGridDelitSelectCell
        OnSetEditText = StringGridDelitSetEditText
        ColWidths = (
          118
          565)
      end
    end
    object TabSheet2: TTabSheet
      Caption = #1060#1080#1083#1100#1090#1088
      ImageIndex = 1
      object PanelFilter: TPanel
        Left = 0
        Top = 104
        Width = 713
        Height = 503
        Align = alBottom
        BevelInner = bvLowered
        BevelOuter = bvLowered
        TabOrder = 0
        object PageControl2: TPageControl
          Left = 2
          Top = 2
          Width = 709
          Height = 499
          ActivePage = TabSheet4
          Align = alClient
          TabIndex = 1
          TabOrder = 0
          object TabSheet3: TTabSheet
            Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099
            object Label2: TLabel
              Left = 8
              Top = 120
              Width = 87
              Height = 13
              Caption = #1050#1086#1101#1092#1092#1080#1094#1080#1077#1085#1090#1099' b'
            end
            object Label5: TLabel
              Left = 8
              Top = 168
              Width = 87
              Height = 13
              Caption = #1050#1086#1101#1092#1092#1080#1094#1080#1077#1085#1090#1099' a'
            end
            object Label6: TLabel
              Left = 8
              Top = 187
              Width = 6
              Height = 13
              Caption = '1'
            end
            object Label14: TLabel
              Left = 8
              Top = 92
              Width = 39
              Height = 13
              Caption = #1056#1072#1079#1084#1077#1088
            end
            object GroupBox1: TGroupBox
              Left = 0
              Top = 0
              Width = 129
              Height = 81
              Caption = #1056#1072#1079#1088#1103#1076#1085#1086#1089#1090#1100
              TabOrder = 0
              object Label3: TLabel
                Left = 8
                Top = 24
                Width = 43
                Height = 13
                Caption = #1084#1072#1085#1090#1080#1089#1072
              end
              object Label4: TLabel
                Left = 72
                Top = 24
                Width = 42
                Height = 13
                Caption = #1087#1086#1088#1103#1076#1086#1082
              end
              object CSpinEditMantisa: TCSpinEdit
                Left = 8
                Top = 40
                Width = 49
                Height = 22
                MaxValue = 32
                MinValue = 8
                TabOrder = 0
                Value = 8
              end
              object CSpinEditPor: TCSpinEdit
                Left = 72
                Top = 40
                Width = 49
                Height = 22
                MaxValue = 30
                MinValue = 6
                TabOrder = 1
                Value = 8
              end
            end
            object EditB: TEdit
              Left = 8
              Top = 136
              Width = 689
              Height = 21
              TabOrder = 1
            end
            object EditA: TEdit
              Left = 8
              Top = 184
              Width = 689
              Height = 21
              TabOrder = 2
            end
            object GroupBox2: TGroupBox
              Left = 0
              Top = 208
              Width = 553
              Height = 137
              Caption = #1043#1088#1072#1092#1080#1082
              TabOrder = 3
              object Label7: TLabel
                Left = 8
                Top = 24
                Width = 122
                Height = 13
                Caption = #1063#1072#1089#1090#1086#1090#1072' '#1076#1080#1089#1082#1088#1077#1090#1080#1079#1072#1094#1080#1080
              end
              object Label8: TLabel
                Left = 232
                Top = 24
                Width = 65
                Height = 13
                Caption = #1043#1072#1088#1084#1086#1085#1080#1082#1072' 1'
              end
              object Label9: TLabel
                Left = 304
                Top = 24
                Width = 65
                Height = 13
                Caption = #1043#1072#1088#1084#1086#1085#1080#1082#1072' 2'
              end
              object Label10: TLabel
                Left = 376
                Top = 24
                Width = 65
                Height = 13
                Caption = #1043#1072#1088#1084#1086#1085#1080#1082#1072' 3'
              end
              object Label11: TLabel
                Left = 168
                Top = 50
                Width = 42
                Height = 13
                Caption = #1063#1072#1089#1090#1086#1090#1072
              end
              object Label12: TLabel
                Left = 168
                Top = 74
                Width = 55
                Height = 13
                Caption = #1040#1084#1087#1083#1080#1090#1091#1076#1072
              end
              object Label13: TLabel
                Left = 168
                Top = 100
                Width = 54
                Height = 13
                Caption = #1057#1084#1077#1097#1077#1085#1080#1077
              end
              object EditFd: TEdit
                Left = 8
                Top = 40
                Width = 65
                Height = 21
                TabOrder = 0
              end
              object EditFr1: TEdit
                Left = 232
                Top = 48
                Width = 57
                Height = 21
                TabOrder = 1
              end
              object EditFr2: TEdit
                Left = 304
                Top = 48
                Width = 57
                Height = 21
                TabOrder = 2
              end
              object EditFr3: TEdit
                Left = 376
                Top = 48
                Width = 57
                Height = 21
                TabOrder = 3
              end
              object EditA3: TEdit
                Left = 376
                Top = 72
                Width = 57
                Height = 21
                TabOrder = 4
              end
              object EditA2: TEdit
                Left = 304
                Top = 72
                Width = 57
                Height = 21
                TabOrder = 5
              end
              object EditA1: TEdit
                Left = 232
                Top = 72
                Width = 57
                Height = 21
                TabOrder = 6
              end
              object EditSm: TEdit
                Left = 232
                Top = 100
                Width = 201
                Height = 21
                TabOrder = 7
              end
            end
            object CSpinEditKol: TCSpinEdit
              Left = 56
              Top = 88
              Width = 49
              Height = 22
              MaxValue = 32
              MinValue = 2
              TabOrder = 4
              Value = 8
            end
            object ButRunFilter: TBitBtn
              Left = 216
              Top = 12
              Width = 97
              Height = 25
              Hint = #1059#1076#1072#1083#1080#1090#1100
              Caption = #1055#1088#1086#1074#1077#1088#1080#1090#1100
              TabOrder = 5
              OnClick = ButRunFilterClick
              Glyph.Data = {
                76010000424D7601000000000000760000002800000020000000100000000100
                04000000000000010000120B0000120B00001000000000000000000000000000
                8000008000000080800080000000800080008080000080808000C0C0C0000000
                FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
                3333333333333333333333333303333333333333333333333333333333003333
                333333333333333333333333330C003333333333333FF33333330000000CCC03
                3333333333773FF333330CCCCCCCCCC033333333337F773FF3330CCCCCCCCCCC
                00333FFFFF7F33773FF30CCCCCCCCCCCCC03777777733333773F0CCCCCCCCCCC
                CCC07FFFFFFF33333F770CCCCCCCCCCCCC037777777F333F77330CCCCCCCCCC0
                00333333337F3F7733330CCCCCCCCCC033333333337F773333330000000CCC03
                333333333377333333333333330C003333333333333333333333333333003333
                3333333333333333333333333303333333333333333333333333}
              NumGlyphs = 2
            end
          end
          object TabSheet4: TTabSheet
            Caption = #1043#1088#1072#1092#1080#1082
            ImageIndex = 1
            object Panel2: TPanel
              Left = 0
              Top = 409
              Width = 701
              Height = 62
              Align = alClient
              BevelInner = bvRaised
              TabOrder = 0
              object ScrollBar1: TScrollBar
                Left = 0
                Top = 9
                Width = 689
                Height = 15
                LargeChange = 500
                Max = 9300
                Min = 1
                PageSize = 0
                Position = 1
                SmallChange = 100
                TabOrder = 0
                OnChange = ScrollBar1Change
              end
              object ButIn: TBitBtn
                Left = 336
                Top = 29
                Width = 65
                Height = 25
                Caption = #1042#1093#1086#1076
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clBlue
                Font.Height = -11
                Font.Name = 'MS Sans Serif'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 1
                OnClick = ButInClick
              end
              object ButMidle: TBitBtn
                Left = 408
                Top = 29
                Width = 121
                Height = 25
                Caption = #1042#1099#1093#1086#1076' '#1055#1069#1042#1052
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clRed
                Font.Height = -11
                Font.Name = 'MS Sans Serif'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 2
                OnClick = ButMidleClick
              end
              object ButOut: TBitBtn
                Left = 536
                Top = 29
                Width = 89
                Height = 25
                Caption = #1042#1099#1093#1086#1076' VHDL'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clLime
                Font.Height = -11
                Font.Name = 'MS Sans Serif'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 3
                OnClick = ButOutClick
              end
            end
            object PageControl3: TPageControl
              Left = 0
              Top = 0
              Width = 701
              Height = 409
              ActivePage = TabSheet6
              Align = alTop
              TabIndex = 1
              TabOrder = 1
              object TabSheet5: TTabSheet
                Caption = #1060#1091#1085#1082#1094#1080#1103
                object image: TImage
                  Left = 0
                  Top = -8
                  Width = 512
                  Height = 337
                end
                object Label15: TLabel
                  Left = 8
                  Top = 352
                  Width = 38
                  Height = 13
                  Caption = #1057#1078#1072#1090#1080#1077
                end
                object Label16: TLabel
                  Left = 379
                  Top = 352
                  Width = 3
                  Height = 13
                end
                object TrackBar1: TTrackBar
                  Left = 48
                  Top = 344
                  Width = 313
                  Height = 25
                  Max = 100
                  Min = 1
                  Orientation = trHorizontal
                  Frequency = 1
                  Position = 1
                  SelEnd = 0
                  SelStart = 0
                  TabOrder = 0
                  TickMarks = tmTopLeft
                  TickStyle = tsAuto
                  OnChange = TrackBar1Change
                end
              end
              object TabSheet6: TTabSheet
                Caption = #1057#1087#1077#1082#1090#1088
                ImageIndex = 1
                object imagefft: TImage
                  Left = 0
                  Top = 8
                  Width = 512
                  Height = 337
                end
                object Label17: TLabel
                  Left = 8
                  Top = 360
                  Width = 38
                  Height = 13
                  Caption = #1057#1078#1072#1090#1080#1077
                end
                object Label20: TLabel
                  Left = 387
                  Top = 352
                  Width = 3
                  Height = 13
                end
                object Label1: TLabel
                  Left = 520
                  Top = 328
                  Width = 57
                  Height = 13
                  Caption = #1095#1072#1089#1090#1086#1090#1072', '#1043#1094
                end
                object TrackBar2: TTrackBar
                  Left = 48
                  Top = 352
                  Width = 313
                  Height = 25
                  Max = 50
                  Min = -50
                  Orientation = trHorizontal
                  Frequency = 1
                  Position = 1
                  SelEnd = 0
                  SelStart = 0
                  TabOrder = 0
                  TickMarks = tmTopLeft
                  TickStyle = tsAuto
                  OnChange = TrackBar2Change
                end
              end
            end
          end
        end
      end
      object StringGridFilter: TStringGrid
        Left = 0
        Top = 0
        Width = 241
        Height = 104
        Align = alLeft
        ColCount = 2
        DefaultColWidth = 24
        DefaultRowHeight = 16
        FixedCols = 0
        RowCount = 2
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
        TabOrder = 1
        OnSelectCell = StringGridFilterSelectCell
        ColWidths = (
          118
          96)
      end
      object ButAdd: TBitBtn
        Left = 256
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
        Left = 288
        Top = 4
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
    end
  end
end
