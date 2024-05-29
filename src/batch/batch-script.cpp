/* GTlab - Gas Turbine laboratory
 * Source File:
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 24.07.2015
 * Author: Stanislaus Reitenbach (AT-TW)
 */

#include <iostream>

#include <QApplication>
#include <QCommandLineParser>
#include <QDomDocument>
#include <QObject>
#include <QDir>

#include "gt_application.h"
#include "gtpy_contextmanager.h"
#include "gt_globals.h"
#include "gt_versionnumber.h"
#include "gt_python.h"

using namespace std;

void
showSplashScreen()
{
    cout << endl;
    cout << "******************************************" << endl;
    cout << "    _____________________      ______  " << endl;
    cout << "    __  ____/__  __/__  /_____ ___  /_ " << endl;
    cout << "    _  / __ __  /  __  /_  __ `/_  __ \\" << endl;
    cout << "    / /_/ / _  /   _  / / /_/ /_  /_/ /" << endl;
    cout << "    \\____/  /_/    /_/  \\__,_/ /_.___/ " << endl;
    cout << "                                 " <<
#if GT_VERSION < GT_VERSION_CHECK(2,0, 0)
         GtApplication::versionToString().toStdString() << endl;
#else
         GtApplication::version().toString().toStdString() << endl;
#endif
    cout << "******************************************" << endl;
    cout << endl;
}

QString
parseScriptFile(const QString& filename)
{
    QFile scriptFile(filename);

    if (!scriptFile.exists())
    {
        cout << "ERROR: script file not found!" << endl;
        return QString();
    }

    if (!scriptFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        cout << "ERROR: could not open script file!" << endl;
        return QString();
    }

    QTextStream out(&scriptFile);

    return out.readAll();
}

int main(int argc, char* argv[])
{
    QCoreApplication::setOrganizationDomain("www.dlr.de");
    QCoreApplication::setOrganizationName("DLR");
    QCoreApplication::setApplicationName("GTlab");

#if GT_VERSION < GT_VERSION_CHECK(2,0, 0)
    QCoreApplication::setApplicationVersion(GtApplication::versionToString());
#else
    QCoreApplication::setApplicationVersion(GtApplication::version().toString());
#endif

    QApplication a(argc, argv);

    showSplashScreen();

    // parse arguments
    QCommandLineParser parser;
    parser.setApplicationDescription("GTlab Console");
    parser.addHelpOption();
    parser.addVersionOption();

    GtApplication app(qApp);
    app.init();

    // save to system environment (temporary)
    app.saveSystemEnvironment();

    QStringList args = qApp->arguments();
    args.removeAt(0);

    return PythonExecution::runPythonInterpreter(args);

}
