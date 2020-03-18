OutFile "VLFT_Gamification.exe"
RequestExecutionLevel admin
InstallDir "$PROGRAMFILES\VLFT_Gamification\0.2"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show
SetCompress auto

Section
	SetOutPath $INSTDIR\bin\release
	File "..\..\ApertusVR09-build\bin\release\" 
	
	SetOutPath $INSTDIR\macros\sceneMaker\resources
	File "..\..\ApertusVR09-build\macros\sceneMaker\resources\" 
	
	SetOutPath $INSTDIR\plugins\render\ogreRender\resources
	File "..\..\ApertusVR09-build\plugins\render\ogreRender\resources\" 
	
	SetOutPath $INSTDIR\plugins\languageAPI\jsAPI\nodeJsPlugin\js
	File "..\..\ApertusVR09-build\plugins\languageAPI\jsAPI\nodeJsPlugin\js\" 
	
	SetOutPath $INSTDIR\samples\virtualLearningFactory
	File "..\..\ApertusVR09-build\samples\virtualLearningFactory\"

	SetOutPath $INSTDIR
	File "VLFT_teacher.bat" 

	SetOutPath $INSTDIR
	File "VLFT_student.bat" 

	SetOutPath $INSTDIR
	File "VLFT_local.bat" 	
		
	CreateDirectory "$DESKTOP\VLFT_Gamification"
	CreateShortCut "$DESKTOP\VLFT_Gamification\Local.lnk" "$INSTDIR\VLFT_local.bat" ""
	CreateShortCut "$DESKTOP\VLFT_Gamification\Student.lnk" "$INSTDIR\VLFT_student.bat" ""
	CreateShortCut "$DESKTOP\VLFT_Gamification\Teacher.lnk" "$INSTDIR\VLFT_teacher.bat" ""
	
SectionEnd