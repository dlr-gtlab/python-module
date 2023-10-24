/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_icons_compat.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 08.03.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_ICONS_COMPAT_H
#define GTPY_ICONS_COMPAT_H

#include "gt_globals.h"

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)

#include "gt_icons.h"

#define GTPY_ICON(x) gt::gui::icon::x()

#else

#include <QIcon>
#include "gt_application.h"

#define GTPY_ICON(x) gtApp->icon(QStringLiteral(GTPY_PNG_ICON_##x))

#define GTPY_PNG_ICON_add "addIcon_16.png"
#define GTPY_PNG_ICON_calculator "calculatorIcon_16.png"
#define GTPY_PNG_ICON_clear "clearIcon_16.png"
#define GTPY_PNG_ICON_close "closeIcon_16.png"
#define GTPY_PNG_ICON_collection "collectionIcon_16.png"
#define GTPY_PNG_ICON_config "configIcon_16.png"
#define GTPY_PNG_ICON_export_ "exportIcon.png"
#define GTPY_PNG_ICON_file "fileIcon_16.png"
#define GTPY_PNG_ICON_import "importIcon.png"
#define GTPY_PNG_ICON_info "infoIcon_16.png"
#define GTPY_PNG_ICON_info2 "infoBlueIcon_16.png"
#define GTPY_PNG_ICON_layers "stackIcon.png"
#define GTPY_PNG_ICON_list "listIcon_16.png"
#define GTPY_PNG_ICON_play "runProcessIcon_24.png"
#define GTPY_PNG_ICON_plugin "pluginIcon_16.png"
#define GTPY_PNG_ICON_process "processIcon_16.png"
#define GTPY_PNG_ICON_property "propertyIcon_16.png"
#define GTPY_PNG_ICON_python "pythonIcon_16.png"
#define GTPY_PNG_ICON_questionmark "questionIcon_16.png"
#define GTPY_PNG_ICON_remove "closeIcon_16.png"
#define GTPY_PNG_ICON_save "saveProjectIcon_16.png"
#define GTPY_PNG_ICON_stop "stopIcon.png"
#define GTPY_PNG_ICON_triangleSmallLeft "arrowleftIcon.png"
#define GTPY_PNG_ICON_triangleSmallRight "arrowrightIcon.png"
#define GTPY_PNG_ICON_update "updateIcon_16.png"
#define GTPY_PNG_ICON_undo "undoIcon_24.png"
#define GTPY_PNG_ICON_redo "undoIcon_24.png"
#define GTPY_PNG_ICON_swap "reloadIcon.png"

#endif

#endif // GTPY_ICONS_COMPAT_H
