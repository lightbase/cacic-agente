; -- Languages.iss --
; Demonstrates a multilingual installation.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName={cm:MyAppName}
AppId=Cacic
AppVerName={cm:MyAppVerName,1.5}
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


[Files]
; DLL's obrigatórias do Qt
Source: "bin\Enginio.dll"; DestDir: "{app}\bin"
Source: "bin\icudt52.dll"; DestDir: "{app}\bin"
Source: "bin\icuin52.dll"; DestDir: "{app}\bin"
Source: "bin\icuuc52.dll"; DestDir: "{app}\bin"
Source: "bin\libgcc_s_dw2-1.dll"; DestDir: "{app}\bin"
Source: "bin\libstdc++-6.dll"; DestDir: "{app}\bin"
Source: "bin\libwinpthread-1.dll"; DestDir: "{app}\bin"
Source: "bin\Qt5Concurrent.dll"; DestDir: "{app}\bin"
Source: "bin\Qt5Core.dll"; DestDir: "{app}\bin"
Source: "bin\Qt5Declarative.dll"; DestDir: "{app}\bin"
Source: "bin\Qt5Designer.dll"; DestDir: "{app}\bin"
Source: "bin\Qt5Gui.dll"; DestDir: "{app}\bin"
Source: "bin\Qt5Multimedia.dll"; DestDir: "{app}\bin"
Source: "bin\Qt5Network.dll"; DestDir: "{app}\bin"
Source: "bin\Qt5QuickWidgets.dll"; DestDir: "{app}\bin"
Source: "bin\Qt5WebKit.dll"; DestDir: "{app}\bin"
Source: "bin\Qt5WebSockets.dll"; DestDir: "{app}\bin"
Source: "bin\Qt5Widgets.dll"; DestDir: "{app}\bin"
Source: "bin\Qt5WinExtras.dll"; DestDir: "{app}\bin"
Source: "bin\qwindows.dll"; DestDir: "{app}\bin"

; Só precisa do install-cacic
Source: "install-cacic.exe"; DestDir: "{app}"


[Icons]
Name: "{group}\{cm:MyAppName}"; Filename: "{app}\cacic-logo.png"
Name: "{group}\{cm:UninstallProgram,{cm:MyAppName}}"; Filename: "{uninstallexe}"

[Tasks]
; The following task doesn't do anything and is only meant to show [CustomMessages] usage
Name: mytask; Description: "{cm:MyDescription}"
