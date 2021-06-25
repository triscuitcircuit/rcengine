@ECHO OFF
pushd ..\
MD cmake-build
pushd cmake-build
call cmake .. -G "Visual Studio 16 2019"
popd
popd
PAUSE
