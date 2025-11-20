/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_directory.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 18.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPY_DIRECTORY_H
#define GTPY_DIRECTORY_H

#include <gt/resource/data/projectdirectory.h>

class GtpyDirectory : public gt::resource::data::ProjectDirectory
{
    Q_OBJECT

public:
    Q_INVOKABLE GtpyDirectory();
};

#endif // GTPY_DIRECTORY_H
