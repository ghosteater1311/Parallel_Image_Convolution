@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"
cd /d "%~dp0\build"
cmake -G "Visual Studio 16 2019" -A x64 ..
cmake --build . --config Release
pause
