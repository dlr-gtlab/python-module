SET NIGHTLYBUILDINCL=G:\AT-TW\GTlab\NightlyBuild

REM for /r .\src\ %%f in (*.h) do @copy "%%f" D:\devel\GTlab-NightlyBuild\include\python\

for /r .\src\ %%f in (*.h) do @copy "%%f" %NIGHTLYBUILDINCL%\include\python\
