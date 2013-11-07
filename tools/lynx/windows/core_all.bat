echo off
REM put this in the core directory and run it to create one file for ease of 
REM compilation (only have to specify core_all.cpp rather than all the cpp files
REM individually)
setlocal ENABLEEXTENSIONS
del  /q core_all.cpp core_all.tmp
for %%i in (*.cpp) do (
echo #include "%%i" >>core_all.tmp
)
rename core_all.tmp core_all.cpp
