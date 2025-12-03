/* GTlab - Gas Turbine laboratory
 * Source File: textviewer.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 10.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef FILEVIEWER_H
#define FILEVIEWER_H

#include <gt_mdiitem.h>

#include "gt/resource/data/file.h"

class QPlainTextEdit;

namespace gt
{

namespace resource
{

namespace gui
{

namespace mdi
{

class FileViewer  : public GtMdiItem
{
    Q_OBJECT

public:
    Q_INVOKABLE FileViewer();

    QIcon icon() const override;

    void setData(GtObject* obj) override;

private:
    QPlainTextEdit* m_textEdit{nullptr};

    QPointer<::gt::resource::data::File> m_file;

private slots:
    void updateText();
};

} // namespace mdi

} // namespace gui

} // namespace resource

} // namespace gt

#endif // FILEVIEWER_H
