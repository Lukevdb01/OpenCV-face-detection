@echo off

set "dirNameEx=external"
set "dirNameDp=Dependencies" 
set "dirNameBp=Binaries" 

if not exist "%dirNameEx%" (
    mkdir "%dirNameEx%"
    echo Directory "%dirNameEx%" created.
) else (
    echo Directory "%dirNameEx%" already exists.
)

if not exist "%dirNameDp%" (
    mkdir "%dirNameDp%"
    echo Directory "%dirNameDp%" created.
) else (
    echo Directory "%dirNameDp%" already exists.
)

if not exist "%dirNameBp%" (
    mkdir "%dirNameBp%"
    echo Directory "%dirNameBp%" created.
) else (
    echo Directory "%dirNameBp%" already exists.
)
echo.

echo Make sure to install: https://github.com/opencv/opencv/releases/download/4.9.0/opencv-4.9.0-windows.exe
echo After the installation extract the files to this directory: %dirNameEx%
echo.
echo Press Enter to continue...
pause >nul

echo Copying files from "%dirNameEx"% to "%dirNameDp%" folder
xcopy "%dirNameEx%\opencv\build\include\opencv2" "%dirNameDp%\include\opencv2\" /E /I /Y
xcopy "%dirNameEx%\opencv\build\x64\vc16\lib\opencv_world490.lib" "%dirNameDp%\lib\" /E /I /Y
copy "misc\instructions.txt" "%dirNameBp%\"
copy "%dirNameEx%\opencv\build\x64\vc16\bin\opencv_world490.dll" "%dirNameBp%\"
echo.
echo Choose an IDE to build:
echo vs2022
echo vs2019
echo vs2017
echo vs2015

set /p choice="Enter your choice (e.g., 1 for vs2022): "


:: Get the selected IDE from the array
call premake5 --file=Build.lua %choice%

echo Premake setup completed!
echo.
if exist "%dirNameEx%" (
    rmdir /s /q "%dirNameEx%"
    echo Folder deleted successfully.
) else (
    echo Folder: %dirNameEx% does not exist.
)

:: Pause the script to see the output before closing
pause