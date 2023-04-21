# define name of installer
OutFile "WorkTimer_Installer.exe"
 
# define installation directory
InstallDir $PROGRAMFILES64\WorkTimer
 
# For removing Start Menu shortcut in Windows 7
RequestExecutionLevel admin #user
 
# start default section
Section
	
	#IfFileExists "$INSTDIR\uninstall.exe" 0 +2
	#	ExecWait '"$INSTDIR\uninstall.exe"'
    # set the installation directory as the destination for the following actions
    SetOutPath $INSTDIR
 
    # create the uninstaller
    WriteUninstaller "$INSTDIR\uninstall.exe"
 
    # create a shortcut named "new shortcut" in the start menu programs directory
    # point the new shortcut at the program uninstaller
	#---
	CreateDirectory "$SMPROGRAMS\WorkTimer"
	#---
    CreateShortCut "$SMPROGRAMS\WorkTimer\WorkTimer_Uninstall.lnk" "$INSTDIR\uninstall.exe"
	#---
	SetOutPath "$INSTDIR\Bin"
	File /r ..\x64\Release\*.exe
	File /r ..\x64\Release\*.dll
	#File settings.ini
	#IfFileExists "$EXEDIR\settings.ini" 0 +2
	#	CopyFiles "$EXEDIR\settings.ini" "$INSTDIR\Bin"
	
	#SetOutPath "$INSTDIR\Driver"
	#File /r Driver\*.*
	#SetOutPath "$INSTDIR"
	#---
	;create desktop shortcut
	CreateShortCut "$DESKTOP\WorkTimer.lnk" "$INSTDIR\Bin\WorkTimer.exe" 
 
	;create start-menu items
	CreateShortCut "$SMPROGRAMS\WorkTimer\WorkTimer.lnk" "$INSTDIR\Bin\WorkTimer.exe" 
 
	;create start-up items
    CreateShortCut "$SMSTARTUP\WorkTimer.lnk" "$INSTDIR\Bin\WorkTimer.exe" 
	#---
	#!include WinVer.nsh
	#!include x64.nsh
	# $R0 contains: 95, 98, ME, NT x.x, 2000, XP, 2003, Vista, 7, 8, 8.1, 10 or '' (for unknown)
	#Var /GLOBAL WINVIRPATH
	#
	#${If} ${IsWin10}
    #    StrCpy $WINVIRPATH "Win10"
	#${ElseIf} ${IsWin8.1}
	#	StrCpy $WINVIRPATH "Win81"
	#${ElseIf} ${IsWin8}
	#	StrCpy $WINVIRPATH "Win8"
	#${ElseIf} ${IsWin7}
	#	StrCpy $WINVIRPATH "Win7"
	#${ElseIf} ${IsWinVista}
	#	StrCpy $WINVIRPATH "vista"
	#${Else}
	#	StrCpy $WINVIRPATH "wxp"
	#${EndIf}
	#DetailPrint "$INSTDIR\driver\$WINVIRPATH\x64\cyusb3.inf"
	#${DisableX64FSRedirection}
	#nsExec::ExecToLog '"$SYSDIR\PnPutil.exe" -i -a "$INSTDIR\driver\$WINVIRPATH\x64\cyusb3.inf"'
	#${EnableX64FSRedirection}
	#IfFileExists "$PROGRAMFILES\Cypress\SuperSpeed Explorer Kit\1.0\Updater\SuperSpeedExplorerKitInstaller.exe" 0 +2
	#	ExecWait '"$INSTDIR\driver\SuperSpeedExplorerKitSetup_RevSS.exe"'
		
	#ExecWait '"$INSTDIR\driver\SuperSpeedExplorerKitSetup_RevSS.exe"'
	#Exec '"$INSTDIR\driver\WinPcap_4_1_3.exe"'
	#---
	
SectionEnd
 
# uninstaller section start
Section "uninstall"
 
	#ExecWait '"$INSTDIR\driver\SuperSpeedExplorerKitSetup_RevSS.exe"'
	
    # first, delete the uninstaller
    Delete "$INSTDIR\uninstall.exe"
 
    # second, remove the link from the start menu
    Delete "$SMPROGRAMS\WorkTimer\WorkTimer_Uninstall.lnk"
 
	Delete "$DESKTOP\WorkTimer.lnk"
	
	Delete "$SMPROGRAMS\WorkTimer\WorkTimer.lnk"
	
    Delete "$SMSTARTUP\WorkTimer.lnk"
	
	RMDir /r "$SMPROGRAMS\WorkTimer\"
	
	RMDir /r $INSTDIR
	
# uninstaller section end
SectionEnd