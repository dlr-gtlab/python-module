/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scripteditormdi.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 28.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gtpy_scripteditormdi.h"

#include <QLabel>
#include <QTimer>
#include <QHBoxLayout>
#include <QPushButton>
#include <QThreadPool>

#include <gt_task.h>
#include <gt_icons.h>
#include <gt_application.h>
#include <gt_pyhighlighter.h>
#include <gt_searchwidget.h>

#include "gtpy_shortcut.h"
#include "gtpy_scripteditor.h"
#include "gtpy_contextmanager.h"

namespace
{

void updateEvalBtn(QPushButton& evalBtn, QLabel& label, bool isEvaluating)
{
    evalBtn.setIcon(isEvaluating ? gt::gui::icon::stop()
                                 : gt::gui::icon::play());
    evalBtn.setToolTip(isEvaluating ? QObject::tr("Interrupt Evaluation")
                                    : QObject::tr("Evaluate Python Script"));

    label.setText(QString{"<font color='grey'>  (%1)</font>"}
                      .arg(isEvaluating ? gtpy::shortcut::interruptStr()
                                        : gtpy::shortcut::evalStr()));
}

void activateSearch(GtSearchWidget& search, QPushButton& backward,
                    QPushButton& forward)
{
    forward.setVisible(true);
    backward.setVisible(true);

    const bool hasText = !search.text().isEmpty();
    forward.setEnabled(hasText);
    backward.setEnabled(hasText);
}

}

GtpyScriptEditorMdi::GtpyScriptEditorMdi()
{
    setObjectName(tr("Python Script Viewer"));

    m_contextId = GtpyContextManager::instance()->createNewContext(
        GtpyContextManager::CollectionContext, true);

    // add tmp task to context; makes sure that findGtTask() is callable
    auto* tmpTask = new GtTask{};
    tmpTask->setParent(this);
    GtpyContextManager::instance()->addObject(
        m_contextId, gtpy::code::attrs::TASK, tmpTask, false);

    // script editor
    m_scriptEdit = new GtpyScriptEditor{m_contextId, widget()};
    m_scriptEdit->setReadOnly(true);
    m_scriptEdit->setFrameShape(QFrame::NoFrame);
    m_scriptEdit->replaceTabsBySpaces();
    m_scriptEdit->setTabSize(4);

    QTextOption defaultOps = m_scriptEdit->document()->defaultTextOption();
    defaultOps.setFlags(defaultOps.flags() | QTextOption::ShowTabsAndSpaces);
    m_scriptEdit->document()->setDefaultTextOption(defaultOps);

    auto* _ = new GtPyHighlighter(m_scriptEdit->document());
    Q_UNUSED(_)

    // search layout
    auto* searchWid = new GtSearchWidget{};
    searchWid->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    auto* backwardBtn = new QPushButton{};
    backwardBtn->setIcon(gt::gui::icon::triangleSmallLeft());
    backwardBtn->setToolTip(tr("Search backwards"));
    backwardBtn->setMaximumSize({15, 20});
    backwardBtn->setFlat(true);
    backwardBtn->setVisible(false);

    auto* forwardBtn = new QPushButton{};
    forwardBtn->setIcon(gt::gui::icon::triangleSmallRight());
    forwardBtn->setToolTip(tr("Search forwards"));
    forwardBtn->setMaximumSize({15, 20});
    forwardBtn->setFlat(true);
    forwardBtn->setVisible(false);

    auto* searchContainerLay = new QHBoxLayout{};
    searchContainerLay->setContentsMargins(0, 0, 0, 0);
    searchContainerLay->setSpacing(0);
    searchContainerLay->addWidget(searchWid);

    auto* searchContainer = new QWidget{};
    searchContainer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    searchContainer->setLayout(searchContainerLay);

    auto* searchLay = new QHBoxLayout;
    searchLay->setSpacing(0);
    searchLay->setContentsMargins(0,0,0,0);
    searchLay->addWidget(searchContainer);
    searchLay->addWidget(backwardBtn);
    searchLay->addWidget(forwardBtn);

    // eval button layout
    m_evalBtn = new QPushButton{};
    m_evalBtn->setMaximumSize({20, 20});
    m_evalBtn->setFlat(true);

    m_evalLabel = new QLabel{};
    QFont font = m_evalLabel->font();
    font.setItalic(true);
    font.setPointSize(7);
    m_evalLabel->setFont(font);

    updateEvalBtn(*m_evalBtn, *m_evalLabel, false);

    auto* evalButtonLay = new QHBoxLayout;
    evalButtonLay->addWidget(m_evalBtn);
    evalButtonLay->addWidget(m_evalLabel);

    // toolbar layout
    auto* toolbarLayout = new QHBoxLayout;
    toolbarLayout->setSpacing(0);
    toolbarLayout->setContentsMargins(0, 0, 0, 0);
    toolbarLayout->addLayout(searchLay);
    toolbarLayout->addStretch();
    toolbarLayout->addLayout(evalButtonLay);

    // main layout
    auto* mainLayout = new QVBoxLayout{widget()};
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_scriptEdit);
    mainLayout->addLayout(toolbarLayout);

    widget()->setLayout(mainLayout);

    auto onEvalBtnClicked = [this]() {
        if (m_isEvaluating && m_runnable) m_runnable->interrupt();
        else evalScript();

        updateEvalBtn(*m_evalBtn, *m_evalLabel, m_isEvaluating);
    };

    auto handleShortCut = [this, onEvalBtnClicked](bool startEval) {
        if (m_isEvaluating != startEval) onEvalBtnClicked();
    };

    connect(m_evalBtn, &QPushButton::clicked, this, [onEvalBtnClicked](bool) {
        onEvalBtnClicked();
    });
    connect(m_scriptEdit, &GtpyScriptEditor::evalShortcutTriggered, this,
            std::bind(handleShortCut, true));
    connect(m_scriptEdit, &GtpyScriptEditor::interruptShortcutTriggered, this,
            std::bind(handleShortCut, false));
    connect(m_scriptEdit, &GtpyScriptEditor::searchShortcutTriggered, this,
            [searchWid](const QString& text) {
        searchWid->setText(text);
        searchWid->enableSearch();
    });
    connect(searchWid, &GtSearchWidget::searchEnabled, this,
            [this, searchWid, backwardBtn, forwardBtn]() {
        activateSearch(*searchWid, *backwardBtn, *forwardBtn);
        m_scriptEdit->textSearchingActivated();
    });
    connect(searchWid, &GtSearchWidget::searchDisabled, this,
            [this, backwardBtn, forwardBtn]() {
        forwardBtn->setVisible(false);
        backwardBtn->setVisible(false);

        m_scriptEdit->removeSearchHighlighting();
    });
    connect(searchWid, &GtSearchWidget::textEdited, this,
            [this, searchWid, backwardBtn, forwardBtn](const QString&) {
        activateSearch(*searchWid, *backwardBtn, *forwardBtn);
    });
    connect(searchWid, &GtSearchWidget::textEdited, m_scriptEdit,
            [this](const QString& text) {
        m_scriptEdit->searchHighlighting(text);
    });
    connect(searchWid, &GtSearchWidget::returnPressed, this,
            [this]() { m_scriptEdit->moveCursorToNextFound(); });
    connect(backwardBtn, &QPushButton::clicked, this,
            [this](bool) { m_scriptEdit->moveCursorToLastFound(); });
    connect(forwardBtn, &QPushButton::clicked, this,
            [this](bool) { m_scriptEdit->moveCursorToNextFound(); });
}

GtpyScriptEditorMdi::~GtpyScriptEditorMdi()
{
    if (m_runnable) m_runnable->interrupt();
}

QIcon
GtpyScriptEditorMdi::icon() const
{
    return gt::gui::icon::file();
}

void
GtpyScriptEditorMdi::setData(GtObject* obj)
{
    m_file = qobject_cast<GtpyFile*>(obj);
    if (!m_file)
    {
        gtError().verbose() << tr("Not a python file!") << " ( "
                            << obj << " )";
        return;
    }

    m_scriptEdit->setPlainText(m_file->readAll());

    auto objName = m_file->objectName();
    setObjectName(objName);

    GtpyContextManager::instance()->setLoggingPrefix(m_contextId, objName);

    connect(obj, &QObject::destroyed, this, &GtpyScriptEditorMdi::deleteLater);

    m_scriptEdit->setContextMenuPolicy(Qt::CustomContextMenu);

    auto* updateTimer = new QTimer{m_scriptEdit};
    updateTimer->setSingleShot(true);
    updateTimer->setInterval(300);

    connect(m_scriptEdit, &QPlainTextEdit::textChanged,
            m_scriptEdit, [updateTimer]() { updateTimer->start(); });

    connect(updateTimer, &QTimer::timeout, this, [this]() {
        if (m_scriptEdit->toPlainText().toUtf8() ==
            m_file->readAll()) return;
        auto _ = gtApp->makeCommand(
            m_file, tr("%1 modified").arg(m_file->objectName()));
        QSignalBlocker blocker{m_file};
        m_file->writeAll(m_scriptEdit->toPlainText().toUtf8());
    });

    connect(m_file, qOverload<GtObject*>(&GtpyFile::dataChanged), this,
            [this](GtObject*){
        QSignalBlocker blocker{m_scriptEdit};
        m_scriptEdit->setPlainText(m_file->readAll());
    });
    connect(m_file, &GtpyFile::objectNameChanged, this,
            [this](const QString& newName){ setObjectName(newName); });

    m_scriptEdit->setReadOnly(false);
}

void
GtpyScriptEditorMdi::evalScript()
{
    if (m_isEvaluating) return;
    m_isEvaluating = true;

    // setup runnable
    m_runnable = new GtpyScriptRunnable{m_contextId};
    m_runnable->setScript(m_scriptEdit->toPlainText());
    m_runnable->setOutputToConsole(true);

    connect(m_runnable, &GtpyScriptRunnable::runnableFinished, this,
            [this](){
        m_isEvaluating = false;
        updateEvalBtn(*m_evalBtn, *m_evalLabel, m_isEvaluating);
    });

    connect(m_runnable, &GtpyScriptRunnable::runnableFinished, m_runnable,
            &GtpyScriptRunnable::deleteLater);

    // start runnable
    auto* tp = QThreadPool::globalInstance();
    tp->start(m_runnable);
}
