@echo off
echo ================================================
echo    关系数据库树结构模拟器 启动脚本
echo ================================================
echo.
echo 正在启动服务器...
echo.

cd /d "%~dp0"
D:\python\python.exe database.py

pause
