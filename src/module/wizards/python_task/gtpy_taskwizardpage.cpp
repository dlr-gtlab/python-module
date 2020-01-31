/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_taskwizardpage.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

// Qt includes
#include <QHBoxLayout>
#include <QSplitter>
#include <QPushButton>
#include <QMenu>
#include <QSignalMapper>

// python includes
#include "gtpy_contextmanager.h"
#include "gtpy_taskitemmodel.h"
#include "gtpy_taskstylemodel.h"
#include "gtpy_tasktreeview.h"
#include "gtpy_codegenerator.h"
#include "gtpy_scripteditor.h"

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
#include "gt_taskprovider.h"
#include "gt_processwizard.h"
#include "gt_command.h"
#include "gt_application.h"
#include "gt_deleteitemmessagebox.h"
#include "gt_textfilterdelegate.h"
#include "gt_objectmementodiff.h"
#include "gt_calculator.h"
#include "gt_processfiltermodel.h"
#include "gt_pyhighlighter.h"
#include "gt_calculatordata.h"

#include "gtpy_taskwizardpage.h"

const QString GtpyTaskWizardPage::ARROW_LEFT = " <--";
const QString GtpyTaskWizardPage::ARROW_RIGHT = "-->";

GtpyTaskWizardPage::GtpyTaskWizardPage() :
    GtpyAbstractScriptingWizardPage(GtpyContextManager::TaskEditorContext),
    m_task(Q_NULLPTR),
    m_treeView(Q_NULLPTR),
    m_calcModel(Q_NULLPTR),
    m_styledModel(Q_NULLPTR),
    m_filterModel(Q_NULLPTR),
    m_actionMapper(new QSignalMapper(this))
{
    m_treeView = new GtpyTaskTreeView(this);

    m_treeView->setFrameStyle(defaultFrameStyle());

    QPushButton* addElementButton = new QPushButton(tr("Add..."));
    addElementButton->setIcon(gtApp->icon("addIcon_16.png"));
    addElementButton->setToolTip(tr("Add New Element"));
    addElementButton->setStyleSheet(GtStyleSheets::buttonStyleSheet());

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
GtpyTaskWizardPage::initialization()
{
    acceptCalculatorDrops(true);

    GtProcessWizard* wiz = findParentWizard();

    if (wiz)
    {
        wiz->setWindowModality(Qt::NonModal);

    }

    GtObjectMemento memento = provider()->componentData();

    if (memento.isNull())
    {
        return;
    }

    if (m_task != Q_NULLPTR)
    {
        delete m_task;
    }

    m_task = memento.restore<GtpyTask*>(gtProcessFactory);

    if (m_task == Q_NULLPTR)
    {
        return;
    }

    m_task->setParent(this);

    if (gtDataModel->objectByUuid(m_task->uuid()) == Q_NULLPTR)
    {
        enableSaving(false);
    }

    enableCalculators(m_task);

    m_task->setFlag(GtObject::NewlyCreated, false);
    m_task->setFlag(GtObject::HasOwnChanges, false);
    m_task->setFlag(GtObject::HasChildChanges, false);

    foreach (GtObject* child, m_task->findChildren<GtObject*>())
    {
        if (child != Q_NULLPTR)
        {
            child->setFlag(GtObject::NewlyCreated, false);
            child->setFlag(GtObject::HasOwnChanges, false);
            child->setFlag(GtObject::HasChildChanges, false);
        }
    }

    m_treeView->setHeaderHidden(true);

    m_calcModel = new GtpyTaskItemModel(m_task, m_treeView);

    /// Own styled model created to enable editing calculator names
    m_styledModel = new GtpyTaskStyleModel(m_treeView);
    m_styledModel->setSourceModel(m_calcModel);
    m_styledModel->setRootTask(m_task);

    m_filterModel = new GtProcessFilterModel(m_treeView);
    m_filterModel->setSourceModel(m_styledModel);

    m_treeView->setModel(m_filterModel);

    m_treeView->expandAll();

    setPlainTextToEditor(m_task->script());

    setPackageNames(m_task->packageNames());

    connect(m_calcModel, SIGNAL(processComponentRenamed(
                                    QString, QString, QString)), this,
            SLOT(onProcessComponentRenamed(QString, QString, QString)));
    connect(m_task, SIGNAL(childAppended(GtObject*, GtObject*)), this,
            SLOT(calculatorAppendedToTask(GtObject*, GtObject*)));
    connect(this, SIGNAL(calculatorDropReceived(GtCalculator*)), this,
            SLOT(onCalculatorDropReceived(GtCalculator*)));
}

bool
GtpyTaskWizardPage::validation()
{
    if (m_task == Q_NULLPTR)
    {
        return false;
    }

    m_task->setScript(editorText());

    QObjectList children = m_task->children();

    foreach (QObject* child, children)
    {
        child->setParent(Q_NULLPTR);
        delete child;
        child = Q_NULLPTR;
    }

    provider()->setComponentData(m_task->toMemento());


    GtProcessWizard* wiz = findParentWizard();

    if (wiz)
    {
        wiz->setWindowModality(Qt::ApplicationModal);
    }

    return true;
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
    if (calc == Q_NULLPTR)
    {
        return;
    }

    GtProject* project = gtDataModel->currentProject();

    if (project == Q_NULLPTR)
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
        QString headline;

        QString className = calc->metaObject()->className();
        headline = calc->objectName();

        onProcessComponentRenamed(className, calcNameBefor, calcNameNew);
    }

    QString headline = " " + calcNameNew + " ";

    int newHeadlineSize = headline.size();

    headline = ("#" + GtpyTaskWizardPage::ARROW_LEFT + headline+
                   GtpyTaskWizardPage::ARROW_RIGHT);

    QString caption;

    for (int i = 0; i < newHeadlineSize; i++)
    {
        caption.append("-");
    }

    caption = ("#" + GtpyTaskWizardPage::ARROW_LEFT + caption +
               GtpyTaskWizardPage::ARROW_RIGHT);

    QString pyCode = GtpyCodeGenerator::instance()->calculatorPyCode(calc);

    int lastLineBreak = pyCode.lastIndexOf(QChar('\n'));
    pyCode.remove(lastLineBreak, 1);

    replaceCalcPyCode(headline, caption, pyCode);
}

void
GtpyTaskWizardPage::addCalculator()
{
    GtProject* project = gtDataModel->currentProject();

    if (project == Q_NULLPTR)
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

    if (calc == Q_NULLPTR)
    {
        return;
    }

    gtDebug() << "Calculator appended! (" << calc->metaObject()->className()
              << ")";

    updateLastUsedElementList(calc->metaObject()->className());

    if (appendCalcToTask(calc))
    {
        insertConstructor(calc);
    }
    else
    {
        delete calc;
        calc = Q_NULLPTR;
    }
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
    if (calc == Q_NULLPTR)
    {
        return;
    }

    QString objName = " " + calc->objectName() + " ";

    QString pyCode = ("#" + GtpyTaskWizardPage::ARROW_LEFT + objName +
                      GtpyTaskWizardPage::ARROW_RIGHT + "\n");

    pyCode += GtpyCodeGenerator::instance()->calculatorPyCode(calc);

    QString caption;

    for (int i = 0; i < objName.size(); i++)
    {
        caption.append("-");
    }

    pyCode += ("#" + GtpyTaskWizardPage::ARROW_LEFT + caption +
               GtpyTaskWizardPage::ARROW_RIGHT + "\n");

    insertToCurrentCursorPos(pyCode + "\n");

    evalScript(pyCode, false);
    showEvalButton(false);
}

bool
GtpyTaskWizardPage::appendCalcToTask(GtCalculator* calc)
{
    if (m_task == Q_NULLPTR)
    {
        return false;
    }

    if (calc == Q_NULLPTR)
    {
        return false;
    }

    QString objName = calc->objectName();

    int sameNamedCalcs = 1;
    int iterations = 1;

    while (sameNamedCalcs > 0)
    {
        sameNamedCalcs =
                m_task->findDirectChildren<GtCalculator*>(objName).count();

        if (sameNamedCalcs > 0)
        {
            objName = calc->objectName();
            objName = objName + " " + QString::number(iterations);
        }

        iterations++;
    }

    calc->setObjectName(objName);

    m_task->appendChild(calc);

    if (m_calcModel != Q_NULLPTR &&
            m_treeView != Q_NULLPTR)
    {
        m_treeView->setFocus();
        m_calcModel->updateView();
        m_treeView->expandAll();
    }

    return true;
}

void
GtpyTaskWizardPage::onDoubleClicked(const QModelIndex& index)
{
    if (!index.isValid())
    {
        return;
    }

    if (m_task == Q_NULLPTR)
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
    if (m_task == Q_NULLPTR)
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

        if (obj == Q_NULLPTR)
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

                foreach (GtProcessComponent* comp, pComList)
                {
                    if (comp == obj)
                    {
                        delete comp;
                        comp = Q_NULLPTR;

                        if (m_calcModel != Q_NULLPTR && m_treeView != Q_NULLPTR)
                        {
                            m_calcModel->updateView();
                            m_treeView->expandAll();
                        }

                        break;
                    }
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
GtpyTaskWizardPage::onProcessComponentRenamed(QString className,
        QString oldName, QString newName)
{
    QString oldHeadline = " " + oldName + " ";

    int oldHeadlineSize = oldHeadline.size();

    oldHeadline = ("#" + GtpyTaskWizardPage::ARROW_LEFT + oldHeadline +
               GtpyTaskWizardPage::ARROW_RIGHT);

    QString newHeadline = " " + newName + " ";

    int newHeadlineSize = newHeadline.size();

    newHeadline = ("#" + GtpyTaskWizardPage::ARROW_LEFT + newHeadline+
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
    addcalc->setIcon(gtApp->icon(QStringLiteral("calculatorIcon_16.png")));

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
                if (gtApp->devMode() ||
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

                    if (extendedData == Q_NULLPTR ||
                            extendedData->icon.isNull())
                    {
                        act->setIcon(gtApp->icon(QStringLiteral(
                                                     "calculatorIcon_16.png")));
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
    if (gtApp->currentProject() == Q_NULLPTR)
    {
        return;
    }

    QAction* act = qobject_cast<QAction*>(obj);

    if (act == Q_NULLPTR)
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

        if (newObj == Q_NULLPTR)
        {
            return;
        }

        GtCalculator* calc = qobject_cast<GtCalculator*>(newObj);

        if (calc == Q_NULLPTR)
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

        if (appendCalcToTask(calc))
        {
            insertConstructor(calc);
        }
        else
        {
            delete calc;
        }
    }
}

void
GtpyTaskWizardPage::calculatorAppendedToTask(GtObject* child, GtObject* parent)
{
    if (child == Q_NULLPTR)
    {
        return;
    }

    if (parent == Q_NULLPTR)
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

    if (calc == Q_NULLPTR)
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

    if (calc == Q_NULLPTR)
    {
        return;
    }

    m_calcIds.remove(calc->uuid());
}

void
GtpyTaskWizardPage::saveScript()
{
    if (m_task == Q_NULLPTR)
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

void
GtpyTaskWizardPage::onCalculatorDropReceived(GtCalculator* calc)
{
    cursorToNewLine();

    if (appendCalcToTask(calc))
    {
        insertConstructor(calc);
    }

    delete calc;
    calc = Q_NULLPTR;
}

void
GtpyTaskWizardPage::endEval(bool /*success*/)
{
    if (m_calcModel != Q_NULLPTR &&
            m_task != Q_NULLPTR &&
            m_treeView != Q_NULLPTR)
    {
        m_calcModel->updateView();
        m_treeView->expandAll();
    }
}
