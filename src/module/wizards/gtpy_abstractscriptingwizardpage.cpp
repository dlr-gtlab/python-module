/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_abstractscriptingwizardpage.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 12.08.2019
 * Author: Marvin Noethen (DLR AT-TWK)
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
#include <QMainWindow>
#include <QApplication>

// python includes
#include "gtpy_icons_compat.h"
#include "gtpy_scripteditor.h"
#include "gtpy_console.h"
#include "gtpy_scriptrunnable.h"
#include "gtpy_wizardgeometries.h"
#include "gtpy_editorsettingsdialog.h"
#include "gtpy_packageiteration.h"
#include "gtpy_transfer.h"

// GTlab framework includes
#include "gt_object.h"
#include "gt_project.h"
#include "gt_datamodel.h"
#include "gt_filedialog.h"
#include "gt_saveprojectmessagebox.h"

#include "gt_pyhighlighter.h"
#include "gt_searchwidget.h"

#include "gtpy_abstractscriptingwizardpage.h"

GtpyAbstractScriptingWizardPage::GtpyAbstractScriptingWizardPage(
    GtpyContextManager::Context type) :
    m_contextId(-1),
    m_contextType(type),
    m_editor(nullptr),
    m_editorSplitter(nullptr),
    m_tabWidget(nullptr),
    m_editorSettings(nullptr),
    m_isEvaluating(false),
    m_runnable(nullptr),
    m_savingEnabled(true),
    m_componentUuid(QString())
{
    setTitle(tr("Python Script Editor"));

    m_contextId = GtpyContextManager::instance()->createNewContext(type);

    QVBoxLayout* layout = new QVBoxLayout;

    QSplitter* splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Vertical);

    m_editorSplitter = new QSplitter(this);
    m_editorSplitter->setOrientation(Qt::Horizontal);

    m_editor = new GtpyScriptEditor(m_contextId, this);

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
    separatorLay->setContentsMargins(0, 0, 0, 0);

    QLabel* label = new QLabel("Output Console:", this);

    separatorLay->addWidget(label);
    separatorLay->addStretch(1);

    m_searchWidget = new GtSearchWidget(this);
    separatorLay->addWidget(m_searchWidget);

    m_backwardButton = new QPushButton;
    m_backwardButton->setIcon(GTPY_ICON(triangleSmallLeft));
    m_backwardButton->setToolTip(tr("Search backwards"));
    m_backwardButton->setMaximumSize(QSize(15, 20));
    m_backwardButton->setFlat(true);
    m_backwardButton->setVisible(false);

    separatorLay->addWidget(m_backwardButton);

    m_forwardButton = new QPushButton;
    m_forwardButton->setIcon(GTPY_ICON(triangleSmallRight));
    m_forwardButton->setToolTip(tr("Search forwards"));
    m_forwardButton->setMaximumSize(QSize(15, 20));
    m_forwardButton->setFlat(true);
    m_forwardButton->setVisible(false);

    separatorLay->addWidget(m_forwardButton);

    m_separator->setLayout(separatorLay);
    m_separator->setMaximumHeight(20);
    m_separator->setMinimumHeight(20);

    splitter->addWidget(m_separator);

    m_pythonConsole = new GtpyConsole(m_contextId, this);

    m_pythonConsole->setFrameStyle(m_editor->frameStyle());
    m_pythonConsole->setReadOnly(true);
    m_pythonConsole->setCommandPrompt("Evaluation");

    splitter->addWidget(m_pythonConsole);

    QHBoxLayout* toolBarLayout = new QHBoxLayout;

    //Clear Button
    m_consoleClearButton = new QPushButton;
    m_consoleClearButton->setIcon(GTPY_ICON(clear));
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
    m_evalButton->setIcon(GTPY_ICON(play));
    m_evalButton->setToolTip(tr("Evaluate Python script"));

    m_shortCutEval = new QLabel("<font color='grey'>  Ctrl+E</font>");
    QFont font = m_shortCutEval->font();
    font.setItalic(true);
    font.setPointSize(7);
    m_shortCutEval->setFont(font);

    QVBoxLayout* evalButtonLay = new QVBoxLayout;

    evalButtonLay->addWidget(m_evalButton);
    evalButtonLay->addWidget(m_shortCutEval);

    toolBarLayout->addLayout(evalButtonLay);

    toolBarLayout->addStretch(1);

    //Save Button
    m_shortCutSave = new QLabel("<font color='grey'>  Ctrl+S</font>");
    QFont fontSave = m_shortCutSave->font();
    fontSave.setItalic(true);
    fontSave.setPointSize(7);
    m_shortCutSave->setFont(fontSave);

    m_saveButton = new QPushButton;
    m_saveButton->setIcon(GTPY_ICON(save));
    m_saveButton->setToolTip(tr("Save Script"));

    QVBoxLayout* saveButtonLay = new QVBoxLayout;
    saveButtonLay->addWidget(m_saveButton);
    saveButtonLay->addWidget(m_shortCutSave);

    toolBarLayout->addLayout(saveButtonLay);

    enableSaveButton(false);

    //Import Button
    QLabel* shortCutImport = new QLabel("<font color='grey'></font>");
    QFont fontImport = shortCutImport->font();
    fontImport.setItalic(true);
    fontImport.setPointSize(7);
    shortCutImport->setFont(fontImport);

    QPushButton* importButton = new QPushButton;
    importButton->setIcon(GTPY_ICON(import));
    importButton->setToolTip(tr("Import Python Script"));

    QVBoxLayout* importButtonLay = new QVBoxLayout;

    importButtonLay->addWidget(importButton);
    importButtonLay->addWidget(shortCutImport);

    toolBarLayout->addLayout(importButtonLay);

    //Export Button
    QPushButton* exportButton = new QPushButton;
    exportButton->setIcon(GTPY_ICON(export_));
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

    //Settings Button
    QLabel* shortCutSettings = new QLabel("<font color='grey'></font>");
    QFont fontSettings = shortCutSettings->font();
    fontSettings.setItalic(true);
    fontSettings.setPointSize(7);
    shortCutSettings->setFont(fontSettings);

    QPushButton* settingsButton = new QPushButton;
    settingsButton->setIcon(GTPY_ICON(config));
    settingsButton->setToolTip(tr("Editor settings"));

    QVBoxLayout* settingsButtonLay = new QVBoxLayout;

    settingsButtonLay->addWidget(settingsButton);
    settingsButtonLay->addWidget(shortCutSettings);

    toolBarLayout->addLayout(settingsButtonLay);

    splitter->setCollapsible(splitter->indexOf(m_editorSplitter), false);
    splitter->setCollapsible(splitter->indexOf(m_separator), false);

    splitter->setStretchFactor(splitter->indexOf(m_editorSplitter), 3);
    splitter->setStretchFactor(splitter->indexOf(m_pythonConsole), 1);

    layout->addWidget(splitter);
    layout->addLayout(toolBarLayout);

    setLayout(layout);

    connect(m_editor, SIGNAL(evalShortcutTriggered()), this,
            SLOT(onEvalShortCutTriggered()));
    connect(m_editor, SIGNAL(searchShortcutTriggered(QString)), this,
            SLOT(setSearchedText(QString)));
    connect(m_editor, SIGNAL(calculatorDropped(GtCalculator*)), this,
            SLOT(onCalculatorDropped(GtCalculator*)));
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
    connect(m_evalButton, SIGNAL(clicked(bool)), this,
            SLOT(onEvalButtonClicked()));
    connect(importButton, SIGNAL(clicked(bool)), this, SLOT(onImportScript()));
    connect(exportButton, SIGNAL(clicked(bool)), this, SLOT(onExportScript()));
    connect(settingsButton, SIGNAL(clicked(bool)), this,
            SLOT(onSettingsButton()));
}

GtpyAbstractScriptingWizardPage::~GtpyAbstractScriptingWizardPage()
{
    deleteRunnable();

    GtpyContextManager::instance()->deleteContext(m_contextId);
    registerGeometry();
}

void
GtpyAbstractScriptingWizardPage::initializePage()
{
    // we want to react, when the wizard should be closed
    if (wizard()) wizard()->installEventFilter(this);

    /// Can not be connected in the constructor because onSaveButtonClicked()
    /// calls the pure virtual function saveScript()
    connect(m_saveButton, SIGNAL(clicked(bool)), this,
            SLOT(onSaveButtonClicked()));

    setWizardNonModal();

    initialization();

    m_componentUuid = componentUuid();

    m_editorSettings = createSettings();
    m_editor->setTabSize(m_editorSettings->tabSize());
    m_editor->replaceTabsBySpaces(m_editorSettings->replaceTabBySpace());

    reloadWizardGeometry();

    loadPackages();

    GtObject* component = gtDataModel->objectByUuid(m_componentUuid);

    if (!component)
    {
        enableSaving(false);
    }
    else
    {
        setTitle(component->objectName());
        connect(component, SIGNAL(objectNameChanged(QString)), this,
                SLOT(componentRenamed(QString)));
    }

    connect(m_editor, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
}

bool
GtpyAbstractScriptingWizardPage::validatePage()
{
    deleteRunnable();

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

        case Qt::Key_Escape:

            if (m_saveButton->isEnabled())
            {
                saveMessageBox();
                return;
            }

            break;

        default:
            break;
    }

    // Evaluation shortcut
    if (((e->modifiers() & Qt::ControlModifier) &&
            e->key() == Qt::Key_E))
    {
        onEvalShortCutTriggered();

        return;
    }

    // Interrupt shortcut
    if (((e->modifiers() & Qt::ControlModifier) &&
            e->key() == Qt::Key_I))
    {
        if (m_isEvaluating)
        {
            onEvalButtonClicked();
        }

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
    GtpyContextManager::instance()->addTaskValue(m_contextId, task);
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
    if (!m_editorSplitter)
    {
        return;
    }

    if (!widget)
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
    if (!m_editor)
    {
        return 0;
    }

    return m_editor->frameStyle();
}

void
GtpyAbstractScriptingWizardPage::setPlainTextToEditor(const QString& text)
{
    if (!m_editor)
    {
        return;
    }

    m_editor->setPlainText(text);
}

QString
GtpyAbstractScriptingWizardPage::editorText()
{
    if (!m_editor)
    {
        return QString();
    }

    return m_editor->toPlainText();
}

void
GtpyAbstractScriptingWizardPage::replaceCalcPyCode(const QString& header,
        const QString& caption,
        QString pyCode)
{
    QTextCursor cursor = m_editor->textCursor();

    cursor.clearSelection();

    cursor.movePosition(QTextCursor::Start);

    cursor = m_editor->document()->find(header, cursor);

    if (cursor.position() > -1)
    {
        cursor.movePosition(QTextCursor::Down);
        cursor.movePosition(QTextCursor::StartOfLine);

        cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
        QString firstLine = cursor.selectedText();

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

            QString indent = indentation(firstLine);

            if (!indent.isEmpty())
            {
                pyCode.prepend(indent);
                pyCode.replace("\n", "\n" + indent);
            }

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
    if (!m_editor)
    {
        return;
    }

    m_editor->replaceBlockHeaders(oldHeader, newHeader, oldCaption, newCaption);
}

void
GtpyAbstractScriptingWizardPage::searchAndReplaceEditorText(
    const QRegExp& searchFor, const QString& replaceBy, bool all)
{
    if (!m_editor)
    {
        return;
    }

    m_editor->searchAndReplace(searchFor, replaceBy, all);
}

void
GtpyAbstractScriptingWizardPage::searchAndReplaceEditorText(
    const QString& searchFor, const QString& replaceBy, bool all)
{
    if (!m_editor)
    {
        return;
    }

    m_editor->searchAndReplace(searchFor, replaceBy, all);
}

void
GtpyAbstractScriptingWizardPage::setConsoleVisible(bool visible)
{
    if (m_separator)
    {
        m_separator->setVisible(visible);
    }

    if (m_pythonConsole)
    {
        m_pythonConsole->setVisible(visible);
    }

    if (m_consoleClearButton)
    {
        m_consoleClearButton->setVisible(visible);
    }
}

QString
GtpyAbstractScriptingWizardPage::indentation(const QString& codeLine) const
{
    QString indent;
    QRegExp spacesRegExp("^ +\t*");
    QRegExp tabRegExp("^\t+ *");
    QRegExp regExp;

    if (codeLine.contains(spacesRegExp))
    {
        regExp = spacesRegExp;
    }
    else if (codeLine.contains(tabRegExp))
    {
        regExp = tabRegExp;
    }

    int pos = regExp.indexIn(codeLine);

    if (pos > -1)
    {
        indent = regExp.cap(0);
    }

    return indent;
}

void
GtpyAbstractScriptingWizardPage::insertToCurrentCursorPos(QString text)
{
    if (!m_editor)
    {
        return;
    }

    QTextCursor cur = m_editor->textCursor();
    cur.clearSelection();
    cur.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
    QString selection = cur.selectedText();

    QString indent = indentation(selection);

    if (!indent.isEmpty())
    {
        text.replace("\n", "\n" + indent);
    }

    m_editor->insertToCurrentCursorPos(text);
}

void
GtpyAbstractScriptingWizardPage::addTabWidget(QWidget* wid,
        const QString& label)
{
    if (!wid || !m_tabWidget)
    {
        return;
    }

    m_tabWidget->tabBar()->setVisible(true);
    m_tabWidget->addTab(wid, label);
}

void
GtpyAbstractScriptingWizardPage::evalScript(bool outputToConsole)
{
    GtpyContextManager::instance()->deleteCalcsFromTask(m_contextId);

    evalScript(m_editor->script(), outputToConsole);
}

void
GtpyAbstractScriptingWizardPage::evalScript(const QString& script,
        bool outputToConsole)
{
    if (m_isEvaluating)
    {
        return;
    }

    m_isEvaluating = true;

    m_runnable = new GtpyScriptRunnable(m_contextId);

    m_runnable->setScript(script);
    m_runnable->setOutputToConsole(outputToConsole);

    // make runnable not delete itself
    m_runnable->setAutoDelete(false);

    // connect runnable signals to wizard slots
    connect(m_runnable, SIGNAL(runnableFinished()),
            this, SLOT(evaluationFinished()));

    QThreadPool* tp = QThreadPool::globalInstance();

    // start runnable
    tp->start(m_runnable);

    return;
}

void
GtpyAbstractScriptingWizardPage::onEvalButtonClicked()
{
    if (m_isEvaluating)
    {
        if (m_runnable)
        {
            m_runnable->interrupt();
            return;
        }
    }

    evalScript(true);
    showEvalButton(false);
}

void
GtpyAbstractScriptingWizardPage::onEvalShortCutTriggered()
{
    if (!m_isEvaluating)
    {
        onEvalButtonClicked();
    }
}

void
GtpyAbstractScriptingWizardPage::onSaveButtonClicked()
{
    connect(m_editor, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
    saveScript();
    enableSaveButton(false);
}

void
GtpyAbstractScriptingWizardPage::onTextChanged()
{
    enableSaveButton(true);
    disconnect(m_editor, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
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
GtpyAbstractScriptingWizardPage::enableSaveButton(bool enable)
{
    if (!m_savingEnabled)
    {
        enable = false;
    }

    m_saveButton->setEnabled(enable);

    if (enable)
    {
        m_shortCutSave->setText("<font color='grey'>  Ctrl+S</font>");
    }
    else
    {
        m_shortCutSave->setText("");
    }
}

int
GtpyAbstractScriptingWizardPage::saveMessageBox()
{
    QWidget* wiz = wizard();

    if (!wiz)
    {
        return QMessageBox::Cancel;
    }

    QString text =
        tr("Do you want to ") +
        tr("save your changes before closing the wizard?");

    GtSaveProjectMessageBox mb(text);

    mb.setWindowFlags(mb.windowFlags() | Qt::WindowStaysOnTopHint);

    int ret;

    ret = mb.exec();

    switch (ret)
    {
        case QMessageBox::Yes:
            onSaveButtonClicked();
            wiz->close();
            break;

        case QMessageBox::No:
            wiz->close();
            break;

        case QMessageBox::Cancel:
            break;

        default:
            break;
    }

    return ret;
}

void
GtpyAbstractScriptingWizardPage::setWizardNonModal()
{
    QWidget* wiz = wizard();

    if (wiz)
    {
        QWizard* wizard = qobject_cast<QWizard*>(wiz);

        if (wizard)
        {
            if (wizard->pageIds().count() == 1)
            {
                QWidgetList widgets = QApplication::topLevelWidgets();

                foreach (QWidget* wid, widgets)
                {
                    QMainWindow* mainWin = qobject_cast<QMainWindow*>(wid);

                    if (mainWin)
                    {
                        wiz->setWindowModality(Qt::NonModal);
                        wiz->setParent(mainWin);
                        wiz->setWindowFlags(Qt::Dialog);
                        break;
                    }
                }
            }
        }
    }
}

void
GtpyAbstractScriptingWizardPage::registerGeometry()
{
    if (!m_componentUuid.isEmpty())
    {
        QWidget* wiz = wizard();

        if (wiz)
        {

            QRect rect = wiz->frameGeometry();

            GtpyWizardGeometries::instance()->registerGeometry(m_componentUuid,
                    rect);

            int cursorPos = m_editor->cursorPosition();
            GtpyWizardGeometries::instance()->registerCursorPos(m_componentUuid,
                    cursorPos);

            int vSliderPos = m_editor->verticalSliderPos();

            GtpyWizardGeometries::instance()->registerVSliderPos(m_componentUuid,
                    vSliderPos);
        }
    }
}

void
GtpyAbstractScriptingWizardPage::reloadWizardGeometry()
{
    if (!m_componentUuid.isEmpty())
    {
        QWidget* wiz = wizard();

        if (!wiz)
        {
            return;
        }

        QRect rect = GtpyWizardGeometries::instance()->lastGeometry(
                         m_componentUuid);

        if (!rect.isNull())
        {
            wiz->setGeometry(rect);
        }

        int cursorPos = GtpyWizardGeometries::instance()->lastCursorPos(
                            m_componentUuid);

        m_editor->setCursorPosition(cursorPos);

        int vSliderPos = GtpyWizardGeometries::instance()->lastVSliderPos(
                             m_componentUuid);

        m_editor->setVerticalSliderPos(vSliderPos);
    }
}

void
GtpyAbstractScriptingWizardPage::loadPackages()
{   
    gtpy::package::forEachPackage([&](const GtPackage* pkg){
        if (pkg)
        {
            auto* clone = pkg->clone();
            clone->setParent(this);
            gtpy::transfer::gtObjectToPython(m_contextId, clone);
        }
    });
}

void
GtpyAbstractScriptingWizardPage::deleteRunnable()
{
    if (m_runnable)
    {
        GtpyContextManager::instance()->autoDeleteRunnable(m_runnable);
        m_runnable->interrupt();
        m_runnable = nullptr;
    }
}

void
GtpyAbstractScriptingWizardPage::showEvalButton(bool show)
{
    if (!m_evalButton)
    {
        return;
    }

    if (show)
    {
        m_evalButton->setIcon(GTPY_ICON(play));
        m_evalButton->setToolTip(tr("Evaluate Python script"));

        m_shortCutEval->setText("<font color='grey'>  Ctrl+E</font>");
    }
    else
    {
        m_evalButton->setIcon(GTPY_ICON(stop));
        m_evalButton->setToolTip(tr("Interrupt evaluation"));

        m_shortCutEval->setText("<font color='grey'>  Ctrl+I</font>");
    }
}

void
GtpyAbstractScriptingWizardPage::acceptCalculatorDrops(bool accept)
{
    m_editor->acceptCalculatorDrops(accept);
}

void
GtpyAbstractScriptingWizardPage::cursorToNewLine()
{
    QTextCursor cur = m_editor->textCursor();

    cur.movePosition(QTextCursor::StartOfLine);
    cur.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);

    if (!cur.selectedText().isEmpty())
    {
        cur.insertText("\n");
    }

    m_editor->setTextCursor(cur);
}

void
GtpyAbstractScriptingWizardPage::enableSaving(bool enable)
{
    m_savingEnabled = enable;
}


bool
GtpyAbstractScriptingWizardPage::eventFilter(QObject *watched, QEvent *event)
{
    if (event && event->type() == QEvent::Close )
    {
        if (m_saveButton->isEnabled())
        {
            if (saveMessageBox() == QMessageBox::Cancel)
            {
                event->ignore();
            }
            return true;
        }
    }

    return QObject::eventFilter(watched, event);
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

    if (!file.open(QIODevice::ReadOnly))
    {
        m_pythonConsole->stdErr("Script \"" + filename + "\" not found!",
                                m_contextId);
    }

    QTextStream in(&file);

    QString script;

    while (!in.atEnd())
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
GtpyAbstractScriptingWizardPage::onSettingsButton()
{
    GtpyEditorSettingsDialog* dialog = new GtpyEditorSettingsDialog(
        m_editorSettings);
    dialog->setWindowTitle("Editor settings");

    if (dialog->exec())
    {
        saveSettings(m_editorSettings);
        m_editor->setTabSize(m_editorSettings->tabSize());
        m_editor->replaceTabsBySpaces(m_editorSettings->replaceTabBySpace());
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
    showEvalButton(true);

    m_isEvaluating = false;

    if (m_runnable)
    {
        bool success = m_runnable->successful();

        // connect runnable signals to task runner slots
        disconnect(m_runnable, &GtpyScriptRunnable::runnableFinished,
                   this, &GtpyAbstractScriptingWizardPage::evaluationFinished);

        delete m_runnable;
        m_runnable = nullptr;

        endEval(success);
    }
}

void
GtpyAbstractScriptingWizardPage::onCalculatorDropped(GtCalculator* calc)
{
    emit calculatorDropReceived(calc);
}

void
GtpyAbstractScriptingWizardPage::componentRenamed(const QString& name)
{
    setComponentName(name);
    setTitle(name);
}
