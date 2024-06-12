/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_stylesheet_compat.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 09.03.2023
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPY_STYLESHEET_COMPAT_H
#define GTPY_STYLESHEET_COMPAT_H

#include "gt_globals.h"
#include "gt_stylesheets.h"

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)

#define GTPY_STYLESHEET(x) gt::gui::stylesheet::x()

#else

#define GTPY_STYLESHEET(x) GTPY_STYLESHEET_##x

#define GTPY_STYLESHEET_backgroundFrame \
    "#frame {border-image: " \
    "url(:/pixmaps/startup-background.png)}"

#define GTPY_STYLESHEET_standardLineEdit GtStyleSheets::standardLineEdit()
#endif

#endif // GTPY_STYLESHEET_COMPAT_H
