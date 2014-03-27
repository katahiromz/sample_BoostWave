@setlocal
@echo off
set RET=1
REM --------------------------------------------------------

set ROOT_DIR=%~dp0\..
set CMAKE_DIR=%ROOT_DIR%

set DEFAULT_GENERATOR=Visual Studio 12
set DEFAULT_OUTPUT_DIR_NAME=_build

REM --------------------------------------------------------

REM ---- setup generator
set GENERATOR=%~1
shift
if "%GENERATOR%"=="" (
    set GENERATOR=%DEFAULT_GENERATOR%
)

REM ---- setup output directory name
set SUFFIX=%~1
shift
set OUTPUT_DIR_NAME=%DEFAULT_OUTPUT_DIR_NAME%
if not "%SUFFIX%"=="" (
    set OUTPUT_DIR_NAME=%OUTPUT_DIR_NAME%_%SUFFIX%
)

REM ---- setup extra arguments
set EXTRA_ARGS=%1
:EXTRA_ARG_LOOP_BEGIN
shift
if "%1"=="" goto EXTRA_ARG_LOOP_END
set EXTRA_ARGS=%EXTRA_ARGS% %1
goto EXTRA_ARG_LOOP_BEGIN
:EXTRA_ARG_LOOP_END

REM ---- setup output directory
set OUTPUT_DIR=%ROOT_DIR%\%OUTPUT_DIR_NAME%
if not exist "%OUTPUT_DIR%" ( mkdir "%OUTPUT_DIR%" )

REM ---- output variables
echo GENERATOR  : %GENERATOR%
echo EXTRA_ARGS : %EXTRA_ARGS%
echo OUTPUT_DIR : %OUTPUT_DIR_NAME%
echo.

REM ---- execute cmake
pushd "%OUTPUT_DIR%"
cmake %EXTRA_ARGS% -G "%GENERATOR%" --build "%CMAKE_DIR%"
set RET=%ERRORLEVEL%
popd
if not "%RET%"=="0" pause

REM --------------------------------------------------------

exit /b "%RET%"
