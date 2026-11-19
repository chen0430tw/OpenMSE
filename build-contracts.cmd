@echo off
setlocal
cd /d "%~dp0"
for /f "usebackq delims=" %%V in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "OPENMSE_VS=%%V"
if not defined OPENMSE_VS exit /b 1
call "%OPENMSE_VS%\VC\Auxiliary\Build\vcvarsall.bat" x64 >nul
if errorlevel 1 exit /b 1
if not exist build mkdir build
cl /nologo /std:c++17 /permissive- /W4 /WX /EHsc /c /Iinclude tests\kernel\contracts_compile.cpp /Fobuild\kernel-contracts.obj
exit /b %errorlevel%
