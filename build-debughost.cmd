@echo off
rem Builds the CLR v2 debug adapter twice: bin\OpenMSE.DebugHost.x86.exe and bin\OpenMSE.DebugHost.x64.exe.
rem ICorDebug can only debug processes of its own bitness; the kernel picks the adapter from the target's PE header.
rem Uses the .NET Framework 2.0 SDK headers and libraries shipped in runtime\SDK\v2.0.
setlocal
cd /d "%~dp0"
for /f "usebackq delims=" %%V in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "OPENMSE_VS=%%V"
if not defined OPENMSE_VS exit /b 1
if not exist bin mkdir bin
if not exist build\debughost\x86 mkdir build\debughost\x86
if not exist build\debughost\x64 mkdir build\debughost\x64
set "SOURCES=src\debug\DebugHost.cpp src\core\json.cpp src\core\text.cpp"
set "FLAGS=/nologo /std:c++17 /permissive- /utf-8 /W4 /WX /wd4091 /EHsc /MT /O2 /DUNICODE /D_UNICODE /Iinclude /Iruntime\SDK\v2.0\include"
cmd /c "call "%OPENMSE_VS%\VC\Auxiliary\Build\vcvarsall.bat" x86 >nul && cl %FLAGS% %SOURCES% /Fobuild\debughost\x86\ /Fe:bin\OpenMSE.DebugHost.x86.exe /link /LIBPATH:runtime\SDK\v2.0\lib"
if errorlevel 1 exit /b 1
cmd /c "call "%OPENMSE_VS%\VC\Auxiliary\Build\vcvarsall.bat" x64 >nul && cl %FLAGS% %SOURCES% /Fobuild\debughost\x64\ /Fe:bin\OpenMSE.DebugHost.x64.exe /link /LIBPATH:runtime\SDK\v2.0\lib\AMD64"
if errorlevel 1 exit /b 1
rem The kernel looks for adapters next to OpenMSE.Core.dll; build\ holds the copy used by the kernel tests.
copy /y bin\OpenMSE.DebugHost.x86.exe build\ >nul
copy /y bin\OpenMSE.DebugHost.x64.exe build\ >nul
exit /b %errorlevel%
