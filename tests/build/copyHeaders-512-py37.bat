SET NIGHTLYBUILD=G:\AT-TW\GTlab\Nightly_Builds_512

if exist "%NIGHTLYBUILD%\include\python37\" rmdir /s /q %NIGHTLYBUILD%\include\python37\
if not exist "%NIGHTLYBUILD%\include\python37\" mkdir %NIGHTLYBUILD%\include\python37\
if exist "%NIGHTLYBUILD%\lib\python37\" rmdir /s /q %NIGHTLYBUILD%\lib\python37\
if not exist "%NIGHTLYBUILD%\lib\python37\" mkdir %NIGHTLYBUILD%\lib\python37\

for /r .\include %%f in (*.h) do @copy "%%f" %NIGHTLYBUILD%\include\python37\
for /r .\lib %%f in (*.dll) do @copy "%%f" %NIGHTLYBUILD%\lib\python37\
for /r .\lib %%f in (*.lib) do @copy "%%f" %NIGHTLYBUILD%\lib\python37\
