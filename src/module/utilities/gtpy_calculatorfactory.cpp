/* GTlab - Gas Turbine laboratory
 * Source File: gtpycalculatorfactory.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 12.08.2019
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "PythonQtPythonInclude.h"

#include "gt_calculatordata.h"
#include "gt_calculator.h"
#include "gt_processfactory.h"
#include "gt_calculatorfactory.h"
#include "gt_task.h"

#include "gtpy_calculatorfactory.h"

GtpyCalculatorFactory::GtpyCalculatorFactory(QObject* parent) :
    QObject(parent)
{

}

GtCalculator*
GtpyCalculatorFactory::createCalculator(const QString& className,
                                        const QString& objName,
                                        GtTask* parent)
{
    GtCalculator* calc = nullptr;

    Py_BEGIN_ALLOW_THREADS

    GtCalculatorData calcData =
        gtCalculatorFactory->calculatorData(className);

    if (!calcData)
    {
        Py_BLOCK_THREADS
        return nullptr;
    }

    // GtCalculatorData::isValid() returns false if the version is null,
    // or if the id or author of a calculator is empty. At this point, it is
    // ok if no author is specified, e.g. this is the case for core calculators
    // like GtExportToMementoCalculator. Therefore, we should remove this
    // check here and only check whether the calculator is instantiable.
    // if (!calcData->isValid())
    // {
    //     Py_BLOCK_THREADS
    //     return nullptr;
    // }

    QObject* newObj = calcData->metaData().newInstance();

    if (!newObj)
    {
        Py_BLOCK_THREADS
        return nullptr;
    }

    calc = qobject_cast<GtCalculator*>(newObj);

    if (!calc)
    {
        Py_BLOCK_THREADS
        delete newObj;
        return nullptr;
    }

    calc->setFactory(gtProcessFactory);

    if (objName.isEmpty())
    {
        calc->setObjectName(calcData->id);
    }
    else
    {
        calc->setObjectName(objName);
    }

    if (parent != nullptr)
    {
        if (calc->thread() != parent->thread())
        {
            calc->moveToThread(parent->thread());
        }

        // ensure appendChild is called in the thread where parent and calc are
        // living to avoid thread affinity issues
        QMetaObject::invokeMethod(parent, [parent, calc](){
            parent->appendChild(calc);
        }, Qt::BlockingQueuedConnection);
    }

    // cppcheck-suppress unknownMacro
    Py_END_ALLOW_THREADS

    return calc;
}
