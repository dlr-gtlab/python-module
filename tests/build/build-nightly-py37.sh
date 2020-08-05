cp features/gitlab_linux_local_settings-nightly37.pri local_settings.pri

/home/schm_j13/Qt/5.4/gcc_64/bin/qmake gtlab_python.pro -spec linux-g++ CONFIG+=release

make clean

make
