echo off

set ROOT=EasyGraph
set FBASE=C:\DEVELOP\Cplusplus\%ROOT%
set SOURCERCDIR=%FBASE%\%ROOT%_DEU
set SOURCERCFILE=%SOURCERCDIR%\%ROOT%_DEU.rc
set SOURCENOTFOUNDFILE=%SOURCERCDIR%\NotFound.txt
set SOURCEEXCLUDEFILE=%SOURCERCDIR%\exclude.txt
set CSVBASE=C:\DEVELOP\Cplusplus\EasyControl\Language\EasyControlCSV.csv



set i=0

:LOOP
  if %i%==0 (
    set EXT=ENG
    set i=1
  ) ELSE if %i%==1 (
    set EXT=FRZ
    set i=2
  ) ELSE if %i%==2 (
    set EXT=ITA
    set i=3
  ) ELSE if %i%==3 (
    set EXT=NL
    set i=4
  ) ELSE if %i%==4 (
	set EXT=TRK
    set i=5
  ) ELSE if %i%==5 (
	set EXT=PLK
    set i=6
  ) ELSE if %i%==6 (
	set EXT=ESP
    set i=7
  ) ELSE if %i%==7 (
	set EXT=RUS
    set i=8
  ) ELSE if %i%==8 (
	set EXT=CHS
    set i=9
  ) ELSE if %i%==9 (
	set EXT=UKR
    set i=10
  ) ELSE if %i%==10 (
	set EXT=HUN
    set i=11
  ) ELSE if %i%==11 (
	set EXT=ELL
    set i=12
  ) ELSE (
	goto:ENDLOOP
  )
  set DESTRCDIR=%FBASE%\%ROOT%_%EXT%
  set DESTRCFILE=%DESTRCDIR%\%ROOT%_%EXT%.rc
  set DESTRCAFILENAME=%ROOT%_%EXT%.rca
  set DESTRCAFILE=%DESTRCDIR%\%DESTRCAFILENAME%
  LangConverter %SOURCERCFILE% %DESTRCFILE% %CSVBASE% %EXT% %SOURCEEXCLUDEFILE% 
  ren %DESTRCFILE% %DESTRCAFILENAME%
  CONVERTCP 65001 1200 /i %DESTRCAFILE% /o %DESTRCFILE%
  del %DESTRCAFILE%
  xcopy  /Y %SOURCERCDIR%\resource.h  %DESTRCDIR%
  goto:LOOP

:ENDLOOP

echo on

pause
