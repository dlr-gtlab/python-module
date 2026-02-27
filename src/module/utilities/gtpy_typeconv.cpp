/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_typeconv.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 German Aerospace Center (DLR)
 *
 * Created on: 27.02.2026
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include <QMetaType>
#include <QVariant>
#include <QHash>

#include <PythonQtConversion.h>

#include "gtpy_contextmanager.h"
#include "gtpypp.h"
#include "gtpy_typeconv.h"

namespace
{

QHash<int, int>& conversionTargets()
{
    static QHash<int, int> targets;
    return targets;
}

PyObject*
convertMetaTypeViaVariant(const void* inObject, int metaTypeId)
{
    if (!inObject) return PyPPNone().release();

    const auto targetIter = conversionTargets().constFind(metaTypeId);
    if (targetIter == conversionTargets().constEnd())
    {
        return PyPPNone().release();
    }

    const int targetMetaTypeId = targetIter.value();

    void* targetData = QMetaType::create(targetMetaTypeId);
    if (!targetData) return PyPPNone().release();

    const bool ok = QMetaType::convert(
        inObject, metaTypeId, targetData,targetMetaTypeId);

    if (!ok)
    {
        QMetaType::destroy(targetMetaTypeId, targetData);
        return PyPPNone().release();
    }

    QVariant targetValue(targetMetaTypeId, targetData);
    QMetaType::destroy(targetMetaTypeId, targetData);

    return PyPPObject::fromQVariant(targetValue).release();
}

} // namespace

void
gtpy::typeconv::registerBuiltinConverters()
{
    int objectPtrMapId =
        qRegisterMetaType<QMap<int, double>>("QMap<int, double>");

    PythonQtConv::registerMetaTypeToPythonConverter(
        objectPtrMapId, GtpyTypeConversion::convertFromQMapIntDouble);
    PythonQtConv::registerPythonToMetaTypeConverter(
        objectPtrMapId, GtpyTypeConversion::convertToQMapIntDouble);

    objectPtrMapId =
        qRegisterMetaType<QMap<QString, double>>("QMap<QString, double>");

    PythonQtConv::registerMetaTypeToPythonConverter(
        objectPtrMapId, GtpyTypeConversion::convertFromQMapStringDouble);
    PythonQtConv::registerPythonToMetaTypeConverter(
        objectPtrMapId, GtpyTypeConversion::convertToQMapStringDouble);

    objectPtrMapId =
        qRegisterMetaType<QMap<QString, int>>("QMap<QString, int>");

    PythonQtConv::registerMetaTypeToPythonConverter(
        objectPtrMapId, GtpyTypeConversion::convertFromQMapStringInt);
    PythonQtConv::registerPythonToMetaTypeConverter(
        objectPtrMapId, GtpyTypeConversion::convertToQMapStringInt);

    objectPtrMapId =
        qRegisterMetaType<QMap<QString, QString>>("QMap<QString, QString>");

    PythonQtConv::registerMetaTypeToPythonConverter(
        objectPtrMapId, GtpyTypeConversion::convertFromQMapStringQString);
    PythonQtConv::registerPythonToMetaTypeConverter(
        objectPtrMapId, GtpyTypeConversion::convertToQMapStringQString);
}

bool
gtpy::typeconv::mapMetaTypeToPythonTargetType(
    const QString& sourceTypeName, const QString& targetTypeName)
{
    const int sourceMetaTypeId = QMetaType::type(
        sourceTypeName.toLatin1().constData());
    const int targetMetaTypeId = QMetaType::type(
        targetTypeName.toLatin1().constData());

    if (sourceMetaTypeId == QMetaType::UnknownType ||
        targetMetaTypeId == QMetaType::UnknownType)
    {
        return false;
    }

    conversionTargets().insert(sourceMetaTypeId, targetMetaTypeId);

    PythonQtConv::registerMetaTypeToPythonConverter(
        sourceMetaTypeId, convertMetaTypeViaVariant);

    return true;
}
