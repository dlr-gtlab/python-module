/* GTlab - Gas Turbine laboratory
 * Source File: gtpycalculatorfactory.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYCALCULATORFACTORY_H
#define GTPYCALCULATORFACTORY_H

#include <QObject>

class GtTask;
class GtCalculator;

class GtpyCalculatorFactory: public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE GtpyCalculatorFactory(QObject* parent = Q_NULLPTR);

    Q_INVOKABLE GtCalculator* createCalculator(const QString& className,
                                               const QString& objName,
                                               GtTask* parent);
};

#endif // GTPYCALCULATORFACTORY_H
