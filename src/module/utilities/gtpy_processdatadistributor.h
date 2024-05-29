/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_processdatadistributor.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 29.01.2020
 * Author: Marvin Noethen (AT-TW)
 */

#ifndef GTPY_PROCESSDATADISTRIBUTOR_H
#define GTPY_PROCESSDATADISTRIBUTOR_H

#include <QString>

class GtTask;

class GtpyProcessDataDistributor
{
public:
    explicit GtpyProcessDataDistributor(GtTask* pythonTask);

    GtTask* taskElement(const QString& name);

private:
    GtTask* m_pythonTask;
};

#endif // GTPY_PROCESSDATADISTRIBUTOR_H
