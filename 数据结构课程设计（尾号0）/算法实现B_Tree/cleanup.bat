@echo off
echo 正在清理不需要的文件...

cd /d "c:\Users\wangj\Desktop\B-Tree"

echo 删除原始JavaScript B-树文件...
if exist "btree.js" del "btree.js"

echo 删除Visual Studio文件...
if exist "B-Tree.sln" del "B-Tree.sln"

echo 删除原始README文件...
if exist "README.md" del "README.md"

echo 删除临时README文件...
if exist "README_PYTHON.md" del "README_PYTHON.md"

echo 删除API测试文件...
if exist "test_api.html" del "test_api.html"

echo 删除Visual Studio配置文件夹...
if exist ".vs" rmdir /s /q ".vs"

echo 删除Python缓存文件夹...
if exist "__pycache__" rmdir /s /q "__pycache__"

echo 清理完成！

echo 剩余文件列表：
dir /b

pause