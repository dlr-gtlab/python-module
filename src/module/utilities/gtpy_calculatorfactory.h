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

/**
 * @brief The GtpyCalculatorFactory class
 */
class GtpyCalculatorFactory: public QObject
{
    Q_OBJECT

public:
    /**
     * @brief GtpyCalculatorFactory
     * @param parent Parent object.
     */
    Q_INVOKABLE explicit GtpyCalculatorFactory(QObject* parent = Q_NULLPTR);

    /**
     * @brief createCalculator Creates an instance of the calculator class with
     * the given className.
     * @param className Class name of the calculator class.
     * @param objName Object name of the new calculator.
     * @param parent Parent task object.
     * @return Returns a new calculator instance.
     */
    Q_INVOKABLE GtCalculator* createCalculator(const QString& className,
            const QString& objName,
            GtTask* parent);
};

#endif // GTPYCALCULATORFACTORY_H
