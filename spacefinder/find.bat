@echo off
%~d0
cd %~dp0
if "%~1" == "" (
    @echo �뽫Ҫ���ҵ��ļ���ק����bat�ļ���
    pause
    goto :eof
)
 ".\spacefinder.exe" "%~1" 0x1000
pause