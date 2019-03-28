
set path=..\..\tools\bc31\bin;..\..\tools\locate;..\..\tools\sed

copy MAKEFILE_SRC MAKEFILE

..\..\tools\bc31\bin\make
copy /B  ..\bin\disp.bin ..\bin\door35.bin
copy /B ..\bin\door35.bin ..\tool\door35.bin
