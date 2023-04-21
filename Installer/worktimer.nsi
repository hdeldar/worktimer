; WorkTimer.nsi
;
; This script is based on example1.nsi, but it remember the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install WorkTimer.nsi into a directory that the user selects,

;--------------------------------

; The name of the installer
Name "WorkTimer"

; The file to write
OutFile "WorkTimer.exe"

; The default installation directory
InstallDir $PROGRAMFILES\WorkTimer

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\WorkTimer" "Install_Dir"

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
Section "WorkTimer (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  ;File "WorkTimer\platforms\*.*"
  File /r  "WorkTimer\*.*"
  ;
  CreateShortCut "$SMSTARTUP\WorkTimer.lnk" "$INSTDIR\WorkTimer.exe"
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\WorkTimer "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\WorkTimer" "DisplayName" "WorkTimer"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\WorkTimer" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\WorkTimer" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\WorkTimer" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\WorkTimer"
  CreateShortcut "$SMPROGRAMS\WorkTimer\WorkTimer_Uninstall.lnk" "$INSTDIR\uninstall.exe"
  CreateShortcut "$SMPROGRAMS\WorkTimer\WorkTimer.lnk" "$INSTDIR\WorkTimer.exe"
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\WorkTimer"
  DeleteRegKey HKLM SOFTWARE\WorkTimer

  ; Remove files and uninstaller
  ;Delete $INSTDIR\WorkTimer\*.*
  ;Delete $INSTDIR\uninstall.exe

  ; Remove shortcuts, if any
  ;Delete "$SMPROGRAMS\WorkTimer\*.*"
  Delete "$SMSTARTUP\WorkTimer.lnk"
  ; Remove directories used
  RMDir /r "$SMPROGRAMS\WorkTimer"
  RMDir /r "$INSTDIR"

SectionEnd
