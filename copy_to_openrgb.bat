@echo off
echo Copying Lightscape.dll to OpenRGB plugins directory...

:: Define the target path
set TARGET_PATH=C:\Users\wolfi\AppData\Roaming\OpenRGB\plugins\Lightscape.dll

:: Check if the target file is in use (OpenRGB is running)
echo Checking if OpenRGB is using the plugin...
set IS_IN_USE=0

>nul 2>nul copy /y NUL "%TARGET_PATH%" && (
    echo Target file is accessible.
) || (
    echo WARNING: Target file is locked! Please close OpenRGB before building.
    echo.
    set IS_IN_USE=1
    choice /C YN /M "Attempt to proceed anyway? (Y/N)"
    if errorlevel 2 goto :end
    echo Attempting to proceed anyway...
    echo.
)

:: Check if the DLL exists in debug folder
if exist "debug\Lightscape.dll" (
    echo Found DLL in debug folder
    if %IS_IN_USE%==0 (
        copy /y "debug\Lightscape.dll" "%TARGET_PATH%"
        echo Copied debug\Lightscape.dll to OpenRGB plugins folder
    ) else (
        echo Skipping copy due to locked file
    )
    goto :done
)

:: Check if the DLL exists in release folder
if exist "release\Lightscape.dll" (
    echo Found DLL in release folder
    if %IS_IN_USE%==0 (
        copy /y "release\Lightscape.dll" "%TARGET_PATH%"
        echo Copied release\Lightscape.dll to OpenRGB plugins folder
    ) else (
        echo Skipping copy due to locked file
    )
    goto :done
)

:: Check directly in project folder
if exist "Lightscape.dll" (
    echo Found DLL in project folder
    if %IS_IN_USE%==0 (
        copy /y "Lightscape.dll" "%TARGET_PATH%"
        echo Copied Lightscape.dll to OpenRGB plugins folder
    ) else (
        echo Skipping copy due to locked file
    )
    goto :done
)

echo ERROR: Couldn't find Lightscape.dll in any of the expected locations
goto :end

:done
echo Copy operation completed!

:end
pause