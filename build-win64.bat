@echo off

:: Check if SolutionDir is already set and non-empty
if not defined SolutionDir (
    :: Only set SolutionDir if it's not already set
    SET "SolutionDir=%~dp0"
)

:: Ensure the path ends with a backslash
if not "%SolutionDir:~-1%"=="\" SET "SolutionDir=%SolutionDir%\"

cd /d "%SolutionDir%"

call cmake -G "Visual Studio 17 2022" -B "%SolutionDir%\build\x64\Release" -A x64 -DCMAKE_INSTALL_PREFIX="%SolutionDir%output\x64\Release" -DENABLE_AJA=FALSE -DENABLE_DECKLINK=FALSE  -DENABLE_COREAUDIO_ENCODER=FALSE -DENABLE_FREETYPE=FALSE -DENABLE_LIBFDK=FALSE -DENABLE_HEVC=TRUE -DENABLE_NVAFX=FALSE -DENABLE_NVENC=FALSE -DENABLE_NVVFX=FALSE -DENABLE_QSV11=FALSE -DENABLE_QSV11=FALSE -DENABLE_RNNOISE=FALSE -DENABLE_SCRIPTING=FALSE -DENABLE_SERVICE_UPDATES=FALSE -DENABLE_SPEEXDSP=FALSE -DENABLE_TEST_INPUT=FALSE -DENABLE_UI=FALSE -DENABLE_VIRTUALCAM=FALSE -DENABLE_VLC=FALSE -DENABLE_VST=FALSE -DENABLE_WEBRTC=FALSE -DENABLE_WEBSOCKET=FALSE -DENABLE_WHATSNEW=FALSE

call cmake --build "%SolutionDir%build\x64\Release" --config Release --target install

pause