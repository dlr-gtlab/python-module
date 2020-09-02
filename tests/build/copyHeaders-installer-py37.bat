SET SERVERPATH=C:\deployment\de.dlr.gtlab.dev.stable.1.6.modules.python37\%CI_COMMIT_TAG%

if exist "%SERVERPATH%\include\python37\" rmdir /s /q %SERVERPATH%\include\python37\
if not exist "%SERVERPATH%\include\python37\" mkdir %SERVERPATH%\include\python37\
if exist "%SERVERPATH%\lib\python37\" rmdir /s /q %SERVERPATH%\lib\python37\
if not exist "%SERVERPATH%\lib\python37\" mkdir %SERVERPATH%\lib\python37\

if not exist "%SERVERPATH%\bin\modules\" mkdir %SERVERPATH%\bin\modules\
if not exist "%SERVERPATH%\binDebug\modules\" mkdir %SERVERPATH%\binDebug\modules\

for /r .\include %%f in (*.h) do @copy "%%f" %SERVERPATH%\include\python37\
for /r .\lib %%f in (*.lib) do @copy "%%f" %SERVERPATH%\lib\python37\
for /r .\lib %%f in (*-d.dll) do @copy "%%f" %SERVERPATH%\binDebug\modules\
for /r .\lib %%i in (*.dll) do (echo %%i | findstr /r ".*-d.dll" > nul || copy %%i %SERVERPATH%\bin\modules\)

dir %SERVERPATH%\include\python37
dir %SERVERPATH%\lib\python37
dir %SERVERPATH%\binDebug\modules
dir %SERVERPATH%\bin\modules\

