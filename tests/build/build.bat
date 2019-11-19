copy features\gitlab_windows_local_settings.pri local_settings.pri

C:\\Qt\\5.4\\msvc2013_64\\bin\\qmake.exe gtlab_python.pro -r -spec win32-msvc2013 "CONFIG+=release"
call "C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\vcvarsall.bat" amd64
nmake clean
C:\\Qt\\Tools\\QtCreator\\bin\\jom.exe

