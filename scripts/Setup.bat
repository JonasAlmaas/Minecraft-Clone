@echo off

pushd %~dp0\..\Engine\scripts\python
python setup/main.py
popd

echo.
echo Running premake...
call Win-GenProjects.bat

echo.
echo Setup completed!

PAUSE
