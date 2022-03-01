# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.3.3] - 2023-05-03
### Changed
 - Task wizard page and script editor exported - #231

## [1.3.2] - 2023-04-26
### Fixed
 - shared_function(module_id: str, function_id: str) ends with an error if shared function does not exist  - #226
 - code vulnerabilities fixed  - #229

## [1.3.1] - 2023-04-04
### Added
 - 'Don't ask me again' checkbox added to Python setup warning
 - Shared functions are accessible via shared_function(module_id: str, function_id: str) in Python

### Changed
 - Icons of GTlab 2.0 are used

### Fixed
- Using packages containing spaces in its name via Python Task enabled - #198

## [1.3.0] - 2023-02-09
### Fixed
- Python Module prepared for Core version 2-0-0-Alpha6 - #218
- several bug fixes - #155, #191, #215

## [1.2.2] - 2023-01-24
### Fixed
 - Python Module does no longer crash while loading on Linux
 - Python Setup Module does no longer show an error dialog in batch mode
 - Python executable is selectable in file chooser dialog on Linux

## [1.2.1] - 2022-12-20
### Fixed
 - Process Model is shown in GtTask wizard
 - Python Setup Module sets the GTlab PYTHONHOME environment variable
 
## [1.2.0] - 2022-12-06
### Added
 - Python Setup Module was created, which together with the Python module represents a module bundle.
 - The Python Setup Module checks if the specified Python environment is valid and offers the possibility to specify another environment via the GTlab preferences dialog.
 - The Python Setup Module suppresses the Python Module if the Python environment is not valid.
 - Different types of Python environments (e.g. venv, conda env, ...) are supported.
 - Since GTlab Core version 2.0.0 the Python module supports Python 3.9

### Changed
 - The Python Module no longer crashes GTlab if the Python environment is not valid.
 
## [1.1.0] - 2022-04-22
### Changed
 - Compatibility to GTlab core version 2.0.0 #167

## [1.0.8] - 2021-11-29

### Added
- Decorator: implement parent getter function -#160

### Fixed
 - GtVersionNumber is used to define the version number of the Python Task (framework version >= 2.0.0) 


## [1.0.7] - 2021-10-18
### Fixed
 - Bug Fix: The correct indentation is used in the automatically generated Python code. #142

### Added
 - The objects name of the Python process components are used as wizard title. #140
 - The wizards provide a settings dialog. #141
 - It is possible to define the tab size and to replace tabs with spaces. #141
 - Provides a Python Script Collection. #124

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
