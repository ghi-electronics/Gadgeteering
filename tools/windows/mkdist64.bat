set distname=%DATE:~6,4%%DATE:~3,2%%DATE:~0,2%

FOR %%A IN (%Date%) DO (
    FOR /F "tokens=1-3 delims=/-" %%B in ("%%~A") DO (
        SET Today=%%D%%B%%C
    )
)

cd ../../

set PATH=%PATH%;%CD%/tools/windows;C:\MinGW\bin
rmdir /s /q dist

cd build\gcc-win64\libgadgeteering
make

cd ..\libgmpGHI
make

cd ..\libgmpSeeed
make

cd ..\..\..\

xcopy /s core\*.h "dist\_BETA_gadgeteering-%today%-win\include\core\"
xcopy /s core\*.hpp "dist\_BETA_gadgeteering-%today%-win\include\core\"
xcopy /s modules\*.h "dist\_BETA_gadgeteering-%today%-win\include\modules\"
xcopy /s mainboards\*.h "dist\_BETA_gadgeteering-%today%-win\include\mainboards\"

xcopy /s lib\windows\*.lib "dist\_BETA_gadgeteering-%today%-win\lib\"

cd tools/windows