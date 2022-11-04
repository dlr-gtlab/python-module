/* GTlab - Gas Turbine laboratory
 * Source File:
 * copyright 2009-2015 by DLR
 *
 *  Created on: 24.07.2015
 *  Author: Stanislaus Reitenbach (AT-TW)
 *  Tel.: +49 2203 601 2907
 */

#include <iostream>

#include <QApplication>
#include <QCommandLineParser>
#include <QDomDocument>
#include <QObject>
#include <QDir>

#include "gt_application.h"
#include "gtpy_contextmanager.h"

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
         GtApplication::versionToString().toStdString() << endl;
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

    QCoreApplication::setApplicationVersion(GtApplication::versionToString());

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

    GtpyContextManager* python = GtpyContextManager::instance();

    if (python)
    {
        python->initContexts();
    }

    QStringList args = qApp->arguments();

    if (args.size() < 2)
    {
        cout << "ERROR: invalid arguments!" << endl;
        return -1;
    }

    QString scriptContent = parseScriptFile(args[1]);

    if (scriptContent.isEmpty())
    {
        cout << "ERROR: empty script file!" << endl;
        return -1;
    }

    if (python)
    {
        python->evalScript(GtpyContextManager::BatchContext, scriptContent,
                           true);
    }

    return 0;
}
