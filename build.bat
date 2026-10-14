@echo off
chcp 65001 >nul

if not exist build mkdir build

g++ -std=c++14 -Wall -Wextra -pedantic -finput-charset=UTF-8 -fexec-charset=CP1251 -Iinclude src\*.cpp -o build\lab3.exe

if errorlevel 1 (
    echo Ошибка сборки
    exit /b 1
)

echo Сборка выполнена успешно
