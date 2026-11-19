@echo off
setlocal
cd /d "%~dp0"
if not exist build mkdir build
if not exist logs mkdir logs
for /f "usebackq delims=" %%V in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "OPENMSE_VS=%%V"
if not defined OPENMSE_VS exit /b 1
call "%OPENMSE_VS%\VC\Auxiliary\Build\vcvarsall.bat" x86 >nul
if errorlevel 1 exit /b 1
if /i "%~1"=="--launcher-only" goto launcher
set "DETOURS_TARGET_PROCESSOR=X86"
pushd vendor\Detours\src
nmake /nologo >"%~dp0logs\detours-build.log" 2>&1
if errorlevel 1 exit /b 1
popd
cl /nologo /std:c++17 /EHsc /MT /O2 /DUNICODE /D_UNICODE /LD src\hook.cpp /Ivendor\Detours\include vendor\Detours\lib.X86\detours.lib advapi32.lib /Fobuild\hook.obj /link /DEF:src\hook.def /OUT:OpenMSEHook.dll /IMPLIB:build\OpenMSEHook.lib
if errorlevel 1 exit /b 1
:launcher
powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -File scripts\build-shell.ps1
if errorlevel 1 exit /b 1
rc /nologo /I . /fo build\OpenMSE.res src\OpenMSE.rc
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /MT /O2 /DUNICODE /D_UNICODE src\launcher.cpp build\OpenMSE.res /Ivendor\Detours\include vendor\Detours\lib.X86\detours.lib advapi32.lib shell32.lib user32.lib /Fobuild\launcher.obj /link /SUBSYSTEM:WINDOWS /OUT:OpenMSE.exe
exit /b %errorlevel%
