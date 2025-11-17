#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __
#          / /_/ / / / / / /_/ / /_/ /
#         ____/ /_/ /_/\__,_/_.___/
#
######################################################################

add_library(ResourceModule STATIC)

set_property(TARGET ResourceModule PROPERTY AUTOMOC ON)

target_sources(ResourceModule
    PRIVATE

    gt/resource/url.h
    gt/resource/data/directory.h
    gt/resource/data/file.h
    gt/resource/data/httpresource.h
    gt/resource/data/package.h
    gt/resource/data/projectfile.h
    gt/resource/data/streamresource.h
    gt/resource/data/urlresource.h
    gt/resource/data/helper/filecontent.h
    gt/resource/gui/ui/directoryui.h
    gt/resource/gui/ui/fileui.h
    gt/resource/gui/mdi/fileviewer.h
    gt/resource/gui/ui/httpresourceui.h
    gt/resource/gui/ui/packageui.h
    gt/resource/module.h

    gt/resource/data/directory.cpp
    gt/resource/data/file.cpp
    gt/resource/data/httpresource.cpp
    gt/resource/data/package.cpp
    gt/resource/data/projectfile.cpp
    gt/resource/data/streamresource.cpp
    gt/resource/data/urlresource.cpp
    gt/resource/data/helper/filecontent.cpp
    gt/resource/gui/ui/directoryui.cpp
    gt/resource/gui/ui/fileui.cpp
    gt/resource/gui/mdi/fileviewer.cpp
    gt/resource/gui/ui/httpresourceui.cpp
    gt/resource/gui/ui/packageui.cpp
    gt/resource/module.cpp
)

add_library(GTlab::ResourceModule ALIAS ResourceModule)

target_link_libraries(ResourceModule
    PUBLIC
    GTlab::Gui
    PRIVATE
    Qt5::Xml
    Qt5::Widgets
)

target_include_directories(ResourceModule PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_compile_definitions(ResourceModule PRIVATE
    GT_MODULE_ID="Resource Module"
)

