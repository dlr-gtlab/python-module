# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Fixed
 - Fixed an error, when a subprocess tries to redirect messages to the GTlab console.
   The missing fileno() method of has been implemented.
   Note: the redirection using a file handle is not properly supported!
   Instead, the messages will be printed to a system terminal - #277
 - The Python Task and Calculator wizard no longer increases in height each time it is reopened. - #607

### Added
 - Log messages in Python by passing an argument to the logging functions (e.g. gtDebug('debug message'), gtInfo('info message'), etc.). Logging via the lshift operator is still supported but is no longer recommended (e.g., gtDebug() << 'debug message'). - #595
 - Added envVars() function to access GTlab environment variables from Python code. - #604
 - Python messages are now prefixed with the task / calculator, where they are created.
   This results in a much better usability when debugging the source of potential errors. - #606, #614
 - input_args for Python Tasks and Calculators supports boolean values. - #265
 - The scripts/python subdirectory of the current GTlab project is now automatically added to Python’s sys.path.
   Python modules placed in this directory can be imported via the embedded Python interpreter in GTlab.
   When the current project changes, the previous path is removed from sys.path and the new one is added. - #618
 - Added env_vars() function to access GTlab environment variables from Python code. - #604
 - The Python function `GTlab.openProject` can now be used also with the project path argument.
   This automatically imports a project into the current session, if it is not yet imported.
   Example: `GTlab.openProject('/home/user/myproject/project.gtlab')` - #565

## [1.7.1] - 2025-04-23

### Fixed
 - Fixed python editor silently discard changes, when it is closed.
   Now, the user is asked whether to store these changes or not. - #592
 - Fixed case-dependent python module detection - #601
 - Fixed error, when trying to redirect subprocess message - #277

## [1.7.0] - 2025-04-07

### Fixed
 - Fixed the automated code generation for double and bool properties in the Python Task wizard - #279

### Added
 - Support for multiple python versions other that 3.9. To support multiple versions, the module needs to be 
   compiled for the different python versions and shipped with GTlab. A restart is required after switching the 
   python version. - #587
 - Support for an embedded python distribution, that comes along with GTlab - #585
 - String input values for Python Tasks and Python Calculators now allow special characters - #269

## [1.6.1] - 2024-11-08

### Fixed
 - Fixed inaccessible GTlab packages in the Python Task and the Python Calculator context - #566
 - Fixed setting ObjectLinkProperties of derived objects - #279
 - Fixed crash of findGtTask from external scripts - #568
 - Fixed crash of findGtTask in debug mode - #281
 - Fixed error `TypeError: <module 'gtlab_svg_backend' from ''> is a built-in module` related to the inspect module - #284

## [1.6.0] - 2024-05-03
### Changed
 - Improved the performance of the python console output - #270
 
### Added
 - New global function `footprint` to query the application footprint - #269
 - Clone function for GtObjects - #258
 - Added some decorator functions for easier access of children and property containers of gtobjects - #254, #255

### Fixed
 - Fix of some memory leaks - #261, #264
 - Fix of calls of shared functions with gtobjects as arguments - #256
 - Fixed deadlock for calls of python calculators in the python console - #251

## [1.5.0] - 2023-12-20
### Fixed
 - Deadlock when calling Python Calculator via Python Console - #251
 
### Changed
 - Builds against PythonQt 3.4.2 - #246
 - Changed build system to cmake
 
### Added
 - New script editor widget that provides redo/undo buttons and find and replace functionality. - #240
 - Input and output arguments of any Python task or calculator are accessible from Python scripts. - #249
 
## [1.4.2] - 2023-09-18

### Added
 - matplotlib uses an svg backend that is automatically set when matplotlib is installed in the Python environment. - #234

## [1.4.1] - 2023-09-13

### Fixed
 - Fixed gtFindTask not working anymore - #236

## [1.4.0] - 2023-08-29
### Added
 - Input arguments for the Python process components can be defined as values of a struct container property. In the Python script, the input arguments are available through dictionary input_args. #216
 - The output arguments for the Python process components can be defined as values of a property struct container. After evaluation, the values of output_args are transferred to the output_args property structure container of the Python process component. #216
 - In Python, GtObjects have a new method findGtParent() that returns the parent object. #160
 
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
