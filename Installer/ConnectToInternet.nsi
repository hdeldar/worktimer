; ConnectToInternet.nsi
;
; This script is based on example1.nsi, but it remember the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install ConnectToInternet.nsi into a directory that the user selects,

;--------------------------------

; The name of the installer
Name "ConnectToInternet"

; The file to write
OutFile "ConnectToInternet.exe"

; The default installation directory
InstallDir $PROGRAMFILES\ConnectToInternet

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\NSIS_ConnectToInternet" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "ConnectToInternet (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  ;File "ConnectToInternet\platforms\*.*"
  File /r  "ConnectToInternet\*.*"
  ;
  CreateShortCut "$SMSTARTUP\ConnectToInternet.lnk" "$INSTDIR\ConnectToInternet.exe"
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\NSIS_ConnectToInternet "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ConnectToInternet" "DisplayName" "NSIS ConnectToInternet"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ConnectToInternet" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ConnectToInternet" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ConnectToInternet" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\ConnectToInternet"
  CreateShortcut "$SMPROGRAMS\ConnectToInternet\Uninstall.lnk" "$INSTDIR\uninstall.exe"
  CreateShortcut "$SMPROGRAMS\ConnectToInternet\ConnectToInternet.lnk" "$INSTDIR\ConnectToInternet.exe"
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ConnectToInternet"
  DeleteRegKey HKLM SOFTWARE\NSIS_ConnectToInternet

  ; Remove files and uninstaller
  ;Delete $INSTDIR\ConnectToInternet\*.*
  ;Delete $INSTDIR\uninstall.exe

  ; Remove shortcuts, if any
  ;Delete "$SMPROGRAMS\ConnectToInternet\*.*"
  Delete "$SMSTARTUP\ConnectToInternet.lnk"
  ; Remove directories used
  RMDir /r "$SMPROGRAMS\ConnectToInternet"
  RMDir /r "$INSTDIR"

SectionEnd
