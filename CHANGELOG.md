# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).
    
             
                    
                

## [Unreleased]
### Changed
 - The objects name of the Python process components are used as wizard title. #110
 - The wizards provide a settings dialog. #135
 - It is possible to define the tab size and to replace tabs with spaces. #135

## [1.0.6] - 2021-07-08
### Fixed
- Bug Fix: Importing numpy on Linux machines is possible - #133

### Added
- GtpyDecorator:: add function to get object by uuid -#130
- Disable import check for GtCalculators module to enable better usage in performance module - #131

## [1.0.5] - 2021-03-16
### Fixed
- Bug Fix: replacement of line feed with carriage return for python scripts

## [1.0.4] - 2021-03-10
### Changed
 - The code quality was improved

## [1.0.3] - 2020-12-23
### Added
- GtpyDecorator::className() function added - #120

### Changed
- Module structure adapted to framework version 1.7 - #117
- Add object class filter to GtpyDecorator::findGtChildren -function -#121

## [1.0.2] - 2020-10-29
### Fixed
- Bug Fix: QSTRING_TO_CHAR_PTR is no longer called during initialization of Python extension modules

## [1.0.1] - 2020-09-23
### Added
 - GtLogging is implemented as a C++ Python extension

### Changed
 - user-specific Python path variables are set before the initialization of PythonQt

### Fixed
 - Bug Fix: Memory leak in the initialization of extension modules
 - Remove bug in GtpyGlobals (convert QString safely to char*)

## [1.0.0] - 2020-07-02
### Added
- Changelog file in repository - #98

### Changed
- Migration to new build configuration (Qt12, MSVC 2017 Build Tools)
- PythonScriptCalculator and PythonTask to RELEASE mode - #99
