OutFile "VLFT_Gamification200.exe"
RequestExecutionLevel admin

InstallDir "c:\VLFT_Gamification\200"
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
	File /r "..\..\VLFT_Gamification-build\bin\assets\" 
	
	SetOutPath $INSTDIR\bin\default_env
	File /r "..\..\VLFT_Gamification-build\bin\default_env\" 
	
	SetOutPath $INSTDIR\bin\release
	File /r "..\..\VLFT_Gamification-build\bin\release\" 
	
	SetOutPath $INSTDIR\samples\virtualLearningFactory
	File /r "..\..\VLFT_Gamification\samples\virtualLearningFactory\"
	
	SetOutPath $INSTDIR\samples
	File "..\..\VLFT_Gamification\samples\virtualLearningFactory.zip"
	
	SetOutPath $INSTDIR\samples
	File "..\..\VLFT_Gamification\samples\virtualLearningFactory.md5"
	
	SetOutPath $INSTDIR\screenshots
	SetOutPath $INSTDIR\screencasts
	SetOutPath $INSTDIR\studentsMovementLog
		
	SetOutPath $DESKTOP\VLFT_Gamification\200
	File "VLFT_Gamification.lnk"
	
SectionEnd