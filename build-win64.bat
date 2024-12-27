@echo off

:: Check if SolutionDir is already set and non-empty
if not defined SolutionDir (
    :: Only set SolutionDir if it's not already set
    SET "SolutionDir=%~dp0"
)

:: Ensure the path ends with a backslash
if not "%SolutionDir:~-1%"=="\" SET "SolutionDir=%SolutionDir%\"

cd /d "%SolutionDir%"

call cmake -G "Visual Studio 17 2022" -B "%SolutionDir%\build\x64\Release" -A x64 -DCMAKE_INSTALL_PREFIX="%SolutionDir%install\x64\Release" -DENABLE_AJA=FALSE -DENABLE_DECKLINK=FALSE 

call cmake --build "%SolutionDir%build\x64\Release" --config Release --target install