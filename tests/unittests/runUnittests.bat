REM THIS IS A SCRIPT TO PREPARE AND RUN UNITTESTS

SET DEVTOOLS=D:\GTlab-DevTools\ThirdPartyLibraries

SET PATH=%PATH%;C:\Qt\5.4\msvc2013_64\bin;%DEVTOOLS%\PythonQt\PythonQt_373\lib;%DEVTOOLS%\Python\Python_373\libs;lib\python373

REM build\MetamodellingUnitTest.exe --gtest_output=xml:unittests.xml 

D:\Programme\OpenCppCoverage\OpenCppCoverage.exe --sources src --export_type=html --export_type=cobertura --excluded_modules Qt --excluded_sources %DEVTOOLS%\* -- build\GTlabUnitTest.exe --gtest_output=xml:unittests.xml 

D:\Programme\GitLab-Runner\BadgeBuilder\BadgeGenerator.exe --cc .\GTlabUnitTestCoverage.xml .
