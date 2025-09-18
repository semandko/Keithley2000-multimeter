@echo off
setlocal

rem Укажите путь к папке, где находится ваш исполняемый файл
set TARGET_DIR=C:\k2000\Qt\mult\programm
rem Имя вашего исполняемого файла
set EXECUTABLE=Keithley2000_single_shoot.exe
rem Путь к windeployqt.exe (обновите на свой путь)
set WINDEPLOYQT_PATH=C:\Qt\6.9.2\mingw_64\bin\windeployqt.exe

rem Переход в целевую директорию
cd /d "%TARGET_DIR%"

rem Запуск windeployqt для вашего приложения
echo Deploying Qt libraries for %EXECUTABLE%...
"%WINDEPLOYQT_PATH%" "%EXECUTABLE%"

echo Deployment complete.
pause