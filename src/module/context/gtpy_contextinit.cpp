/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_context_init.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 30.01.2024
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QDebug>

#include "gtpy_contextinit.h"


void
gtpy::context::init::batchContext()
{
    qDebug() << "init batchContext";
}

void
gtpy::context::init::taskRunContext()
{
    qDebug() << "init taskRunContext";
}

std::map<gtpy::context::Type, gtpy::context::init::InitFunc> initRoutineMap = {
    {gtpy::context::BatchContext, gtpy::context::init::batchContext},
    {gtpy::context::TaskRunContext, gtpy::context::init::taskRunContext}
};

gtpy::context::init::InitFunc
gtpy::context::init::routine(gtpy::context::Type type)
{
    return initRoutineMap.at(type);
}



