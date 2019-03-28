
set path=..\..\tools\bc31\bin;..\..\tools\locate;..\..\tools\sed

copy MAKEFILE_SRC MAKEFILE

..\..\tools\bc31\bin\make
rem copy /B ..\bin\sound.bin + ..\bin\disp.bin ..\bin\8130.bin
copy /B  ..\bin\disp.bin ..\bin\8130.bin
copy /B ..\bin\8130.bin ..\tool\8130.bin
