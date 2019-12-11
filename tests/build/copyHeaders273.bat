SET NIGHTLYBUILD=G:\AT-TW\GTlab\Nightly_Builds

REM for /r .\src\ %%f in (*.h) do @copy "%%f" D:\devel\GTlab-NightlyBuild\include\python\
if not exist "%NIGHTLYBUILD%\include\python273\" mkdir %NIGHTLYBUILD%\include\python273\
if not exist "%NIGHTLYBUILD%\lib\python373\" mkdir %NIGHTLYBUILD%\lib\python273\

for /r .\include %%f in (*.h) do @copy "%%f" %NIGHTLYBUILD%\include\python273\
for /r .\lib %%f in (*.dll) do @copy "%%f" %NIGHTLYBUILD%\lib\python273\
for /r .\lib %%f in (*.lib) do @copy "%%f" %NIGHTLYBUILD%\lib\python273\
