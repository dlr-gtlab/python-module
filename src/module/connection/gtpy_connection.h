/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_connection.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 04.10.2022
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYPYTHONCONNECTION_H
#define GTPYPYTHONCONNECTION_H

#include <QPointer>

#include "gt_object.h"

#include "gt_openfilenameproperty.h"
#include "gt_stringproperty.h"
#include "gt_modeproperty.h"
#include "gt_objectlinkproperty.h"

class GtObject;

class GtpyConnection : public GtObject
{
    Q_OBJECT

public:

    explicit GtpyConnection(GtObject* parent = nullptr);

    void registerPythonFunction(QString funcName);

    void registerSender(GtObject* obj);

private slots:
    void runCommands();

    void onSenderChanged();

    void onPythonSlotFileChanged();

private:
    GtOpenFileNameProperty m_file;

    GtStringProperty m_function;

    GtObjectLinkProperty m_sender;

    GtModeProperty m_signal;

    QPointer<GtObject> m_linkedObj{};
};

#endif // GTPYPYTHONCONNECTION_H
