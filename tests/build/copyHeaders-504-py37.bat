SET NIGHTLYBUILD=G:\AT-TW\GTlab\Nightly_Builds

REM for /r .\src\ %%f in (*.h) do @copy "%%f" D:\devel\GTlab-NightlyBuild\include\python\
if exist "%NIGHTLYBUILD%\include\python373\" rmdir /s /q %NIGHTLYBUILD%\include\python373\
if not exist "%NIGHTLYBUILD%\include\python373\" mkdir %NIGHTLYBUILD%\include\python373\
if exist "%NIGHTLYBUILD%\lib\python373\" rmdir /s /q %NIGHTLYBUILD%\lib\python373\
if not exist "%NIGHTLYBUILD%\lib\python373\" mkdir %NIGHTLYBUILD%\lib\python373\

for /r .\include %%f in (*.h) do @copy "%%f" %NIGHTLYBUILD%\include\python373\
for /r .\lib %%f in (*.dll) do @copy "%%f" %NIGHTLYBUILD%\lib\python373\
for /r .\lib %%f in (*.lib) do @copy "%%f" %NIGHTLYBUILD%\lib\python373\
