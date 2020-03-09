SET NIGHTLYBUILD=G:\AT-TW\GTlab\Nightly_Builds_512

if not exist "%NIGHTLYBUILD%\include\python373\" mkdir %NIGHTLYBUILD%\include\python373\
if not exist "%NIGHTLYBUILD%\lib\python373\" mkdir %NIGHTLYBUILD%\lib\python373\

for /r .\include %%f in (*.h) do @copy "%%f" %NIGHTLYBUILD%\include\python373\
for /r .\lib %%f in (*.dll) do @copy "%%f" %NIGHTLYBUILD%\lib\python373\
for /r .\lib %%f in (*.lib) do @copy "%%f" %NIGHTLYBUILD%\lib\python373\
