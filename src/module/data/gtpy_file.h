/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_file.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 28.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */
#ifndef GTPY_FILE_H
#define GTPY_FILE_H

#include <gt/resource/data/projectfile.h>

class GtpyFile : public gt::resource::data::ProjectFile
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit GtpyFile(const QUrl& url = {});
    explicit GtpyFile(const QString& relPath);

};

#endif // GTPY_FILE_H
