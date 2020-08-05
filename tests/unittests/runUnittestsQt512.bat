REM THIS IS A SCRIPT TO PREPARE AND RUN UNITTESTS

SET DEVTOOLS=C:\devel\GTlab-DevTools\stable\1_6

SET PATH=%PATH%;C:\Qt\5.12.6\msvc2017_64\bin;%DEVTOOLS%\ThirdPartyLibraries\PythonQt\PythonQt_37\lib;%DEVTOOLS%\ThirdPartyLibraries\Python\Python_37;%DEVTOOLS%\ThirdPartyLibraries\Python\Python_37\libs;%DEVTOOLS%\bin;build;lib\python37

SET PROGRAMPATH="C:\Program Files\OpenCppCoverage"

%PROGRAMPATH%\OpenCppCoverage.exe --sources src --export_type=html --export_type=cobertura --excluded_modules Qt --excluded_sources %DEVTOOLS%\* -- build\GTlabUnitTest.exe --gtest_output=xml:unittests.xml 
