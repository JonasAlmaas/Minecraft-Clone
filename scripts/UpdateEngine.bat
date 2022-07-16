@echo off

pushd %~dp0\..\Engine
call git checkout dev
call git pull
popd

echo.
echo Running premake...
call Win-GenProjects.bat

echo.
echo Engine Update completed!

PAUSE
