@echo off
rem Builds the x64 OpenMSE.Core kernel DLL into bin\ and its ABI test into build\.
rem Run build\core-tests.exe <empty directory> afterwards to execute the kernel tests.
setlocal
cd /d "%~dp0"
for /f "usebackq delims=" %%V in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "OPENMSE_VS=%%V"
if not defined OPENMSE_VS exit /b 1
call "%OPENMSE_VS%\VC\Auxiliary\Build\vcvarsall.bat" x64 >nul
if errorlevel 1 exit /b 1
if not exist build\core mkdir build\core
rem OPENMSE_OUT builds elsewhere, e.g. while a running OpenMSE keeps bin\OpenMSE.Core.dll open.
if not defined OPENMSE_OUT set "OPENMSE_OUT=bin"
if not exist "%OPENMSE_OUT%" mkdir "%OPENMSE_OUT%"
set "FLAGS=/nologo /std:c++17 /permissive- /utf-8 /W4 /WX /EHsc /MT /O2 /DUNICODE /D_UNICODE /Iinclude"
cl %FLAGS% /LD src\core\abi.cpp src\core\documents.cpp src\core\text.cpp src\core\json.cpp src\core\design.cpp src\core\xml.cpp src\core\process.cpp src\core\workspace.cpp src\core\tasks.cpp src\core\hosts.cpp src\core\debug.cpp /Fobuild\core\ /link /OUT:"%OPENMSE_OUT%\OpenMSE.Core.dll" /IMPLIB:build\core\OpenMSE.Core.lib
if errorlevel 1 exit /b 1
cl %FLAGS% tests\kernel\core_tests.cpp /Fobuild\core\ /Fe:build\core-tests.exe /link build\core\OpenMSE.Core.lib shell32.lib
if errorlevel 1 exit /b 1
cl %FLAGS% tests\kernel\host_tests.cpp /Fobuild\core\ /Fe:build\host-tests.exe /link build\core\OpenMSE.Core.lib shell32.lib
if errorlevel 1 exit /b 1
copy /y "%OPENMSE_OUT%\OpenMSE.Core.dll" build\OpenMSE.Core.dll >nul
exit /b %errorlevel%
