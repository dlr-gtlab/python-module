/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_sys.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 10.07.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gt_project.h"

#include "gtpy_gilscope.h"
#include "gtpypp.h"
#include "gtpy_globals.h"

#include "gtpy_utils.h"

void
gtpy::utils::addToSysPath(const QString &path)
{
    GTPY_GIL_SCOPE

    auto pyPath = PyPPSys_GetObject("path");
    if (!pyPath) return;

    const auto pathObj = PyPPObject::fromQString(path);
    bool equal = false;

    for (int i = 0; !equal && i < Py_SIZE(pyPath.get()); i++)
    {
        const auto item = PyPPList_GetItem(pyPath, i);
        equal = PyPPObject_RichCompareBool(item, pathObj, Py_EQ);
    }

    if (!equal)
    {
        PyPPList_Append(pyPath, pathObj);
    }
}

void
gtpy::utils::removeFromSysPath(const QString& path)
{
    GTPY_GIL_SCOPE

    auto pyPath = PyPPSys_GetObject("path");
    if (!pyPath) return;

    const auto pathObj = PyPPObject::fromQString(path);

    for (int i = 0; i < Py_SIZE(pyPath.get()); )
    {
        const auto item = PyPPList_GetItem(pyPath, i);
        if (PyPPObject_RichCompareBool(item, pathObj, Py_EQ))
        {
            PySequence_DelItem(pyPath.get(), i);
            // Don't increment i, as list shrinks
        }
        else
        {
            ++i;
        }
    }
}

QString
gtpy::utils::projectPyScriptsPath(const GtProject* project)
{
    if (!project) return {};

    QDir dir{project->path()};
    auto path = dir.absoluteFilePath(gtpy::constants::PROJ_PY_SCRIPTS_DIR);
    return QDir::toNativeSeparators(path);
}
