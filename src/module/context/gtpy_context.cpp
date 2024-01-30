/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_context.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 30.01.2024
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_context.h"

GtpyContext::GtpyContext(gtpy::context::Type type)
{
    auto initRoutine = gtpy::context::init::routine(type);
    initRoutine();
}
