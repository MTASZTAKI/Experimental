OutFile "VLFT_Gamification204.exe"
RequestExecutionLevel admin

InstallDir "c:\VLFT_Gamification\204"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show
SetCompress auto

Section -VC_redist_x64
  SetOutPath "$TEMP\VLFT_Gamification\VC_redist.x64.exe"
  File "VC_redist.x64.exe"
  ExecWait "$TEMP\VLFT_Gamification\VC_redist.x64.exe"
  Sleep 1000
SectionEnd

Section
    SetOutPath $INSTDIR\bin\assets
	File /r "c:\ApertusVR-build\\bin\assets\" 
	
	SetOutPath $INSTDIR\bin\default_env
	File /r "c:\ApertusVR-build\\bin\default_env\" 
	
	SetOutPath $INSTDIR\bin\release
	File /r "c:\ApertusVR-build\\bin\release\" 
	
	SetOutPath $INSTDIR\samples\virtualLearningFactory
	File /r "c:\ApertusVR\samples\virtualLearningFactory\"
	
	SetOutPath $INSTDIR\samples
	File "c:\ApertusVR\samples\virtualLearningFactory.zip"
	
	SetOutPath $INSTDIR\samples
	File "c:\ApertusVR\samples\virtualLearningFactory.md5"
	
	SetOutPath $INSTDIR\screenshots
	SetOutPath $INSTDIR\screencasts
	SetOutPath $INSTDIR\studentsMovementLog
		
	SetOutPath $DESKTOP\VLFT_Gamification\204
	File "VLFT_Gamification.lnk"
	
SectionEnd