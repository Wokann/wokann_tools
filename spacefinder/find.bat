@echo off
%~d0
cd %~dp0
if "%~1" == "" (
    @echo 请将要查找的文件拖拽至本bat文件上
    pause
    goto :eof
)
 ".\spacefinder.exe" "%~1" 0x1000
pause