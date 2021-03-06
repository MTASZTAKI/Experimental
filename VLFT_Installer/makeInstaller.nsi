OutFile "VLFT_Gamification132.exe"
RequestExecutionLevel admin
!include 'StdUtils.nsh'

InstallDir "c:\VLFT_Gamification\132"
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
	SetOutPath $INSTDIR\bin\release
	File /r "..\..\ApertusVR09-build\bin\release\" 
	
	SetOutPath $INSTDIR\macros\sceneMaker\resources
	File /r "..\..\ApertusVR09\macros\sceneMaker\resources\" 
	
	SetOutPath $INSTDIR\plugins\render\ogreRender\resources
	File /r "..\..\ApertusVR09\plugins\render\ogreRender\resources\" 
	
	SetOutPath $INSTDIR\plugins\languageAPI\jsAPI\nodeJsPlugin\js
	File "..\..\ApertusVR09\plugins\languageAPI\jsAPI\nodeJsPlugin\js\" 
	
	SetOutPath $INSTDIR\plugins\languageAPI\jsAPI\nodeJsPlugin\js\api
	File /r "..\..\ApertusVR09\plugins\languageAPI\jsAPI\nodeJsPlugin\js\api\" 
	
	SetOutPath $INSTDIR\plugins\languageAPI\jsAPI\nodeJsPlugin\js\logs
	File /r "..\..\ApertusVR09\plugins\languageAPI\jsAPI\nodeJsPlugin\js\logs\" 
	
	SetOutPath $INSTDIR\plugins\languageAPI\jsAPI\nodeJsPlugin\js\modules
	File /r "..\..\ApertusVR09\plugins\languageAPI\jsAPI\nodeJsPlugin\js\modules\" 
	
	SetOutPath $INSTDIR\plugins\languageAPI\jsAPI\nodeJsPlugin\js\plugins\virtualLearningFactoryUI
	File /r "..\..\ApertusVR09\plugins\languageAPI\jsAPI\nodeJsPlugin\js\plugins\virtualLearningFactoryUI\" 
	
	SetOutPath $INSTDIR\samples\virtualLearningFactory
	File /r "..\..\ApertusVR09\samples\virtualLearningFactory\"
	
	SetOutPath $INSTDIR\screenshots
	SetOutPath $INSTDIR\screencasts
	SetOutPath $INSTDIR\studentsMovementLog

	SetOutPath $INSTDIR
	File "stringReplacer@config.bat"
	File "stringReplacer@config.ps1"
	ExecWait "$INSTDIR\stringReplacer@config.bat"
	Delete "$INSTDIR\stringReplacer@config.bat"
	Delete "$INSTDIR\stringReplacer@config.ps1"	
		
	SetOutPath $DESKTOP\VLFT_Gamification
	File "VLFT_Gamification.lnk"
	
SectionEnd