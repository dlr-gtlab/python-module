/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_packageiteration.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 02.03.2023
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPY_PACKAGEITERATION_H
#define GTPY_PACKAGEITERATION_H

#include <QString>

#include "gt_application.h"
#include "gt_project.h"
#include "gt_package.h"

/**
 * Namespace for the package utilities used in the GTlab Python module
 */
namespace gtpy
{
namespace package
{
/**
 * @brief Iterates through the packages and calls the passed function
 * for each package.
 * @param func Function to be called. It gets a package as a parameter.
 */
template <class Func>
void forEachPackage(Func func)
{
    auto* project = gtApp->currentProject();

    if (!project) return;

    auto packages = project->findDirectChildren<GtPackage*>();

    for (auto* pkg : packages)
    {
        assert(pkg != nullptr);

        func(pkg);
    }
}

} // namespace package

} // namespace gtpy

#endif // GTPY_PACKAGEITERATION_H
