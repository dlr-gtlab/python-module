#ifndef GTPYPYTHONCONNECTION_H
#define GTPYPYTHONCONNECTION_H

#include "gt_object.h"

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

private:
    QStringList m_functions;

    GtObjectLinkProperty m_sender;

};

#endif // GTPYPYTHONCONNECTION_H
