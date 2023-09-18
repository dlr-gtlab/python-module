/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_tempdir.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 12.09.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_TEMPDIR_H
#define GTPY_TEMPDIR_H

#include "gt_pythonmodule_exports.h"

#include <QString>
#include <QDir>

namespace gtpy {

namespace tempdir {

/**
 * @brief Returns the absolute path of the temporary directory specified by
 * the current process ID and the given thread ID. The path looks like this
 * {system_temp_path}/pid_{process_id}/{thread_id}.
 * @param threadId Thread ID used to create the temporary directory.
 * @return The absolute path of the temporary directory specified by
 * the current process ID and the given thread ID.
 */
GT_PYTHON_EXPORT QString threadTempPath(const QString& threadId);

/**
 * @brief Returns the temporary directory specified by the current
 * process ID and the given thread ID.
 * @param threadId Thread ID used to create the temporary directory.
 * @return The temporary directory specified by the current
 * process ID and the given thread ID.
 */
GT_PYTHON_EXPORT QDir threadTempDir(const QString& threadId);

/**
 * @brief Returns a list with the absolute paths of the entries of the temporary
 * directory specified by the current process ID and the given thread ID.
 * @param threadId Thread ID used to create the temporary directory.
 * @param filters The filters can be used to filter for specific types of
 * entries.
 * @return A list with the absolute paths of the entries of the temporary
 * directory specified by the current process ID and the given thread ID.
 */
GT_PYTHON_EXPORT QStringList threadTempDirEntries(
        const QString& threadId, QDir::Filters filters = QDir::Files);

/**
 * @brief Returns the absolute path of the temporary directory specified by
 * the current process ID and the current thread ID. The path looks like this
 * {system_temp_path}/pid_{process_id}/{thread_id}.
 * @return The absolute path of the temporary directory specified by
 * the current process ID and the current thread ID.
 */
GT_PYTHON_EXPORT QString currentThreadTempPath();

/**
 * @brief Returns the temporary directory specified by the current
 * process ID and the current thread ID.
 * @return The temporary directory specified by the current
 * process ID and the current thread ID.
 */
GT_PYTHON_EXPORT QDir currentThreadTempDir();

/**
 * @brief Returns a list with the absolute paths of the entries of the temporary
 * directory specified by the current process ID and the current thread ID.
 * @param filters The filters can be used to filter for specific types of
 * entries.
 * @return A list with the absolute paths of the entries of the temporary
 * directory specified by the current process ID and the current thread ID.
 */
GT_PYTHON_EXPORT QStringList currentThreadTempDirEntries(
        QDir::Filters filters = QDir::Files);

} // tempdir

} // gtpy

#endif // GTPY_TEMPDIR_H
