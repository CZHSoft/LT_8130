
set path=..\..\tools\bc31\bin;..\..\tools\locate;..\..\tools\sed

copy MAKEFILE_RES MAKEFILE

..\..\tools\bc31\bin\make
copy /B  ..\bin\disp.bin ..\bin\door35.bin
