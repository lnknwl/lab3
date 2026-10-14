@echo off

doxygen Doxyfile
if errorlevel 1 exit /b 1

if not exist docs\html\uml mkdir docs\html\uml
copy /y uml\complex.puml docs\html\uml\complex.puml >nul
if errorlevel 1 exit /b 1
copy /y uml\complex.svg docs\html\uml\complex.svg >nul
if errorlevel 1 exit /b 1
