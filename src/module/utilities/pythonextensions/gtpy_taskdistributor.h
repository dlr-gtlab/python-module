/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_taskdistributor.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 30.03.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYTASKDISTRIBUTOR_H
#define GTPYTASKDISTRIBUTOR_H

#include "PythonQt.h"
#include "PythonQtPythonInclude.h"

#include "gt_task.h"

extern PyTypeObject GtpyTaskDistributor_Type;

PyObject* GtpyTaskDistributor_New(PyObject* context);

typedef struct {
    PyObject_HEAD
//    PyObject* m_task;
//    PyObject* m_self;
    PyObject* m_context;
} GtpyTaskDistributor;


#endif // GTPYTASKDISTRIBUTOR_H
