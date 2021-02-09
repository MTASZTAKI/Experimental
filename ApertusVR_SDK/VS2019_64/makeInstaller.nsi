OutFile "ApertusVR_SDK_VS2019_64.exe"
InstallDir "$PROGRAMFILES\ApertusVR\0.9.1\SDK\VS2019_64"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show
SetCompress auto
Section -NodeJS_Runtimes
  SetOutPath "$TEMP\ApertusVR\nodejs\win\x64\node-v10.1.0-x64.msi"
  File "$%APERTUSVR_SOURCE%\plugins\languageAPI\jsAPI\3rdParty\nodejs\10.1.0\installer\win\x64\node-v10.1.0-x64.msi"
  ExecWait "$TEMP\ApertusVR\nodejs\win\x64\node-v10.1.0-x64.msi"
  Sleep 1000
SectionEnd
Section
	SetOutPath $INSTDIR\include
	File /r "$%APERTUSVR_SOURCE%\include\" 
	SetOutPath $INSTDIR\macros
	File /r "$%APERTUSVR_SOURCE%\macros\" 
	SetOutPath $INSTDIR\lib
	File /r "$%APERTUSVR_BUILD%\core\" 
	SetOutPath $INSTDIR\lib
	File /r "$%APERTUSVR_BUILD%\macros\" 
	SetOutPath $INSTDIR\bin\release
	File /r "$%APERTUSVR_BUILD%\bin\release\" 
	SetOutPath $INSTDIR\bin\debug
    File /r /x "*.pdb" /x "*.ilk" "$%APERTUSVR_BUILD%\bin\debug\"	
SectionEnd
