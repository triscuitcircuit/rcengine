@ECHO OFF
pushd ..\
MD cmake-build
pushd visual-studio-build
call cmake .. -G "Visual Studio 16 2019"
popd
popd
PAUSE
