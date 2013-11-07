echo off
REM put this in the mainboard subdirectory for your board and run it to create one file for ease of 
REM compilation (only have to specify main_all.cpp rather than all the cpp files
REM individually)
setlocal ENABLEEXTENSIONS
del  /q main_all.cpp main_all.tmp
for %%i in (*.cpp) do (
echo #include "%%i" >>main_all.tmp
)
echo #include "../../../modules/IO60P16/IO60P16.cpp" >>main_all.tmp
rename main_all.tmp main_all.cpp
