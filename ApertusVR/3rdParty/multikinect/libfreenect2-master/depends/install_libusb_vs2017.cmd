rem This can only be run in a Git Shell or similar environments
rem with access to git.exe and msbuild.exe.

cd libusb_src

set MSBUILD="c:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBuild.exe"

%MSBUILD% msvc\libusb_2017.sln /p:Platform=x64 /p:Configuration=Debug /target:Rebuild || exit /b
%MSBUILD% msvc\libusb_2017.sln /p:Platform=x64 /p:Configuration=Release /target:Rebuild || exit /b

mkdir ..\libusb\include\libusb-1.0
copy libusb\libusb.h ..\libusb\include\libusb-1.0
mkdir ..\libusb\MS64\dll
copy x64\%CONFIG%\dll\*.lib ..\libusb\MS64\dll
copy x64\%CONFIG%\dll\*.dll ..\libusb\MS64\dll
copy x64\%CONFIG%\dll\*.pdb ..\libusb\MS64\dll
