cp features/gitlab_linux_local_settings.pri local_settings.pri

/opt/Qt/5.12.5/gcc_64/bin/qmake gtlab_python.pro -spec linux-g++ CONFIG+=release #CONFIG+=qml_debug

make clean

make
