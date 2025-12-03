/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_file.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 28.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gtpy_file.h"

#include <gt/resource/url.h>

GtpyFile::GtpyFile(const QUrl& url) : gt::resource::data::ProjectFile(url)
{
    setFlag(GtObject::UserRenamable, true);
}

GtpyFile::GtpyFile(const QString& relPath) :
    GtpyFile(gt::resource::url::fromProjRelPath(relPath)) { }
