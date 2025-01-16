/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_taskwizardpage.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 12.08.2019
 * Author: Marvin Noethen (DLR AT-TWK)
 */

// Qt includes
#include <QHBoxLayout>
#include <QSplitter>
#include <QPushButton>
#include <QMenu>
#include <QSignalMapper>

// python includes
#include "gtpy_codegen.h"
#include "gtpy_icons_compat.h"
#include "gtpy_contextmanager.h"
#include "gtpy_taskstylemodel.h"
#include "gtpy_tasktreeview.h"
#include "gtpy_scripteditor.h"
#include "gtpy_objectmodel.h"
#include "gtpy_transfer.h"

// GTlab framework includes
#include "gt_stylesheets.h"
#include "gt_settings.h"
#include "gt_processfactory.h"
#include "gt_calculatorfactory.h"
#include "gt_extendedcalculatordata.h"
#include "gt_objectmemento.h"
#include "gt_abstractprocessprovider.h"
#include "gt_datamodel.h"
#include "gt_project.h"
#include "gt_calculatorprovider.h"
#include "gt_processwizard.h"
#include "gt_command.h"
#include "gt_application.h"
#include "gt_deleteitemmessagebox.h"
#include "gt_calculator.h"
#include "gt_processfiltermodel.h"
#include "gt_pyhighlighter.h"
#include "gt_calculatordata.h"

#include "gtpy_taskwizardpage.h"

const QString GtpyTaskWizardPage::ARROW_LEFT = " <--";
const QString GtpyTaskWizardPage::ARROW_RIGHT = "-->";

GtpyTaskWizardPage::GtpyTaskWizardPage() :
    GtpyAbstractScriptingWizardPage(GtpyContextManager::TaskEditorContext),
    m_task(nullptr),
    m_treeView(nullptr),
    m_calcModel(nullptr),
    m_styledModel(nullptr),
    m_filterModel(nullptr),
    m_actionMapper(new QSignalMapper(this))
{
    m_treeView = new GtpyTaskTreeView(this);

    m_treeView->setFrameStyle(defaultFrameStyle());

    QPushButton* addElementButton = new QPushButton(tr("Add..."));
    addElementButton->setToolTip(tr("Add New Element"));

#if GT_VERSION < GT_VERSION_CHECK(2, 0, 0) 
    addElementButton->setStyleSheet(GtStyleSheets::buttonStyleSheet());
#else
    addElementButton->setStyleSheet(gt::gui::stylesheet::buttonStyleSheet());
#endif

    addElementButton->setIcon(GTPY_ICON(add));

    QVBoxLayout* treeViewLay = new QVBoxLayout;

    treeViewLay->setSpacing(1);
    treeViewLay->setMargin(0);
    treeViewLay->setContentsMargins(0, 0, 0, 0);

    treeViewLay->addWidget(addElementButton);
    treeViewLay->addWidget(m_treeView);

    QWidget* wid = new QWidget;
    wid->setLayout(treeViewLay);

    insertWidgetNextToEditor(wid);

    connect(m_treeView, SIGNAL(doubleClicked(QModelIndex)),
            SLOT(onDoubleClicked(QModelIndex)));
    connect(m_treeView, SIGNAL(deleteProcessElements(QList<QModelIndex>)),
            SLOT(deleteProcessElements(QList<QModelIndex>)));

    connect(addElementButton, SIGNAL(clicked(bool)), SLOT(addElement()));
    connect(m_actionMapper, SIGNAL(mapped(QObject*)),
            SLOT(actionTriggered(QObject*)));

    GtpyScriptEditor* calcEditor = new GtpyScriptEditor(m_contextId, this);

    QTextOption defaultOps = calcEditor->document()->defaultTextOption();
    defaultOps.setFlags(defaultOps.flags() | QTextOption::ShowTabsAndSpaces /*|
                        QTextOption::ShowLineAndParagraphSeparators*/);

    calcEditor->document()->setDefaultTextOption(defaultOps);
    calcEditor->setStyleSheet("QPlainTextEdit {  border: 0px; }");

    GtPyHighlighter* highlighter = new GtPyHighlighter(calcEditor->document());

    Q_UNUSED(highlighter)

    calcEditor->hide();

    //    addTabWidget(calcEditor, "Calculators");
}

void
GtpyTaskWizardPage::endEval(bool /*success*/)
{
    if (m_task && m_treeView)
    {
        m_treeView->expandAll();
    }
}

void
GtpyTaskWizardPage::initialization()
{
    acceptCalculatorDrops(true);

    GtObjectMemento memento = provider()->componentData();

    if (memento.isNull())
    {
        return;
    }

    if (m_task)
    {
        delete m_task;
    }

    m_task = memento.restore<GtpyTask*>(gtProcessFactory);

    if (!m_task)
    {
        return;
    }

    m_task->setParent(this);

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    gtpy::transfer::propStructToPython(m_contextId, m_task->inputArgs());
    gtpy::transfer::propStructToPython(m_contextId, m_task->outputArgs());
#endif

    if (!gtDataModel->objectByUuid(m_task->uuid()))
    {
        enableSaving(false);
    }

    enableCalculators(m_task);

    m_task->setFlag(GtObject::NewlyCreated, false);
    m_task->setFlag(GtObject::HasOwnChanges, false);
    m_task->setFlag(GtObject::HasChildChanges, false);

    foreach (GtObject* child, m_task->findChildren<GtObject*>())
    {
        if (child)
        {
            child->setFlag(GtObject::NewlyCreated, false);
            child->setFlag(GtObject::HasOwnChanges, false);
            child->setFlag(GtObject::HasChildChanges, false);
        }
    }

    m_treeView->setHeaderHidden(true);

    m_calcModel = new GtpyObjectModel(m_treeView);
    m_calcModel->setRootObject(m_task);

    /// Own styled model created to enable editing calculator names
    m_styledModel = new GtpyTaskStyleModel(m_treeView);
    m_styledModel->setSourceModel(m_calcModel);
    m_styledModel->setRootTask(m_task);

    m_filterModel = new GtProcessFilterModel(m_treeView);
    m_filterModel->setSourceModel(m_styledModel);

    m_treeView->setModel(m_filterModel);

    m_treeView->expandAll();

    setPlainTextToEditor(m_task->script());

    createSettings();

    connect(m_calcModel, SIGNAL(
                processComponentRenamed(QString,QString,QString)), this,
            SLOT(onProcessComponentRenamed(QString,QString,QString)));
    connect(m_task, SIGNAL(childAppended(GtObject*,GtObject*)), this,
            SLOT(calculatorAppendedToTask(GtObject*,GtObject*)));
    connect(m_task, SIGNAL(dataChanged(GtObject*)), m_calcModel,
            SLOT(update()));
    connect(m_task, SIGNAL(dataChanged(GtObject*)), m_treeView,
            SLOT(expandAll()));
    connect(this, SIGNAL(calculatorDropReceived(GtCalculator*)), this,
            SLOT(onCalculatorDropReceived(GtCalculator*)));
}

bool
GtpyTaskWizardPage::validation()
{
    if (!m_task)
    {
        return false;
    }

    m_task->setScript(editorText());

    QObjectList children = m_task->children();

    foreach (QObject* child, children)
    {
        child->setParent(nullptr);
        delete child;
        child = nullptr;
    }

    provider()->setComponentData(m_task->toMemento());

    return true;
}

void
GtpyTaskWizardPage::saveScript()
{
    if (!m_task)
    {
        return;
    }

    m_task->setScript(editorText());

    GtObjectMemento memento = m_task->toMemento();

    if (memento.isNull())
    {
        return;
    }

    GtObject* obj = gtDataModel->objectByUuid(m_task->uuid());

    if (GtTask* task = qobject_cast<GtTask*>(obj))
    {
        GtCommand command =
            gtApp->startCommand(gtApp->currentProject(),
                                task->objectName() +
                                tr(" configuration changed"));
        task->fromMemento(memento);
        gtApp->endCommand(command);
    }
}

QString
GtpyTaskWizardPage::componentUuid() const
{
    QString uuid;

    if (m_task)
    {
        uuid = m_task->uuid();
    }

    return uuid;
}

void
GtpyTaskWizardPage::setComponentName(const QString& name)
{
    if (m_task)
    {
        m_task->setObjectName(name);
    }
}

GtpyEditorSettings*
GtpyTaskWizardPage::createSettings()
{
    GtpyEditorSettings* pref = nullptr;

    if (m_task)
    {
        pref = new GtpyEditorSettings(this);

        if (m_task->tabSize() <= 0)
        {
            pref->setTabSize(4);
            pref->setReplaceTabBySpace(false);
        }
        else
        {
            pref->setTabSize(m_task->tabSize());
            pref->setReplaceTabBySpace(m_task->replaceTabBySpaces());
        }
    }

    return pref;
}

void
GtpyTaskWizardPage::saveSettings(GtpyEditorSettings* pref)
{
    if (pref && m_task)
    {
        m_task->setTabSize(pref->tabSize());
        m_task->setReplaceTabBySpaces(pref->replaceTabBySpace());
    }
}

QModelIndex
GtpyTaskWizardPage::mapToSource(const QModelIndex& index)
{
    if (!index.isValid())
    {
        return QModelIndex();
    }

    QModelIndex tmp1 = m_filterModel->mapToSource(index);

    return m_styledModel->mapToSource(tmp1);
}

void
GtpyTaskWizardPage::configCalculator(GtCalculator* calc)
{
    if (!calc)
    {
        return;
    }

    GtProject* project = gtDataModel->currentProject();

    if (!project)
    {
        return;
    }

    QString calcNameBefor = calc->objectName();

    GtCalculatorProvider provider(calc);
    GtProcessWizard wizard(project, &provider);

    if (!wizard.exec())
    {
        return;
    }

    GtObjectMemento memento = provider.componentData();

    if (memento.isNull())
    {
        return;
    }

    calc->fromMemento(memento);

    QString calcNameNew = calc->objectName();

    if (calcNameNew != calcNameBefor)
    {
        QString className = calc->metaObject()->className();
        onProcessComponentRenamed(className, calcNameBefor, calcNameNew);
    }

    QString headline = " " + calcNameNew + " ";

    int newHeadlineSize = headline.size();

    headline = ("#" + GtpyTaskWizardPage::ARROW_LEFT + headline +
                GtpyTaskWizardPage::ARROW_RIGHT);

    QString caption;

    for (int i = 0; i < newHeadlineSize; i++)
    {
        caption.append("-");
    }

    caption = ("#" + GtpyTaskWizardPage::ARROW_LEFT + caption +
               GtpyTaskWizardPage::ARROW_RIGHT);

    QString pyCode = gtpy::codegen::calcToPyCode(calc);

    int lastLineBreak = pyCode.lastIndexOf(QChar('\n'));
    pyCode.remove(lastLineBreak, 1);

    replaceCalcPyCode(headline, caption, pyCode);
}

void
GtpyTaskWizardPage::addCalculator()
{
    GtProject* project = gtDataModel->currentProject();

    if (!project)
    {
        return;
    }

    GtCalculatorProvider provider;
    GtProcessWizard wizard(project, &provider);
    wizard.resize(560, 500);

    if (!wizard.exec())
    {
        return;
    }

    GtObjectMemento memento = provider.componentData();

    if (memento.isNull())
    {
        return;
    }

    GtProcessComponent* component = memento.restore<GtProcessComponent*>(
                                        gtProcessFactory);

    GtCalculator* calc = qobject_cast<GtCalculator*>(component);

    if (!calc)
    {
        return;
    }

    gtDebug() << "Calculator appended! (" << calc->metaObject()->className()
              << ")";

    updateLastUsedElementList(calc->metaObject()->className());

    insertConstructor(calc);

    delete calc;
}

void
GtpyTaskWizardPage::updateLastUsedElementList(const QString& str)
{
    if (str.isEmpty())
    {
        return;
    }

    QStringList list = gtApp->settings()->lastProcessElements();

    if (list.contains(str))
    {
        int idx = list.indexOf(str);

        if (idx > 0)
        {
            list.move(idx, 0);
        }
    }
    else
    {
        list.prepend(str);
    }

    if (list.size() > 10)
    {
        list.removeLast();
    }

    gtApp->settings()->setLastProcessElements(list);
}

void
GtpyTaskWizardPage::insertConstructor(GtCalculator* calc)
{
    if (!calc)
    {
        return;
    }

    QString objName = " " + calc->objectName() + " ";

    QString pyCode = ("#" + GtpyTaskWizardPage::ARROW_LEFT + objName +
                      GtpyTaskWizardPage::ARROW_RIGHT + "\n");

    pyCode += gtpy::codegen::calcToPyCode(calc);

    QString caption;

    for (int i = 0; i < objName.size(); i++)
    {
        caption.append("-");
    }

    pyCode += ("#" + GtpyTaskWizardPage::ARROW_LEFT + caption +
               GtpyTaskWizardPage::ARROW_RIGHT + "\n");

    insertToCurrentCursorPos(pyCode);

    evalScript(pyCode, false);
    showEvalButton(false);
}

void
GtpyTaskWizardPage::onDoubleClicked(const QModelIndex& index)
{
    if (!index.isValid())
    {
        return;
    }

    if (!m_task)
    {
        return;
    }

    QModelIndex srcIndex = mapToSource(index);

    GtObject* obj = m_calcModel->objectFromIndex(srcIndex);

    if (GtCalculator* calc = qobject_cast<GtCalculator*>(obj))
    {
        if (m_task == calc->parent())
        {
            configCalculator(calc);
        }
    }
}

void
GtpyTaskWizardPage::deleteProcessElements(const QList<QModelIndex>& indexList)
{
    if (!m_task)
    {
        return;
    }

    if (indexList.isEmpty())
    {
        return;
    }

    QList<GtObject*> objects;

    foreach (QModelIndex index, indexList)
    {
        if (!index.isValid())
        {
            continue;
        }

        QModelIndex srcIndex = mapToSource(index);

        if (!srcIndex.isValid())
        {
            continue;
        }

        if (srcIndex.model() != m_calcModel)
        {
            continue;
        }

        GtObject* obj = m_calcModel->objectFromIndex(srcIndex);

        if (!obj)
        {
            continue;
        }

        if (obj == m_task)
        {
            continue;
        }

        if (!objects.contains(obj))
        {
            objects.append(obj);
        }
    }

    if (objects.size() <= 0)
    {
        return;
    }

    QString str1;
    QString str2;

    if (objects.size() == 1)
    {
        str1 = tr("Delete Item");
        str2 = objects.first()->objectName();
    }
    else
    {
        str1 = tr("Delete Items");
        str2 = tr("Process Elements");
    }

    GtDeleteItemMessageBox mb(str1, str2);

    int ret = mb.exec();

    switch (ret)
    {
        case QMessageBox::Yes:
        {
            foreach (GtObject* obj, objects)
            {
                QList<GtProcessComponent*> pComList =
                    m_task->findChildren<GtProcessComponent*>(obj->objectName());

                auto objIter = std::find(std::begin(pComList), std::end(pComList), obj);
                if (objIter == std::end(pComList)) continue;

                delete *objIter;

                if (m_calcModel && m_treeView)
                {
                    m_treeView->expandAll();
                }
            }
        }

        case QMessageBox::Cancel:
        {
            break;
        }

        default:
            break;
    }
}

void
GtpyTaskWizardPage::onProcessComponentRenamed(QString const& className,
        QString const& oldName, QString const& newName)
{
    QString oldHeadline = " " + oldName + " ";

    int oldHeadlineSize = oldHeadline.size();

    oldHeadline = ("#" + GtpyTaskWizardPage::ARROW_LEFT + oldHeadline +
                   GtpyTaskWizardPage::ARROW_RIGHT);

    QString newHeadline = " " + newName + " ";

    int newHeadlineSize = newHeadline.size();

    newHeadline = ("#" + GtpyTaskWizardPage::ARROW_LEFT + newHeadline +
                   GtpyTaskWizardPage::ARROW_RIGHT);

    QString oldCaption;

    for (int i = 0; i < oldHeadlineSize; i++)
    {
        oldCaption.append("-");
    }

    oldCaption = ("#" + GtpyTaskWizardPage::ARROW_LEFT + oldCaption +
                  GtpyTaskWizardPage::ARROW_RIGHT);

    QString newCaption;

    for (int i = 0; i < newHeadlineSize; i++)
    {
        newCaption.append("-");
    }

    newCaption = ("#" + GtpyTaskWizardPage::ARROW_LEFT + newCaption +
                  GtpyTaskWizardPage::ARROW_RIGHT);

    replaceBlockHeaders(oldHeadline, newHeadline, oldCaption, newCaption);

    //    QString search = className + "(\"" + oldName + "\")";
    //    QString replace = className + "(\"" + newName + "\")";

    //    searchAndReplaceEditorText(search, replace, true);

    QString pattern = "(" + className + "\\( *\"" + oldName + "\" *\\))";
    QString replace = className + "(\"" + newName + "\")";

    searchAndReplaceEditorText(QRegExp(pattern), replace, true);
}

void
GtpyTaskWizardPage::addElement()
{
    QMenu menu(this);

    QAction* addcalc = menu.addAction(tr("New Calculator..."));

    addcalc->setIcon(GTPY_ICON(calculator));

    QStringList list = gtApp->settings()->lastProcessElements();

    int count = list.size();

    if (count >= 5)
    {
        count = 5;
    }

    for (int i = 0; i < count; i++)
    {
        if (i == 0)
        {
            menu.addSeparator();
        }

        QString entry = list[i];

        if (gtCalculatorFactory->calculatorDataExists(entry))
        {
            // entry is a calculator
            GtCalculatorData calcData =
                gtCalculatorFactory->calculatorData(entry);

            if (calcData->isValid())
            {
                if (gtDevMode() ||
                        calcData->status == GtCalculatorDataImpl::RELEASE)
                {
                    QAction* act = menu.addAction(calcData->id);
                    act->setData(entry);

                    connect(act, SIGNAL(triggered(bool)),
                            m_actionMapper, SLOT(map()),
                            Qt::UniqueConnection);
                    m_actionMapper->setMapping(act, act);

                    GtExtendedCalculatorDataImpl* extendedData =
                        dynamic_cast<GtExtendedCalculatorDataImpl*>(
                            calcData.get());

                    if (!extendedData || extendedData->icon.isNull())
                    {
                        act->setIcon(GTPY_ICON(calculator));
                    }
                    else
                    {
                        act->setIcon(extendedData->icon);
                    }
                }
            }
        }
    }

    QAction* a = menu.exec(QCursor::pos());

    if (a == addcalc)
    {
        addCalculator();
    }
}

void
GtpyTaskWizardPage::actionTriggered(QObject* obj)
{
    if (!gtApp->currentProject())
    {
        return;
    }

    QAction* act = qobject_cast<QAction*>(obj);

    if (!act)
    {
        return;
    }

    QString className = act->data().toString();

    if (className.isEmpty())
    {
        return;
    }

    if (gtCalculatorFactory->calculatorDataExists(className))
    {
        GtCalculatorData calcData =
            gtCalculatorFactory->calculatorData(className);

        if (!calcData->isValid())
        {
            return;
        }

        QObject* newObj = calcData->metaData().newInstance();

        if (!newObj)
        {
            return;
        }

        GtCalculator* calc = qobject_cast<GtCalculator*>(newObj);

        if (!calc)
        {
            delete newObj;
            return;
        }

        calc->setFactory(gtProcessFactory);

        GtCalculatorProvider provider(calc);
        GtProcessWizard wizard(gtApp->currentProject(), &provider);
        wizard.setWindowTitle(tr("Config ") + calcData->id);

        if (!wizard.exec())
        {
            delete calc;
            return;
        }

        GtObjectMemento memento = provider.componentData();

        if (memento.isNull())
        {
            delete calc;
            return;
        }

        calc->fromMemento(memento);

        QString lastUsedClassName = newObj->metaObject()->className();

        if (lastUsedClassName.isEmpty())
        {
            return;
        }

        updateLastUsedElementList(lastUsedClassName);

        insertConstructor(calc);

        delete calc;
    }
}

void
GtpyTaskWizardPage::calculatorAppendedToTask(GtObject* child, GtObject* parent)
{
    if (!child)
    {
        return;
    }

    if (!parent)
    {
        return;
    }

    if (parent == m_task)
    {
        m_calcIds.insert(child->uuid(), child->objectName());

        connect(child, SIGNAL(objectNameChanged(QString)), this,
                SLOT(calculatorNameChanged(QString)));
        connect(child, SIGNAL(destroyed(QObject*)), this,
                SLOT(calculatorDestroyed(QObject*)));
    }
}

void
GtpyTaskWizardPage::calculatorNameChanged(const QString& name)
{
    GtObject* calc = qobject_cast<GtObject*>(sender());

    if (!calc)
    {
        return;
    }

    const QString calcClassName = calc->metaObject()->className();
    const QString oldName = m_calcIds.value(calc->uuid());

    m_calcIds.insert(calc->uuid(), name);

    onProcessComponentRenamed(calcClassName, oldName, name);
}

void
GtpyTaskWizardPage::calculatorDestroyed(QObject* obj)
{
    GtObject* calc = qobject_cast<GtObject*>(obj);

    if (!calc)
    {
        return;
    }

    m_calcIds.remove(calc->uuid());
}

void
GtpyTaskWizardPage::onCalculatorDropReceived(GtCalculator* calc)
{
    cursorToNewLine();

    insertConstructor(calc);

    delete calc;
}
