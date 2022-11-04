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

#include "gt_versionnumber.h"

class GtpsPythonEvaluator
{
public:
    explicit GtpsPythonEvaluator(const QString& pythonExe);

    bool isValid() const;

    QString pythonExe() const;

    GtVersionNumber pythonVersion() const;

    QString eval(const QString& pythonCommand, bool* ok = nullptr) const;

private:
    QString m_pythonExe{};

    GtVersionNumber m_pyVersion{};

    void setPythonVersion();
};

#endif // GTPSPYTHONEVALUATOR_H
