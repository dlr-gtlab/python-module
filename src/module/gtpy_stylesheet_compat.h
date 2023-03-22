/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_stylesheet_compat.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 09.03.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_STYLESHEET_COMPAT_H
#define GTPY_STYLESHEET_COMPAT_H

#include "gt_globals.h"

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)

#include "gt_stylesheets.h"

#define GTPY_STYLESHEET(x) gt::gui::stylesheet::x()

#else

#define GTPY_STYLESHEET(x) GTPY_STYLESHEET_##x

#define GTPY_STYLESHEET_backgroundFrame \
    "#frame {border-image: " \
    "url(:/pixmaps/startup-background.png)}"

#endif

#endif // GTPY_STYLESHEET_COMPAT_H
