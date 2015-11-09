[Code]
#include "services_unicode.iss"

var
  UserPage: TInputQueryWizardPage; 


procedure StartIfExists(SvcName: String; FileName: String);
var
  S: Longword;
  Status: Boolean;
begin
  //If service is installed, it needs to be start
  if ServiceExists(SvcName) then begin
    //MsgBox('Buscando o serviço o serviço ' + svcName, mbInformation, MB_OK);
    S:= SimpleQueryService(SvcName);
    if S = SERVICE_STOPPED then begin
      //MsgBox('Iniciando o serviço ' + svcName, mbInformation, MB_OK);
      SimpleStartService(SvcName, True, True);
    end;
  end else begin
    //Cria o serviço
    Status := SimpleCreateService(
      SvcName,                                    //Nome do serviço
      '',                                         //Display name do serviço
      FileName,  //Arquivo do serviço
      SERVICE_BOOT_START,                         //Tipo de inicialiação
      '',                                         //Usuário
      '',                                         //Senha
      True,                                       //Serviço internativo?
      True                                        //Ignora erro caso já exista
    );
    if Status then begin
      SimpleStartService(SvcName, True, True);
    end;  
  end;

end;

procedure StopIfExists(SvcName: String);
var
  S: Longword;
begin
  if ServiceExists(SvcName) then begin
    //MsgBox('Buscando o serviço o serviço ' + svcName, mbInformation, MB_OK);
    S:= SimpleQueryService(SvcName);
    if S <> SERVICE_STOPPED then begin
      //MsgBox('Parando o serviço ' + svcName, mbInformation, MB_OK);
      SimpleStopService(SvcName, True, True);
    end;
  end;
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if curStep = ssInstall then begin
    { Para os serviços obrigatórios }
    StopIfExists('CheckCacic');
    StopIfExists('CacicDaemon');
  end;
  
  // Inicia o serviço após a instalação
  if curStep = ssDone then begin
    StartIfExists('CacicDaemon', ExpandConstant('{app}\cacic-service.exe'));
  end;
end;

{ Para os serviços obrigatórios }
function InitializeUninstall(): Boolean;
begin  
  StopIfExists('CheckCacic');
  StopIfExists('CacicDaemon');

  Result := True;
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
  if CurUninstallStep = usUninstall then begin
    SimpleDeleteService('CheckCacic');
    SimpleDeleteService('CacicDaemon');
  end;
end;
  
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

  UserPage.Values[0] := GetPreviousData('Host', 'localhost');
  UserPage.Values[1] := GetPreviousData('Usuario', 'cacic');
  UserPage.Values[2] := GetPreviousData('ApiKey', 'cacic123');
  
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