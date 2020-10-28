# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
- Bug Fix: QSTRING_TO_CHAR_PTR is no longer called during initialization of Python extension modules

## [1.0.1] - 2020-09-23
### Added
 - GtLogging is implemented as a C++ Python extension
 - 
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
