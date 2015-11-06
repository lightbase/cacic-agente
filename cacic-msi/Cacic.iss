[Setup]
AppName={cm:MyAppName}
AppId=Cacic
AppVersion=3.2.0
DefaultDirName=C:\{cm:MyAppName}
DefaultGroupName={cm:MyAppName}
;UninstallDisplayIcon={app}\install-cacic.exe
VersionInfoDescription=Módulo Agente do Software Cacic
VersionInfoProductName=Cacic Agente
OutputDir=userdocs:Inno Setup Examples Output
SetupLogging=yes
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
;Name: en; MessagesFile: "compiler:Default.isl"

[Messages]
pt.BeveledLabel=Português Brasileiro
;en.BeveledLabel=English

[CustomMessages]
pt.MyDescription=Módulo Agente do Software Cacic
pt.MyAppName=Cacic
pt.MyAppVerName=Cacic %1
;en.MyDescription=Cacic Software Agent
;en.MyAppName=Cacic
;en.MyAppVerName=Cacic %1


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
Type: files; Name: "{app}\chksys.exe"
Type: files; Name: "{app}\cacic-ui.exe"
Type: files; Name: "{app}\cacic-service.exe"
Type: files; Name: "{app}\cacicdeploy.exe"
Type: files; Name: "{app}\gercols.exe"
Type: files; Name: "{app}\mapacacic.exe"

; Apaga DLL's
Type: filesandordirs; Name: "{app}\bin\"

; Apaga configurações antigas
Type: files; Name: "{app}\getConfig.json"

[UninstallRun]
; A única forma é matar manualmente essa parada
Filename: "{cmd}"; Parameters: "/C ""taskkill /im cacic-ui.exe /f /t"

[UninstallDelete]
Type: files; Name: "{app}\chksys.exe"
Type: files; Name: "{app}\cacic-ui.exe"
Type: files; Name: "{app}\cacic-service.exe"
Type: files; Name: "{app}\cacicdeploy.exe"
Type: files; Name: "{app}\gercols.exe"
Type: files; Name: "{app}\mapacacic.exe"

; Apaga DLL's
Type: filesandordirs; Name: "{app}\bin\"

; Apaga configurações antigas
Type: files; Name: "{app}\getConfig.json"

; Adiciona funções e bibliotecas
#include "cacic_lib.iss"
