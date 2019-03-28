
c:\windows\system32\xcopy.exe /S /y . ..\fenji_res
del /q ..\fenji_res\*.*
del /q ..\fenji_res\src\*src.*
del /q ..\fenji_res\src\boot\loader.c
del /q ..\fenji_res\src\boot\loader.~c
rd /s /q ..\fenji_res\src\lib_src

