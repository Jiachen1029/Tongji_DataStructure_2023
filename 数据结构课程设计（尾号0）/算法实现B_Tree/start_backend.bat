@echo off
chcp 65001 >nul
echo Starting B-Tree Python Backend Service...
echo.
echo Please make sure you have installed Python and required packages:
echo   pip install flask flask-cors
echo.
echo Starting service...
"D:/python/python.exe" app.py
pause
