#include "gtpy_connection.h"

GtpyConnection::GtpyConnection(GtObject* parent)
    : GtObject(parent),
    m_sender{"SenderObj", "Sender", "Link To Sender", "",
                   this, QStringList() << GT_CLASSNAME(GtObject), true}
{

}

void
GtpyConnection::registerPythonFunction(QString funcName)
{

}

void
GtpyConnection::registerSender(GtObject* /*obj*/)
{

}

void
GtpyConnection::runCommands()
{

}
