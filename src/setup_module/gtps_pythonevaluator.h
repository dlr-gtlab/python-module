/* GTlab - Gas Turbine laboratory
 * Source File: gtps_pythonevaluator.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 20.10.2022
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPSPYTHONEVALUATOR_H
#define GTPSPYTHONEVALUATOR_H

#include <QString>

class GtpsPythonEvaluator
{
public:
    GtpsPythonEvaluator(const QString& pythonExe);

    QString evaluate(const QString& pythonCommand, bool* ok = nullptr) const;

    bool isValid() const;

    QString pythonVersion() const;

private:
    QString m_pythonExe;

    bool m_isValid{false};
};

#endif // GTPSPYTHONEVALUATOR_H
