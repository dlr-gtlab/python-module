#ifndef GTPYPYTHONCONNECTION_H
#define GTPYPYTHONCONNECTION_H

#include <QObject>

#include "gt_objectlinkproperty.h"

class GtObject;

class GtpyPythonConnection : public QObject
{
    Q_OBJECT

public:

    Q_INVOKABLE GtpyPythonConnection();

    void registerPythonFunction(QString funcName);

    void registerSender(GtObject* obj);

private slots:
    void runCommands();

private:
    QStringList m_functions;

    GtObjectLinkProperty m_sender;

};

#endif // GTPYPYTHONCONNECTION_H
