@REM @echo off
cd ../Engine
call git checkout dev
call git pull

cd ../scripts
call Win-GenProjects.bat

PAUSE
