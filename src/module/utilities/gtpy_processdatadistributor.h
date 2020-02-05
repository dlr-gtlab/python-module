/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_processdatadistributor.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 29.01.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_PROCESSDATADISTRIBUTOR_H
#define GTPY_PROCESSDATADISTRIBUTOR_H

#include <QString>

class GtpyTask;
class GtTask;

class GtpyProcessDataDistributor
{
public:
    GtpyProcessDataDistributor(GtpyTask* pythonTask);

    GtTask* taskElement(const QString& name);

private:
    GtpyTask* m_pythonTask;
};

#endif // GTPY_PROCESSDATADISTRIBUTOR_H
