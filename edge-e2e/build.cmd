@REM Copyright (c) Microsoft. All rights reserved.
@REM Licensed under the MIT license. See LICENSE file in the project root for full license information.

@setlocal
@echo off

set sdk-root=%~dp0/..
for %%i in ("%sdk-root%") do set sdk-root=%%~fi

if not exist %sdk-root%\CMakeLists.txt (
  echo ERROR: file %sdk-root%\CMakeLists.txt does not exist
  exit /b 1
)

if "%VSCMD_VER%" == "" (
  echo ERROR: this script must be called from a VS build window
  exit /b 1
)

mkdir %~dp0\cmake
pushd %~dp0\cmake
if errorlevel 1 goto :failure

cmake -D BUILD_TESTING=OFF -D use_edge_modules=ON  -D skip_samples=ON -D C_SDK_ROOT=%sdk-root% ..\wrapper
if errorlevel 1 goto :failure

msbuild project.sln /t:edge_e2e_rest_server /p:Configuration=Debug
if errorlevel 1 goto :failure

popd
echo success
exit /b 0

:failure
popd
echo failure
exit /b %ERRORLEVEL%