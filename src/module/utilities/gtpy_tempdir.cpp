/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_tempdir.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 12.09.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QCoreApplication>
#include <QThread>

#include "gtpy_tempdir.h"

namespace {

QStringList
fileInfoListToStringList(const QFileInfoList& infoList)
{
    QStringList fileList;

    for (const auto& info : infoList)
    {
        fileList << info.absoluteFilePath();
    }

    return fileList;
}

QString
currentThreadId()
{
    return QString::number((long long)QThread::currentThreadId(), 10);
}

}


QString
gtpy::tempdir::threadTempPath(const QString& threadId)
{
    return threadTempDir(threadId).absolutePath();
}

QDir
gtpy::tempdir::threadTempDir(const QString& threadId)
{
    return QDir(QString("%1/pid_%2/%3")
                .arg(QDir::tempPath())
                .arg(QCoreApplication::applicationPid())
                .arg(threadId));
}

QStringList
gtpy::tempdir::threadTempDirEntries(const QString& threadId,
                                    QDir::Filters filters)
{
    return fileInfoListToStringList(
                threadTempDir(threadId).entryInfoList(filters));
}

QString
gtpy::tempdir::currentThreadTempPath()
{
    return threadTempPath(currentThreadId());
}

QDir
gtpy::tempdir::currentThreadTempDir()
{
    return threadTempDir(currentThreadId());
}

QStringList
gtpy::tempdir::currentThreadTempDirEntries(QDir::Filters filters)
{
    return fileInfoListToStringList(
                currentThreadTempDir().entryInfoList(filters));
}