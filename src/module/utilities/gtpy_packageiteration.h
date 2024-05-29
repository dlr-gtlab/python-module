/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_packageiteration.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 02.03.2023
 * Author: Marvin Noethen (AT-TW)
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
 * @brief The PackageInfo struct
 * Struct to store information about the GtPackages.
 */
struct PackageInfo
{
    QString modId;
    QString className;
    QString objectName;
};

/**
 * @brief Iterates through the packages and calls the passed function
 * for each package.
 * @param func Function to be called. It gets a package as a parameter.
 */
template <class Func>
void forEachPackage(Func func)
{
    if (auto* project = gtApp->currentProject())
    {
        auto packages = project->findDirectChildren<GtPackage*>();

        for (const auto& modId: project->moduleIds())
        {
            auto pkgClassName = gtApp->modulePackageId(modId);

            if (!pkgClassName.isEmpty())
            {
                auto pkgObj = std::find_if(packages.begin(), packages.end(),
                            [&pkgClassName](GtPackage* p){
                        return p->metaObject()->className() == pkgClassName; });

                func({modId, pkgClassName,(*pkgObj)->objectName()});
            }
        }
    }
}

} // namespace package

} // namespace gtpy

#endif // GTPY_PACKAGEITERATION_H
