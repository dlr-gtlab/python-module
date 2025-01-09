/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_codegenerator.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 18.11.2019
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gt_application.h"
#include "gt_calculator.h"

#include "gtpy_codegen.h"

#include "gtpy_codegenerator.h"

GtpyCodeGenerator::GtpyCodeGenerator(QObject* parent) : QObject(parent)
{

}

GtpyCodeGenerator*
GtpyCodeGenerator::instance()
{
    static GtpyCodeGenerator* retval = nullptr;

    if (retval == nullptr)
    {
        retval = new GtpyCodeGenerator(gtApp);
    }

    return retval;
}

QString
GtpyCodeGenerator::calculatorPyCode(GtCalculator* calc)
{
    return gtpy::codegen::calcToPyCode(calc);
}

