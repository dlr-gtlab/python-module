/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_abstractscriptingwizardpage.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

// Qt includes
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCompleter>
#include <QLabel>
#include <QSplitter>
#include <QTabWidget>
#include <QFileDialog>
#include <QTextStream>
#include <QTextOption>
#include <QMetaProperty>
#include <QTabBar>
#include <QThreadPool>

// python includes
#include "gtpy_scripteditor.h"
#include "gtpy_console.h"
#include "gtpy_scriptrunnable.h"

// GTlab framework includes
#include "gt_object.h"
#include "gt_project.h"
#include "gt_package.h"
#include "gt_application.h"
#include "gt_calculatorhelperfactory.h"
#include "gt_modeproperty.h"
#include "gt_objectlinkproperty.h"
#include "gt_datamodel.h"
#include "gt_filedialog.h"

#include "gt_pyhighlighter.h"
#include "gt_searchwidget.h"

#include "gtpy_abstractscriptingwizardpage.h"

GtpyAbstractScriptingWizardPage::GtpyAbstractScriptingWizardPage(
        GtpyContextManager::Context type) :
    m_contextType(type),
    m_editor(Q_NULLPTR),
    m_editorSplitter(Q_NULLPTR),
    m_tabWidget(Q_NULLPTR)
{
    setTitle(tr("Python Script Editor"));

    QVBoxLayout* layout = new QVBoxLayout;

    QSplitter* splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Vertical);

    m_editorSplitter = new QSplitter(this);
    m_editorSplitter->setOrientation(Qt::Horizontal);

    m_editor = new GtpyScriptEditor(m_contextType, this);

    QTextOption defaultOps = m_editor->document()->defaultTextOption();
    defaultOps.setFlags(defaultOps.flags() | QTextOption::ShowTabsAndSpaces /*|
                        QTextOption::ShowLineAndParagraphSeparators*/);

    m_editor->document()->setDefaultTextOption(defaultOps);
    m_editor->setStyleSheet("QPlainTextEdit {  border: 0px; }");

    GtPyHighlighter* highlighter = new GtPyHighlighter(m_editor->document());

    Q_UNUSED(highlighter)

    m_tabWidget = new QTabWidget(this);
    m_tabWidget->addTab(m_editor, "Script");
    m_tabWidget->tabBar()->setVisible(false);

    m_editorSplitter->addWidget(m_tabWidget);
    m_editorSplitter->setCollapsible(m_editorSplitter->indexOf(m_tabWidget),
                                     false);
    m_editorSplitter->setStretchFactor(m_editorSplitter->indexOf(m_tabWidget),
                                       3);
    splitter->addWidget(m_editorSplitter);

    m_separator = new QWidget(this);

    QHBoxLayout* separatorLay = new QHBoxLayout(this);

    separatorLay->setSpacing(0);
    separatorLay->setMargin(0);
    separatorLay->setContentsMargins(0,0,0,0);

    QLabel* label = new QLabel("Output Console:", this);

    separatorLay->addWidget(label);
    separatorLay->addStretch(1);

    m_searchWidget = new GtSearchWidget(this);
    separatorLay->addWidget(m_searchWidget);

    m_backwardButton = new QPushButton;
    m_backwardButton->setIcon(gtApp->icon("arrowleftIcon.png"));
//    m_backwardButton->setToolTip(tr("Import Python Script"));
    m_backwardButton->setMaximumSize(QSize(15, 20));
    m_backwardButton->setFlat(true);
    m_backwardButton->setVisible(false);

    separatorLay->addWidget(m_backwardButton);

    m_forwardButton = new QPushButton;
    m_forwardButton->setIcon(gtApp->icon("arrowrightIcon.png"));
//    m_forwardButton->setToolTip(tr("Import Python Script"));
    m_forwardButton->setMaximumSize(QSize(15, 20));
    m_forwardButton->setFlat(true);
    m_forwardButton->setVisible(false);

    separatorLay->addWidget(m_forwardButton);

    m_separator->setLayout(separatorLay);
    m_separator->setMaximumHeight(20);
    m_separator->setMinimumHeight(20);

    splitter->addWidget(m_separator);

    m_pythonConsole = new GtpyConsole(m_contextType, this);

    m_pythonConsole->setFrameStyle(m_editor->frameStyle());
    m_pythonConsole->setReadOnly(true);
    m_pythonConsole->setCommandPrompt("Evaluation");

    splitter->addWidget(m_pythonConsole);

    QHBoxLayout* toolBarLayout = new QHBoxLayout;

    //Clear Button
    m_consoleClearButton = new QPushButton;
    m_consoleClearButton->setIcon(gtApp->icon("clearIcon_16.png"));
    m_consoleClearButton->setToolTip(tr("Clear Output"));

    QLabel* shortCutClearOutput = new QLabel("<font color='grey'></font>");
    QFont fontClearOutput = shortCutClearOutput->font();
    fontClearOutput.setItalic(true);
    fontClearOutput.setPointSize(7);
    shortCutClearOutput->setFont(fontClearOutput);
    shortCutClearOutput->installEventFilter(this);

    QVBoxLayout* clearButtonLay = new QVBoxLayout;

    clearButtonLay->addWidget(m_consoleClearButton);
    clearButtonLay->addWidget(shortCutClearOutput);

    toolBarLayout->addLayout(clearButtonLay);

    //Evaluation Button
    m_evalButton = new QPushButton;
    m_evalButton->setIcon(gtApp->icon("updateIcon_16.png"));
    m_evalButton->setToolTip(tr("Evaluate Python Script"));

    QLabel* shortCutLabel = new QLabel("<font color='grey'>  Ctrl+E</font>");
    QFont font = shortCutLabel->font();
    font.setItalic(true);
    font.setPointSize(7);
    shortCutLabel->setFont(font);

    QVBoxLayout* evalButtonLay = new QVBoxLayout;

    evalButtonLay->addWidget(m_evalButton);
    evalButtonLay->addWidget(shortCutLabel);

    toolBarLayout->addLayout(evalButtonLay);

    toolBarLayout->addStretch(1);

    //Save Button
    m_shortCutSave = new QLabel("<font color='grey'>  Ctrl+S</font>");
    QFont fontSave = m_shortCutSave->font();
    fontSave.setItalic(true);
    fontSave.setPointSize(7);
    m_shortCutSave->setFont(fontSave);
    QSizePolicy sizePolicy = m_shortCutSave->sizePolicy();
    sizePolicy.setRetainSizeWhenHidden(true);
    m_shortCutSave->setSizePolicy(sizePolicy);

    m_saveButton = new QPushButton;
    m_saveButton->setIcon(gtApp->icon("saveProjectIcon.png"));
    m_saveButton->setToolTip(tr("Save Script"));

    QVBoxLayout* saveButtonLay = new QVBoxLayout;
    saveButtonLay->addWidget(m_saveButton);
    saveButtonLay->addWidget(m_shortCutSave);

    toolBarLayout->addLayout(saveButtonLay);

    //Import Button
    QLabel* shortCutImport = new QLabel("<font color='grey'></font>");
    QFont fontImport = shortCutImport->font();
    fontImport.setItalic(true);
    fontImport.setPointSize(7);
    shortCutImport->setFont(fontImport);

    QPushButton* importButton = new QPushButton;
    importButton->setIcon(gtApp->icon("importIcon.png"));
    importButton->setToolTip(tr("Import Python Script"));

    QVBoxLayout* importButtonLay = new QVBoxLayout;

    importButtonLay->addWidget(importButton);
    importButtonLay->addWidget(shortCutImport);

    toolBarLayout->addLayout(importButtonLay);

    //Export Button
    QPushButton* exportButton = new QPushButton;
    exportButton->setIcon(gtApp->icon("exportIcon.png"));
    exportButton->setToolTip(tr("Export Python Script"));

    QLabel* shortCutExport = new QLabel("<font color='grey'></font>");
    QFont fontExport = shortCutExport->font();
    fontExport.setItalic(true);
    fontExport.setPointSize(7);
    shortCutExport->setFont(fontExport);

    QVBoxLayout* exportButtonLay = new QVBoxLayout;

    exportButtonLay->addWidget(exportButton);
    exportButtonLay->addWidget(shortCutExport);

    toolBarLayout->addLayout(exportButtonLay);

    splitter->setCollapsible(splitter->indexOf(m_editorSplitter), false);
    splitter->setCollapsible(splitter->indexOf(m_separator), false);

    splitter->setStretchFactor(splitter->indexOf(m_editorSplitter), 3);
    splitter->setStretchFactor(splitter->indexOf(m_pythonConsole), 1);

    layout->addWidget(splitter);
    layout->addLayout(toolBarLayout);

    setLayout(layout);

    connect(m_editor, SIGNAL(evalShortcutTriggered()), this,
            SLOT(evalScript()));
    connect(m_editor, SIGNAL(searchShortcutTriggered(QString)), this,
            SLOT(setSearchedText(QString)));
    connect(m_searchWidget, SIGNAL(textEdited(QString)), m_editor,
            SLOT(searchHighlighting(QString)));
    connect(m_searchWidget, SIGNAL(textEdited(QString)), this,
            SLOT(onSearchTextEdit()));
    connect(m_searchWidget, SIGNAL(searchEnabled()), this,
            SLOT(onSearchEnabled()));
    connect(m_searchWidget, SIGNAL(searchDisabled()), this,
            SLOT(onSearchDisabled()));
    connect(m_searchWidget, SIGNAL(returnPressed()), this,
            SLOT(onSearchForward()));
    connect(m_backwardButton, SIGNAL(clicked(bool)), this,
            SLOT(onSearchBackward()));
    connect(m_forwardButton, SIGNAL(clicked(bool)), this,
            SLOT(onSearchForward()));
    connect(m_consoleClearButton, SIGNAL(clicked(bool)),
            m_pythonConsole, SLOT(clearConsole()));
    connect(m_evalButton, SIGNAL(clicked(bool)), this, SLOT(evalScript()));
    connect(importButton, SIGNAL(clicked(bool)), this, SLOT(onImportScript()));
    connect(exportButton, SIGNAL(clicked(bool)), this, SLOT(onExportScript()));
    connect(m_saveButton, SIGNAL(clicked(bool)), this,
            SLOT(onSaveButtonClicked()));
}

void
GtpyAbstractScriptingWizardPage::initializePage()
{
    GtpyContextManager::instance()->resetContext(m_contextType);

    initialization();

    foreach (QString packageName, m_packageNames)
    {
        GtObject* obj = scope()->getObjectByPath(QStringList() <<
                                                 scope()->objectName()
                                                 << packageName);

        if (obj != Q_NULLPTR)
        {
            GtObject* clone = obj->clone();
            clone->setParent(this);

            GtpyContextManager::instance()->addObject(
                        m_contextType, clone->objectName(), clone);
        }
    }

//    evalScript(false);
    qDebug() << "initializePage()";
}

bool
GtpyAbstractScriptingWizardPage::validatePage()
{
    //evalScript(false);

    return validation();
}

void
GtpyAbstractScriptingWizardPage::keyPressEvent(QKeyEvent* e)
{
    switch (e->key())
    {
        case Qt::Key_Return:
            // Ignore return
         return;

        default:
            break;
    }

    // Evaluation shortcut
    if (((e->modifiers() & Qt::ControlModifier) &&
         e->key() == Qt::Key_E))
    {
        evalScript(true);
        return;
    }

    // Save shortcut
    if (((e->modifiers() & Qt::ControlModifier) &&
         e->key() == Qt::Key_S))
    {
        onSaveButtonClicked();
        return;
    }

    GtProcessWizardPage::keyPressEvent(e);
}

void
GtpyAbstractScriptingWizardPage::enableCalculators(GtTask* task)
{
    GtpyContextManager::instance()->addTaskValue(m_contextType, task);
}

void
GtpyAbstractScriptingWizardPage::endEval(bool /*success*/)
{
    return;
}

void
GtpyAbstractScriptingWizardPage::insertWidgetNextToEditor(QWidget* widget,
                                                          int index,
                                                          int stretchFactor)
{
    if (m_editorSplitter == Q_NULLPTR)
    {
        return;
    }

    if (widget == Q_NULLPTR)
    {
        return;
    }

    m_editorSplitter->insertWidget(index, widget);
    m_editorSplitter->setStretchFactor(m_editorSplitter->indexOf(widget),
                                       stretchFactor);
}

int
GtpyAbstractScriptingWizardPage::defaultFrameStyle()
{
    if (m_editor == Q_NULLPTR)
    {
        return 0;
    }

    return m_editor->frameStyle();
}

void
GtpyAbstractScriptingWizardPage::setPlainTextToEditor(const QString& text)
{
    if (m_editor == Q_NULLPTR)
    {
        return;
    }

    m_editor->setPlainText(text);
}

QString
GtpyAbstractScriptingWizardPage::editorText()
{
    if (m_editor == Q_NULLPTR)
    {
        return QString();
    }

    return m_editor->toPlainText();
}

void
GtpyAbstractScriptingWizardPage::replaceCalcPyCode(const QString& header,
                                                   const QString& caption,
                                                   const QString& pyCode)
{
    QTextCursor cursor = m_editor->textCursor();

    cursor.clearSelection();

    cursor.movePosition(QTextCursor::Start);

    cursor = m_editor->document()->find(header, cursor);

    if (cursor.position() > -1)
    {
        cursor.movePosition(QTextCursor::Down);
        cursor.movePosition(QTextCursor::StartOfLine);

        int startPos = cursor.position();

        cursor = m_editor->document()->find(caption, cursor);

        if (cursor.position() > -1)
        {
            cursor.movePosition(QTextCursor::Up);
            cursor.movePosition(QTextCursor::EndOfLine);

            int endPos = cursor.position();

            cursor.setPosition(startPos);
            cursor.setPosition(endPos, QTextCursor::KeepAnchor);

            cursor.insertText(pyCode);
        }
    }
}

void
GtpyAbstractScriptingWizardPage::replaceBlockHeaders(const QString& oldHeader,
                                                     const QString& newHeader,
                                                     const QString& oldCaption,
                                                     const QString& newCaption)
{
    if (m_editor == Q_NULLPTR)
    {
        return;
    }

    m_editor->replaceBlockHeaders(oldHeader, newHeader, oldCaption, newCaption);
}

void
GtpyAbstractScriptingWizardPage::searchAndReplaceEditorText(
        const QRegExp& searchFor, const QString& replaceBy, bool all)
{
    if (m_editor == Q_NULLPTR)
    {
        return;
    }

    m_editor->searchAndReplace(searchFor, replaceBy, all);
}

void
GtpyAbstractScriptingWizardPage::searchAndReplaceEditorText(
        const QString& searchFor, const QString& replaceBy, bool all)
{
    if (m_editor == Q_NULLPTR)
    {
        return;
    }

    m_editor->searchAndReplace(searchFor, replaceBy, all);
}

void
GtpyAbstractScriptingWizardPage::setConsoleVisible(bool visible)
{
    if (m_separator != Q_NULLPTR)
    {
        m_separator->setVisible(visible);
    }

    if (m_pythonConsole != Q_NULLPTR)
    {
        m_pythonConsole->setVisible(visible);
    }

    if (m_consoleClearButton != Q_NULLPTR)
    {
        m_consoleClearButton->setVisible(visible);
    }
}

void
GtpyAbstractScriptingWizardPage::enableEvaluation(bool enable)
{
    if (m_evalButton != Q_NULLPTR)
    {
        m_evalButton->setVisible(enable);

        if (m_editor == Q_NULLPTR)
        {
            return;
        }

        if (enable)
        {
            connect(m_editor, SIGNAL(evalShortcutTriggered()), this,
                    SLOT(evalScript()));
        }
        else
        {
            disconnect(m_editor, SIGNAL(evalShortcutTriggered()), this,
                       SLOT(evalScript()));
        }
    }
}

void
GtpyAbstractScriptingWizardPage::insertToCurrentCursorPos(const QString& text)
{
    if (m_editor == Q_NULLPTR)
    {
        return;
    }

    m_editor->insertToCurrentCursorPos(text);
}

QString
GtpyAbstractScriptingWizardPage::propValToString(GtAbstractProperty* prop)
{
    QString val;

    if (qobject_cast<GtModeProperty*>(prop))
    {
        QString valTemp = GtpyContextManager::instance()->qvariantToPyStr(prop->valueToVariant());

        if (valTemp.isEmpty())
        {
            valTemp = "\"\"";
        }

        val = "\""
              + valTemp
              + "\"";
    }
    else if (qobject_cast<GtObjectLinkProperty*>(prop))
    {
        GtObject* obj = gtDataModel->objectByUuid(prop->valueToVariant().toString());

        if (obj)
        {
            GtPackage* pack = Q_NULLPTR;

            while (pack == Q_NULLPTR)
            {
                QString objName = obj->objectName();

                if (!objName.contains(
                            QRegExp(QStringLiteral("^[a-zA-Z0-9_]*$"))))
                {
                    QString funcName =
                            GtpyContextManager::instance()->findChildFuncName();

                    objName = funcName + "(\"" + objName +
                                   "\")";
                }

                obj = qobject_cast<GtObject*>(obj->parent());

                if (obj == Q_NULLPTR)
                {
                    break;
                }

                pack = qobject_cast<GtPackage*>(obj);

                val.insert(0, objName);

                val.insert(0, ".");

                if (pack != Q_NULLPTR)
                {
                    objName = pack->objectName();

                    if (!objName.contains(
                                QRegExp(QStringLiteral("^[a-zA-Z0-9_]*$"))))
                    {
                        QString funcName =
                                GtpyContextManager::instance()->findChildFuncName();

                        objName = funcName + "(\"" + objName +
                                       "\")";
                    }

                    val.insert(0, objName);
                }
            }

            obj = Q_NULLPTR;
            pack = Q_NULLPTR;
        }
        else
        {
            QString valTemp = prop->valueToVariant().toString();

            if (valTemp.isEmpty())
            {
                val = "\"\"";
            }
            else
            {
                val =  GtpyContextManager::instance()->qvariantToPyStr(prop->valueToVariant());
            }
        }
    }
    else if (qobject_cast<GtStringProperty*>(prop))
    {
        val = "\"";
        val +=  GtpyContextManager::instance()->qvariantToPyStr(prop->valueToVariant());
        val += "\"";
    }
    else if (dynamic_cast<GtProperty<QString>*>(prop))
    {
        val = "\"";
        val +=  GtpyContextManager::instance()->qvariantToPyStr(prop->valueToVariant());
        val += "\"";
    }
    else
    {
        val = GtpyContextManager::instance()->qvariantToPyStr(prop->valueToVariant());
    }

    return val;
}

void
GtpyAbstractScriptingWizardPage::setPackageNames(QStringList packageNames)
{
    m_packageNames = packageNames;
}

void
GtpyAbstractScriptingWizardPage::enableSaveButton(bool enable)
{
    m_saveButton->setEnabled(enable);
    m_shortCutSave->setVisible(enable);
}

void
GtpyAbstractScriptingWizardPage::addTabWidget(QWidget* wid,
                                              const QString& label)
{
    if (wid == Q_NULLPTR || m_tabWidget == Q_NULLPTR)
    {
        return;
    }

    m_tabWidget->tabBar()->setVisible(true);
    m_tabWidget->addTab(wid, label);
}

void
GtpyAbstractScriptingWizardPage::evalScript(bool outputToConsole)
{
    m_evalButton->setEnabled(false);
    m_editor->setReadOnly(true);

    GtpyContextManager::instance()->deleteCalcsFromTask(m_contextType);

    GtpyScriptRunnable* runnable = new GtpyScriptRunnable(m_contextType);

    runnable->setParent(this);
    runnable->setScript(m_editor->script());

    QThreadPool* tp = QThreadPool::globalInstance();

    // start runnable
    tp->start(runnable);

    // make runnable not delete herself
    runnable->setAutoDelete(false);

    // connect runnable signals to wizard slots
    connect(runnable, &GtpyScriptRunnable::runnableFinished,
            this, &GtpyAbstractScriptingWizardPage::evaluationFinished);

//    bool success = GtpyContextManager::instance()->evalScript(
//                    m_contextType, m_editor->script(), outputToConsole);

//    endEval(success);

    return;
}

void
GtpyAbstractScriptingWizardPage::initialization()
{
    return;
}

bool
GtpyAbstractScriptingWizardPage::validation()
{
    return true;
}

void
GtpyAbstractScriptingWizardPage::onImportScript()
{
    QString filename = GtFileDialog::getOpenFileName(this,
                       tr("Open Python Script"), QString(),
                       tr("Python Script (*.py)"));

    if (filename.isEmpty())
    {
        return;
    }

    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly))
    {
        m_pythonConsole->stdErr("Script \"" + filename + "\" not found!",
                                m_contextType);
    }

    QTextStream in(&file);

    QString script;

    while(!in.atEnd())
    {
        QString line = in.readLine();
        script = script + line + "\n";
    }

    file.close();

    m_editor->clear();

    m_editor->setPlainText(script);

    evalScript(false);
}

void
GtpyAbstractScriptingWizardPage::onExportScript()
{
    QString filename = GtFileDialog::getSaveFileName(this, tr("Save Script"),
                  QString(), "*.py", "pythonTaskScript.py");

    if (!filename.endsWith(".py"))
    {
        filename = filename + ".py";
    }

    QFile file(filename);

    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        stream << m_editor->script();
        file.close();
    }
}

void
GtpyAbstractScriptingWizardPage::setSearchedText(const QString& text)
{
    m_searchWidget->setText(text);
    m_searchWidget->enableSearch();
}

void
GtpyAbstractScriptingWizardPage::onSearchEnabled()
{
    m_forwardButton->setVisible(true);
    m_backwardButton->setVisible(true);

    if (m_searchWidget->text().isEmpty())
    {
        m_forwardButton->setEnabled(false);
        m_backwardButton->setEnabled(false);
    }
    else
    {
        m_forwardButton->setEnabled(true);
        m_backwardButton->setEnabled(true);
    }

    m_editor->textSearchingActivated();
}

void
GtpyAbstractScriptingWizardPage::onSearchDisabled()
{
    m_forwardButton->setVisible(false);
    m_backwardButton->setVisible(false);

    m_editor->removeSearchHighlighting();
}

void
GtpyAbstractScriptingWizardPage::onSearchBackward()
{
    m_editor->moveCursorToLastFound();
}

void
GtpyAbstractScriptingWizardPage::onSearchForward()
{
    m_editor->moveCursorToNextFound();
}

void
GtpyAbstractScriptingWizardPage::onSearchTextEdit()
{
    if (m_searchWidget->text().isEmpty())
    {
        m_forwardButton->setEnabled(false);
        m_backwardButton->setEnabled(false);
    }
    else
    {
        m_forwardButton->setEnabled(true);
        m_backwardButton->setEnabled(true);
    }
}

void
GtpyAbstractScriptingWizardPage::evaluationFinished()
{
    qDebug() << "Evaluation finished!!!";

    m_evalButton->setEnabled(true);
    m_editor->setReadOnly(false);

    GtpyScriptRunnable* runnable = qobject_cast<GtpyScriptRunnable*>(sender());

    if (runnable)
    {
        bool success = runnable->successful();

        // connect runnable signals to task runner slots
        disconnect(runnable, &GtpyScriptRunnable::runnableFinished,
                this, &GtpyAbstractScriptingWizardPage::evaluationFinished);

        delete runnable;

        qDebug() << success;

        endEval(success);
    }
}
