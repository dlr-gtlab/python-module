#include "gtpy_pythonconnection.h"

GtpyPythonConnection::GtpyPythonConnection() :
    m_sender("SenderObj", "Sender", "Link To Sender", "",
                   nullptr, QStringList() << GT_CLASSNAME(GtObject))
{

}

void GtpyPythonConnection::registerPythonFunction(QString funcName)
{

}

void GtpyPythonConnection::registerSender(GtObject* /*obj*/)
{

}

void GtpyPythonConnection::runCommands()
{

}
