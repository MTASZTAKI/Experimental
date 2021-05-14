OutFile "VLFT_Gamification202.exe"
RequestExecutionLevel admin

InstallDir "c:\VLFT_Gamification\202"
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
	File /r "e:\VLFT-build\bin\assets\" 
	
	SetOutPath $INSTDIR\bin\default_env
	File /r "e:\VLFT-build\bin\default_env\" 
	
	SetOutPath $INSTDIR\bin\release
	File /r "e:\VLFT-build\bin\release\" 
	
	SetOutPath $INSTDIR\samples\virtualLearningFactory
	File /r "e:\VLFT\samples\virtualLearningFactory\"
	
	SetOutPath $INSTDIR\samples
	File "e:\VLFT\samples\virtualLearningFactory.zip"
	
	SetOutPath $INSTDIR\samples
	File "e:\VLFT\samples\virtualLearningFactory.md5"
	
	SetOutPath $INSTDIR\screenshots
	SetOutPath $INSTDIR\screencasts
	SetOutPath $INSTDIR\studentsMovementLog
		
	SetOutPath $DESKTOP\VLFT_Gamification\202
	File "VLFT_Gamification.lnk"
	
SectionEnd