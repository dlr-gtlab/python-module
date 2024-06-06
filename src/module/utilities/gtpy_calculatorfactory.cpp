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

    //    if (parent != nullptr)
    //    {
    //        QList<GtCalculator*> calculators =
    //                parent->findChildren<GtCalculator*>(objName);

    //        foreach (GtCalculator* calculator, calculators)
    //        {
    //            if (className == QString::fromUtf8(calculator->
    //                                     metaObject()->className()))
    //            {
    //                return calculator;
    //            }
    //        }
    //    }

    GtCalculatorData calcData =
        gtCalculatorFactory->calculatorData(className);

    if (calcData == nullptr)
    {
        return nullptr;
    }

    if (!calcData->isValid())
    {
        return nullptr;
    }

    QObject* newObj = calcData->metaData().newInstance();

    if (newObj == nullptr)
    {
        return nullptr;
    }

    calc = qobject_cast<GtCalculator*>(newObj);

    if (calc == nullptr)
    {
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

        parent->appendChild(calc);
    }

    // cppcheck-suppress unknownMacro
    Py_END_ALLOW_THREADS

    return calc;
}
