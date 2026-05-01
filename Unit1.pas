unit Unit1;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.ExtCtrls, Vcl.StdCtrls, CPort,
  VCLTee.TeEngine, VCLTee.Series, VCLTee.TeeProcs, VCLTee.Chart,
  System.Diagnostics;

type
  TForm1 = class(TForm)
    ComPort1: TComPort;
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    ListBox1: TListBox;
    Timer1: TTimer;
    Chart1: TChart;
    Series1: TFastLineSeries;
    Series3: TFastLineSeries;
    Chart2: TChart;
    Series2: TLineSeries;
    Series4: TLineSeries;
    Series6: TPointSeries;
    Series10: TPointSeries;
    Button4: TButton;
    OpenDialog1: TOpenDialog;
    Label1: TLabel;
    Label2: TLabel;
    Series7: TPointSeries;
    Series5: TPointSeries;
    Series9: TPointSeries;
    Series8: TPointSeries;
    Label3: TLabel;
    Label4: TLabel;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure ComPort1RxChar(Sender: TObject; Count: Integer);
    procedure Timer1Timer(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure Button4Click(Sender: TObject);
  private
    Logger: TStreamWriter;
    FLastGState: Double;        // Mengingat status (state) pada siklus sebelumnya (Real-Time)
    FStateSampleCount: Integer; // Menghitung umur status saat ini (Real-Time)
    procedure ParseLine(const Line: string);
  public
  end;

var
  Form1: TForm1;
  NewDataReady: Boolean;
  RxBuffer: string;
  SensorData: array[0..5] of Double;
  AppTimer: TStopwatch;

const
  WINDOW_SECONDS = 15.0;

implementation

{$R *.dfm}

procedure TForm1.Button1Click(Sender: TObject);
begin
  ComPort1.ShowSetupDialog;
end;

procedure TForm1.Button2Click(Sender: TObject);
var
  FileName: string;
begin
  FileName := ExtractFilePath(Application.ExeName) + FormatDateTime('yyyy-mm-dd_hh-nn-ss', Now) + '_SensorData.csv';
  Logger := TStreamWriter.Create(FileName, False, TEncoding.UTF8);
  Logger.WriteLine('Time_Sec,ThighAx,ThighGy,ThighAz');

  ComPort1.FlowControl.ControlDTR := dtrEnable;
  ComPort1.Open;

  AppTimer := TStopwatch.StartNew;

  Timer1.Interval := 10;
  Timer1.Enabled := True;
end;

procedure TForm1.Button3Click(Sender: TObject);
begin
  Timer1.Enabled := False;
  ComPort1.Close;
  AppTimer.Stop;

  if Assigned(Logger) then
  begin
    Logger.Close;
    FreeAndNil(Logger);
  end;
end;

procedure TForm1.Button4Click(Sender: TObject);
var
  SL, LineData: TStringList;
  i: Integer;
  TimeSec, GyL, AngL, GyR, AngR, GState: Double;
  FS: TFormatSettings;
  LastGState: Double;
  StateSampleCount: Integer;
  Marker_N: Integer;
begin
  if OpenDialog1.Execute then
  begin
    Series1.Clear; Series2.Clear; Series3.Clear; Series4.Clear;
    Series5.Clear; Series6.Clear; Series7.Clear; Series8.Clear;
    Series9.Clear; Series10.Clear;

    Chart1.AutoRepaint := False;
    Chart2.AutoRepaint := False;

    Marker_N := 1;      // Plot hanya 1 sampel marker saat berpindah state
    LastGState := -1;   // Reset tracker
    StateSampleCount := 0;

    SL := TStringList.Create;
    LineData := TStringList.Create;
    FS := TFormatSettings.Create('en-US'); // Set koma/titik desimal ke format standar

    try
      SL.LoadFromFile(OpenDialog1.FileName);
      LineData.Delimiter := ',';
      LineData.StrictDelimiter := True;

      for i := 1 to SL.Count - 1 do
      begin
        if Trim(SL[i]) = '' then Continue;
        LineData.DelimitedText := SL[i];

        if LineData.Count >= 6 then
        begin
          // Waktu buatan (sintetis) untuk menghindari efek lumping buffer USB di offline viewer
          TimeSec := i * 0.01;

          GyL     := StrToFloatDef(Trim(LineData[1]), 0.0, FS);
          AngL    := StrToFloatDef(Trim(LineData[2]), 0.0, FS);
          GyR     := StrToFloatDef(Trim(LineData[3]), 0.0, FS);
          AngR    := StrToFloatDef(Trim(LineData[4]), 0.0, FS);
          GState  := StrToFloatDef(Trim(LineData[5]), 0.0, FS);

          Series1.AddXY(TimeSec, GyL);
          Series2.AddXY(TimeSec, AngL);
          Series3.AddXY(TimeSec, GyR);
          Series4.AddXY(TimeSec, AngR);

          // Logika Deteksi Transisi (Hanya mem-plot marker untuk n sampel pertama)
          if GState <> LastGState then
          begin
            LastGState := GState;
            StateSampleCount := 0;
          end;
          Inc(StateSampleCount);

          if (GState = 1) and (StateSampleCount <= Marker_N) then Series10.AddXY(TimeSec, AngR) else Series10.AddXY(TimeSec, 0);
          if (GState = 4) and (StateSampleCount <= Marker_N) then Series6.AddXY(TimeSec, AngL) else Series6.AddXY(TimeSec, 0);

          if (GState = 2) and (StateSampleCount <= Marker_N) then Series5.AddXY(TimeSec, GyL) else Series5.AddXY(TimeSec, 0);
          if (GState = 3) and (StateSampleCount <= Marker_N) then Series8.AddXY(TimeSec, GyL) else Series8.AddXY(TimeSec, 0);
          if (GState = 5) and (StateSampleCount <= Marker_N) then Series7.AddXY(TimeSec, GyR) else Series7.AddXY(TimeSec, 0);
          if (GState = 6) and (StateSampleCount <= Marker_N) then Series9.AddXY(TimeSec, GyR) else Series9.AddXY(TimeSec, 0);
        end;
      end;
    finally
      SL.Free;
      LineData.Free;
      Chart1.AutoRepaint := True;
      Chart2.AutoRepaint := True;
      Chart1.Refresh;
      Chart2.Refresh;
    end;
  end;
end;

procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  if Assigned(Logger) then
  begin
    Logger.Close;
    FreeAndNil(Logger);
  end;
end;

procedure TForm1.ComPort1RxChar(Sender: TObject; Count: Integer);
var
  S: string;
begin
  ComPort1.ReadStr(S, Count);
  RxBuffer := RxBuffer + S;

  while Pos(#10, RxBuffer) > 0 do
  begin
    ParseLine(Copy(RxBuffer, 1, Pos(#10, RxBuffer) - 1));
    Delete(RxBuffer, 1, Pos(#10, RxBuffer));
  end;
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  Timer1.Enabled := False;

  Chart1.BottomAxis.Automatic := True;
  Chart2.BottomAxis.Automatic := True;

  // Inisialisasi awal variabel pelacak
  FLastGState := -1;
  FStateSampleCount := 0;
end;

procedure TForm1.ParseLine(const Line: string);
var
  SL: TStringList;
  FS: TFormatSettings;
  i: Integer;
  Val, CurrentTimeSec: Double;
begin
  if Trim(Line) = '' then Exit;

  FS := TFormatSettings.Create('en-US');
  SL := TStringList.Create;
  try
    SL.Delimiter := #9;
    SL.StrictDelimiter := True;
    SL.DelimitedText := Line;

    if (SL.Count > 0) and TryStrToFloat(Trim(SL[0]), Val, FS) then
    begin
      SensorData[0] := Val;

      // Parsing ke dalam 4 nilai setelahnya agar SensorData[4] menampung gaitState
      for i := 1 to 5 do
      begin
        if i < SL.Count then
          SensorData[i] := StrToFloatDef(Trim(SL[i]), 0.0, FS)
        else
          SensorData[i] := 0.0;
      end;

      NewDataReady := True;

      CurrentTimeSec := AppTimer.ElapsedMilliseconds / 1000.0;

      if Assigned(Logger) then
      begin
        Logger.WriteLine(Format('%.3f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f',
          [CurrentTimeSec, SensorData[0], SensorData[1], SensorData[2], SensorData[3], SensorData[4],  SensorData[5]], FS));
      end;

      ListBox1.Items.Add(FloatToStr(SensorData[4]));
      if ListBox1.Items.Count > 100 then
        ListBox1.Items.Delete(0);
    end;
  finally
    SL.Free;
  end;
end;

procedure TForm1.Timer1Timer(Sender: TObject);
var
  CurrentTimeSec: Double;
  Marker_N: Integer;
begin
  if not NewDataReady then Exit;

  Marker_N := 1; // Plot hanya 1 sampel marker saat berpindah state

  Chart1.AutoRepaint := False;
  Chart2.AutoRepaint := False;
  if SensorData[5] = 0 then Label4.Caption := 'L SWING'
  else if SensorData[5] = 1 then Label4.Caption := 'L STANCE';

  try
    CurrentTimeSec := AppTimer.ElapsedMilliseconds / 1000.0;

    Series1.AddXY(CurrentTimeSec, SensorData[0]);
    Series2.AddXY(CurrentTimeSec, SensorData[1]);
    Series3.AddXY(CurrentTimeSec, SensorData[2]);
    Series4.AddXY(CurrentTimeSec, SensorData[3]);

    // Logika Deteksi Transisi Real-Time
    if SensorData[4] <> FLastGState then
    begin
      FLastGState := SensorData[4];
      FStateSampleCount := 0;
    end;
    Inc(FStateSampleCount);

    // Pembaruan UI Teks
    if SensorData[4] = 1 then Label2.Caption := '1'
    else if SensorData[4] = 2 then Label2.Caption := '2'
    else if SensorData[4] = 3 then Label2.Caption := '3'
    else if SensorData[4] = 4 then Label2.Caption := '4'
    else if SensorData[4] = 5 then Label2.Caption := '5'
    else if SensorData[4] = 6 then Label2.Caption := '6';



    // Plotting Event Marker
    if (SensorData[4] = 1) and (FStateSampleCount <= Marker_N) then Series10.AddXY(CurrentTimeSec, SensorData[3]) else Series10.AddnullXY(CurrentTimeSec, 0);
    if (SensorData[4] = 4) and (FStateSampleCount <= Marker_N) then Series6.AddXY(CurrentTimeSec, SensorData[1]) else Series6.AddXY(CurrentTimeSec, 0);

    if (SensorData[4] = 2) and (FStateSampleCount <= Marker_N) then Series5.AddXY(CurrentTimeSec, SensorData[0]) else Series5.AddXY(CurrentTimeSec, 0);
    if (SensorData[4] = 3) and (FStateSampleCount <= Marker_N) then Series8.AddXY(CurrentTimeSec, SensorData[0]) else Series8.AddXY(CurrentTimeSec, 0);
    if (SensorData[4] = 5) and (FStateSampleCount <= Marker_N) then Series7.AddXY(CurrentTimeSec, SensorData[2]) else Series7.AddXY(CurrentTimeSec, 0);
    if (SensorData[4] = 6) and (FStateSampleCount <= Marker_N) then Series9.AddXY(CurrentTimeSec, SensorData[2]) else Series9.AddXY(CurrentTimeSec, 0);

    // Penghapusan data lama secara independen untuk menghindari error "Out of Bounds"
    while (Series1.Count > 0) and (Series1.XValues[0] < CurrentTimeSec - WINDOW_SECONDS) do Series1.Delete(0);
    while (Series2.Count > 0) and (Series2.XValues[0] < CurrentTimeSec - WINDOW_SECONDS) do Series2.Delete(0);
    while (Series3.Count > 0) and (Series3.XValues[0] < CurrentTimeSec - WINDOW_SECONDS) do Series3.Delete(0);
    while (Series4.Count > 0) and (Series4.XValues[0] < CurrentTimeSec - WINDOW_SECONDS) do Series4.Delete(0);
    while (Series5.Count > 0) and (Series5.XValues[0] < CurrentTimeSec - WINDOW_SECONDS) do Series5.Delete(0);
    while (Series6.Count > 0) and (Series6.XValues[0] < CurrentTimeSec - WINDOW_SECONDS) do Series6.Delete(0);
    while (Series7.Count > 0) and (Series7.XValues[0] < CurrentTimeSec - WINDOW_SECONDS) do Series7.Delete(0);
    while (Series8.Count > 0) and (Series8.XValues[0] < CurrentTimeSec - WINDOW_SECONDS) do Series8.Delete(0);
    while (Series9.Count > 0) and (Series9.XValues[0] < CurrentTimeSec - WINDOW_SECONDS) do Series9.Delete(0);
    while (Series10.Count > 0) and (Series10.XValues[0] < CurrentTimeSec - WINDOW_SECONDS) do Series10.Delete(0);

  finally
    Chart1.AutoRepaint := True;
    Chart2.AutoRepaint := True;
    Chart1.Refresh;
    Chart2.Refresh;
  end;

  NewDataReady := False;
end;

end.
