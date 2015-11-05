; -- Languages.iss --
; Demonstrates a multilingual installation.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName={cm:MyAppName}
AppId=Cacic
AppVersion=3.2.0
DefaultDirName=C:\{cm:MyAppName}
DefaultGroupName={cm:MyAppName}
UninstallDisplayIcon={app}\install-cacic.exe
VersionInfoDescription=Módulo Agente do Software Cacic
VersionInfoProductName=Cacic Agente
OutputDir=userdocs:Inno Setup Examples Output
; Uncomment the following line to disable the "Select Setup Language"
; dialog and have it rely solely on auto-detection.
;ShowLanguageDialog=no
; If you want all languages to be listed in the "Select Setup Language"
; dialog, even those that can't be displayed in the active code page,
; uncomment the following line. Note: Unicode Inno Setup always displays
; all languages.
;ShowUndisplayableLanguages=yes

; Inserir aqui diretório onde estarão os arquivos binários do Cacic
SourceDir=E:\

; Fecha aplicações listadas installdelete e uninstalldelete
CloseApplications=yes

[Languages]
Name: pt; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"
Name: en; MessagesFile: "compiler:Default.isl"

[Messages]
pt.BeveledLabel=Português Brasileiro
en.BeveledLabel=English

[CustomMessages]
pt.MyDescription=Módulo Agente do Software Cacic
pt.MyAppName=Cacic
pt.MyAppVerName=Cacic %1
en.MyDescription=Cacic Software Agent
en.MyAppName=Cacic
en.MyAppVerName=Cacic %1


[Registry]
Root: HKCU; Subkey: "Software\Lightbase"; Flags: uninsdeletekeyifempty
Root: HKCU; Subkey: "Software\Lightbase\{cm:MyAppName}"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Lightbase\{cm:MyAppName}\Settings"; ValueType: string; ValueName: "password"; ValueData: "{code:GetParams|ApiKey}"
Root: HKCU; Subkey: "Software\Lightbase\{cm:MyAppName}\Settings"; ValueType: string; ValueName: "usuario"; ValueData: "{code:GetParams|Usuario}"
Root: HKCU; Subkey: "Software\Lightbase\{cm:MyAppName}\Settings"; ValueType: string; ValueName: "mainFolder"; ValueData: "{app}"
Root: HKCU; Subkey: "Software\Lightbase\{cm:MyAppName}\Settings"; ValueType: string; ValueName: "applicationUrl"; ValueData: "{code:GetParams|Host}"

; Variáveis de ambiente
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; ValueType: expandsz; ValueName: "Path"; ValueData: "{olddata};{cm:MyAppName}\bin"; Check: NeedsAddPath('{cm:MyAppName}\bin');

[Files]
; DLL's obrigatórias do Qt
Source: "dll\Enginio.dll"; DestDir: "{app}\bin"; Flags: restartreplace 
Source: "dll\icudt52.dll"; DestDir: "{app}\bin"; Flags: restartreplace 
Source: "dll\icuin52.dll"; DestDir: "{app}\bin"; Flags: restartreplace 
Source: "dll\icuuc52.dll"; DestDir: "{app}\bin"; Flags: restartreplace 
Source: "dll\libeay32.dll"; DestDir: "{app}\bin"; Flags: restartreplace 
Source: "dll\libgcc_s_dw2-1.dll"; DestDir: "{app}\bin"; Flags: restartreplace 
Source: "dll\libssl32.dll"; DestDir: "{app}\bin"; Flags: restartreplace 
Source: "dll\libstdc++-6.dll"; DestDir: "{app}\bin"; Flags: restartreplace 
Source: "dll\libwinpthread-1.dll"; DestDir: "{app}\bin"; Flags: restartreplace 
Source: "dll\Qt5Concurrent.dll"; DestDir: "{app}\bin"; Flags: restartreplace 
Source: "dll\Qt5Core.dll"; DestDir: "{app}\bin"; Flags: restartreplace 
Source: "dll\Qt5Declarative.dll"; DestDir: "{app}\bin"; Flags: restartreplace 
Source: "dll\Qt5Designer.dll"; DestDir: "{app}\bin"; Flags: restartreplace 
Source: "dll\Qt5Gui.dll"; DestDir: "{app}\bin"; Flags: restartreplace 
Source: "dll\Qt5Multimedia.dll"; DestDir: "{app}\bin"; Flags: restartreplace 
Source: "dll\Qt5Network.dll"; DestDir: "{app}\bin"; Flags: restartreplace 
Source: "dll\Qt5QuickWidgets.dll"; DestDir: "{app}\bin"; Flags: restartreplace 
Source: "dll\Qt5WebKit.dll"; DestDir: "{app}\bin"; Flags: restartreplace 
Source: "dll\Qt5WebSockets.dll"; DestDir: "{app}\bin"; Flags: restartreplace 
Source: "dll\Qt5Widgets.dll"; DestDir: "{app}\bin"; Flags: restartreplace 
Source: "dll\Qt5WinExtras.dll"; DestDir: "{app}\bin"; Flags: restartreplace 
Source: "dll\qwindows.dll"; DestDir: "{app}\bin"; Flags: restartreplace 
Source: "dll\ssleay32.dll"; DestDir: "{app}\bin"; Flags: restartreplace 

; Copia as dependências
Source: "dependencies\WindowsXP-KB961503-x86-PTB.exe"; DestDir: "{app}\bin"; Flags: deleteafterinstall;
Source: "dependencies\vcredist_x86_2005.exe"; DestDir: "{app}\bin"; Flags: deleteafterinstall;
Source: "dependencies\vcredist_x86_2008.exe"; DestDir: "{app}\bin"; Flags: deleteafterinstall;
Source: "dependencies\vcredist_x86.exe"; DestDir: "{app}\bin"; Flags: deleteafterinstall;
Source: "dependencies\Win32OpenSSL_Light-1_0_1j.exe"; DestDir: "{app}\bin"; Flags: deleteafterinstall;

; Só precisa do install-cacic
Source: "bin\install-cacic.exe"; DestDir: "{app}\bin" 

[Run]
; Instala as dependências
Filename: "{app}\bin\WindowsXP-KB961503-x86-PTB.exe"; Parameters: "/q /norestart"
Filename: "{app}\bin\vcredist_x86_2005.exe"; Parameters: "/Q"
Filename: "{app}\bin\vcredist_x86_2008.exe"; Parameters: "/q /norestart"
Filename: "{app}\bin\vcredist_x86.exe"; Parameters: "/q /norestart"
Filename: "{app}\bin\Win32OpenSSL_Light-1_0_1j.exe"; Parameters: "/q /norestart"

; Instala o Cacic
Filename: "{app}\bin\install-cacic.exe"; Parameters: "{code:GetBatchParams}"

[InstallDelete]
Type: files; Name: "{app}\cacic-service.exe"
Type: files; Name: "{app}\cacicdeploy.exe"
Type: files; Name: "{app}\cacic-ui.exe"
Type: files; Name: "{app}\chksys.exe"
Type: files; Name: "{app}\gercols.exe"
Type: files; Name: "{app}\mapacacic.exe"

[UninstallDelete]
Type: files; Name: "{app}\cacic-service.exe"
Type: files; Name: "{app}\cacicdeploy.exe"
Type: files; Name: "{app}\cacic-ui.exe"
Type: files; Name: "{app}\chksys.exe"
Type: files; Name: "{app}\gercols.exe"
Type: files; Name: "{app}\mapacacic.exe"

; Apaga DLL's
Type: files; Name: "{app}\bin\*"

; Apaga configurações antigas
Type: files; Name: "{app}\getConfig.json"

[Code]
var
  UserPage: TInputQueryWizardPage;  
  
procedure InitializeWizard;
begin
  { Create the pages }
  
  UserPage := CreateInputQueryPage(wpWelcome,
    'Dados de Instalação', 'Configurações do Gerente',
    'Preencha com os dados de instalação informados pelo Módulo Gerente.');
  UserPage.Add('URL do Gerente:', False);
  UserPage.Add('Usuário:', False);  
  UserPage.Add('Chave de API:', False);

  { Set default values, using settings that were stored last time if possible }

  UserPage.Values[0] := GetPreviousData('Host', 'URL do Módulo gerente');
  UserPage.Values[1] := GetPreviousData('Usuario', 'Usuário para instalação dos agentes');
  UserPage.Values[2] := GetPreviousData('ApiKey', 'Chave de API (consultar gerente)');
  
end;

procedure RegisterPreviousData(PreviousDataKey: Integer);
var
  UsageMode: String;
begin
  { Store the settings so we can restore them next time }
  SetPreviousData(PreviousDataKey, 'Host', UserPage.Values[0]);
  SetPreviousData(PreviousDataKey, 'Usuario', UserPage.Values[1]);
  SetPreviousData(PreviousDataKey, 'ApiKey', UserPage.Values[2]);
end;


function UpdateReadyMemo(Space, NewLine, MemoUserInfoInfo, MemoDirInfo, MemoTypeInfo,
  MemoComponentsInfo, MemoGroupInfo, MemoTasksInfo: String): String;
var
  S: String;
begin
  { Fill the 'Ready Memo' with the normal settings and the custom settings }
  S := '';
  S := S + 'Dados para instalação para instalação do Módulo Gerente:' + NewLine;
  S := S + 'URL do Gerente:' + Space + UserPage.Values[0] + NewLine;
  S := S + 'Usuário:' + Space + UserPage.Values[1] + NewLine;
  S := S + 'Chave de API:' + Space + UserPage.Values[2] + NewLine;
  
  S := S + NewLine + NewLine;  
  S := S + MemoDirInfo + NewLine;

  Result := S;
end;

//==================================================================
{ Allows for standard command line parsing assuming a key/value organization }
function GetCommandlineParam (inParam: String):String;
var
  LoopVar : Integer;
  BreakLoop : Boolean;
begin
  { Init the variable to known values }
  LoopVar :=0;
  Result := '';
  BreakLoop := False;

  { Loop through the passed in arry to find the parameter }
  while ( (LoopVar < ParamCount) and
          (not BreakLoop) ) do
  begin
    { Determine if the looked for parameter is the next value }
    if ( (ParamStr(LoopVar) = inParam) and
         ( (LoopVar+1) <= ParamCount )) then
    begin
      { Set the return result equal to the next command line parameter }
      Result := ParamStr(LoopVar+1);

      { Break the loop }
      BreakLoop := True;
    end;

    { Increment the loop variable }
    LoopVar := LoopVar + 1;
  end;
end;

function GetParams(Param: String): String;
begin
  { Return a user value }
  { Could also be split into separate GetUserName and GetUserCompany functions }
  Case Param Of
    'Host': Begin
              { Sempre tenta encontrar os parâmetros através da linha de comando primeiro}
              Result := GetCommandlineParam('HOST');
              if Result = '' then
                Result := UserPage.Values[0];
            End;
    'Usuario':  Begin
                  { Sempre tenta encontrar os parâmetros através da linha de comando primeiro}
                  Result := GetCommandlineParam('USER')
                  if Result = '' then
                    Result := UserPage.Values[1];
                End;
    'ApiKey': Begin
                { Sempre tenta encontrar os parâmetros através da linha de comando primeiro}
                Result := GetCommandlineParam('PASS')
                if Result = '' then
                  Result := UserPage.Values[2];
              End;
  End;

end;

function GetBatchParams(Value: string): string;
var
  Host: String;
  User: String;
  Pass: String;
begin
  Result := ' -host=' + GetParams('Host') + ' -user=' + GetParams('Usuario') + ' -pass=' + GetParams('ApiKey');
end;

procedure Install(Module: string);
var
  ResultCode: Integer;
begin
  if not Exec(ExpandConstant('{app}\bin\') + Module, '/q /norestart', '', SW_SHOW, ewWaitUntilTerminated, ResultCode) then
  begin
    // you can interact with the user that the installation failed
    MsgBox('A instalação do módulo ' + Module + ' falhou com o código: ' + IntToStr(ResultCode) + '. Mensagem: ' + SysErrorMessage(ResultCode) ,
      mbError, MB_OK);
  end;
end;

function NeedsAddPath(Param: string): boolean;
var
  OrigPath: string;
begin
  if not RegQueryStringValue(HKEY_LOCAL_MACHINE,
    'SYSTEM\CurrentControlSet\Control\Session Manager\Environment',
    'Path', OrigPath)
  then begin
    Result := True;
    exit;
  end;
  // look for the path with leading and trailing semicolon
  // Pos() returns 0 if not found
  Result := Pos(';' + Param + ';', ';' + OrigPath + ';') = 0;
end;
