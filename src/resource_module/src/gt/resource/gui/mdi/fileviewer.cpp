/* GTlab - Gas Turbine laboratory
 * Source File: textviewer.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 10.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "fileviewer.h"

#include <QUrl>
#include <QMenu>
#include <QTimer>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QPlainTextEdit>

#include <gt_logging.h>

#include <gt_icons.h>
#include <gt_application.h>

#include "gt/resource/data/projectfile.h"

namespace gt
{

namespace resource
{

namespace gui
{

namespace mdi
{

FileViewer::FileViewer()
{
    setObjectName(tr("File Viewer"));

    // text edit
    m_textEdit = new QPlainTextEdit();
    m_textEdit->setReadOnly(true);

    // main layout
    auto* mainLayout = new QHBoxLayout{widget()};
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(m_textEdit);

    widget()->setLayout(mainLayout);
}

QIcon
FileViewer::icon() const
{
    return gt::gui::icon::file();
}

void
FileViewer::setData(GtObject* obj)
{
    m_file  = qobject_cast<gt::resource::data::File*>(obj);
    if (!m_file)
    {
        gtError().verbose() << tr("Not a file resource!") << " ( "
                            << obj << " )";
        return;
    }

    m_textEdit->setPlainText(m_file->readAll());

    auto objName = m_file->objectName();
    auto fileName = m_file->info().fileName();

    setObjectName(objName == fileName ? objName : objName + " - " + fileName);

    connect(obj, &QObject::destroyed, this, &FileViewer::deleteLater);

    if (qobject_cast<gt::resource::data::ProjectFile*>(m_file))
    {
        m_textEdit->setContextMenuPolicy(Qt::CustomContextMenu);

        auto* updateTimer = new QTimer{m_textEdit};
        updateTimer->setSingleShot(true);
        updateTimer->setInterval(300);

        connect(m_textEdit, &QPlainTextEdit::textChanged,
                m_textEdit, [updateTimer]() { updateTimer->start(); });

        connect(updateTimer, &QTimer::timeout, this, [this]() {
            if (m_textEdit->toPlainText().toUtf8() ==
                m_file->readAll()) return;
            auto _ = gtApp->makeCommand(
                m_file, tr("%1 modified").arg(m_file->objectName()));
            QSignalBlocker blocker{m_file};
            m_file->writeAll(m_textEdit->toPlainText().toUtf8());
        });

        connect(m_file, SIGNAL(dataChanged(GtObject*)), this,
                SLOT(updateText()));

        m_textEdit->setReadOnly(false);
    }
}

void
FileViewer::updateText()
{
    QSignalBlocker blocker{m_textEdit};
    m_textEdit->setPlainText(m_file->readAll());
}

} // namespace mdi

} // namespace gui

} // namespace resource

} // namespace gt
