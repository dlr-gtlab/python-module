/* GTlab - Gas Turbine laboratory
 * Source File: gtpycalculatorfactory.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <PythonQtPythonInclude.h>

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
    GtCalculator* calc = Q_NULLPTR;

    Py_BEGIN_ALLOW_THREADS
    //    if (parent != Q_NULLPTR)
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

    if (calcData == Q_NULLPTR)
    {
        return Q_NULLPTR;
    }

    if (!calcData->isValid())
    {
        return Q_NULLPTR;
    }

    QObject* newObj = calcData->metaData().newInstance();

    if (newObj == Q_NULLPTR)
    {
        return Q_NULLPTR;
    }

    calc = qobject_cast<GtCalculator*>(newObj);

    if (calc == Q_NULLPTR)
    {
        delete newObj;
        return Q_NULLPTR;
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

    if (parent != Q_NULLPTR)
    {
        if (calc->thread() != parent->thread())
        {
            calc->moveToThread(parent->thread());
        }

        parent->appendChild(calc);
    }

    Py_END_ALLOW_THREADS

    return calc;
}
