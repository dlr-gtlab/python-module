/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_connection.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 04.10.2022
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QDir>
#include <QMetaObject>
#include <QMetaMethod>

#include "gt_application.h"
#include "gt_project.h"

#include "gtpy_contextmanager.h"

#include "gtpy_connection.h"

GtpyConnection::GtpyConnection(GtObject* parent)
    : GtObject(parent),
      m_file{"pyFile", "Python file", "Select Python file",
             QStringList() << "*.py"},
      m_function{"pyFunc", "Python function"},
      m_sender{"SenderObj", "Sender", "Link To Sender", "",
                   this, QStringList() << GT_CLASSNAME(GtObject), true},
      m_signal{"signal", "Signal", "Select a signal"}
{
    registerProperty(m_file);
    registerProperty(m_sender);
    registerProperty(m_function);
    registerProperty(m_signal);

    connect(&m_sender, SIGNAL(changed()), this, SLOT(onSenderChanged()));
    connect(&m_file, SIGNAL(changed()), this, SLOT(onPythonSlotFileChanged()));

    if (GtProject* project = gtApp->currentProject())
    {
        QFile file{project->path() + QDir::separator() + "python" +
                   QDir::separator() + "gt_slots.py"};

        if(file.exists())
        {
             m_file.setValueFromVariant(file.fileName(), "");
        }
    }
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
    gtDebug() << "run commands!";
}

void
GtpyConnection::onSenderChanged()
{
    if (m_linkedObj)
    {
        disconnect(m_linkedObj, SIGNAL(dataChanged(GtObject*)), this, SLOT(runCommands()));
    }
    m_linkedObj = m_sender.linkedObject();

    auto moTest = m_linkedObj->metaObject();

    for(int methodIdx = moTest->methodOffset(); methodIdx < moTest->methodCount(); ++methodIdx) {
      QMetaMethod mmTest = moTest->method(methodIdx);
      switch((int)mmTest.methodType()) {
        case QMetaMethod::Signal:
      {

          qDebug() << mmTest.name();
//          m_signal.registerSubProperty(mode);
//          signalSignatures.append(QString(mmTest.methodSignature())); // Requires Qt 5.0 or newer
        break;
      }

        case QMetaMethod::Slot:
//          slotSignatures.append(QString(mmTest.methodSignature())); // Requires Qt 5.0 or newer
          break;
      }
    }

connect(m_linkedObj, SIGNAL(dataChanged(GtObject*)), this, SLOT(runCommands()));
}

void
GtpyConnection::onPythonSlotFileChanged()
{
    QFileInfo fileInfo{qvariant_cast<QString>(m_file.valueToVariant(""))};

    if (!fileInfo.isFile())
    {
        return;
    }

    auto* python = GtpyContextManager::instance();
    python->addModulePath(fileInfo.path());

    QString pyCode{"import " + fileInfo.baseName() + "\n"};

    //importlib.reload(module) was introduced in Python 3.4.
    if (PY_MAJOR_VERSION >= 3 && PY_MINOR_VERSION >= 4)
    {
        pyCode += "import importlib\n";
        pyCode += "importlib.reload(" + fileInfo.baseName() + ")\n";
    }

    python->evalScript(GtpyContextManager::GlobalContext, pyCode, false);
    auto map = python->introspection(GtpyContextManager::GlobalContext,
                          fileInfo.baseName());
    gtDebug() << map.keys();
}
