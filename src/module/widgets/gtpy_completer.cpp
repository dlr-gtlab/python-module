/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_completer.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QAbstractItemView>
#include <QScrollBar>
#include <QToolTip>
#include <QMultiMap>

#include "gtpy_completermodel.h"

#include "gtpy_completer.h"

GtpyCompleter::GtpyCompleter(int contextId, QWidget* widget) :
    QCompleter(widget)
{
    setObjectName("Python Completer");
    m_contextId = contextId;

    setCompletionMode(QCompleter::PopupCompletion);
    setCaseSensitivity(Qt::CaseInsensitive);
    setCompletionRole(Qt::DisplayRole);

    m_model = new GtpyCompleterModel(this);

    setModelSorting(QCompleter::CaseInsensitivelySortedModel);

    setModel(m_model);
    setWidget(widget);

    connect(this, SIGNAL(highlighted(QModelIndex)), this,
            SLOT(completionToolTip(QModelIndex)));
}

void
GtpyCompleter::handleTabCompletion(QTextCursor textCursor, QRect cursorRect)
{
    if (m_model == Q_NULLPTR)
    {
        return;
    }

    static int spaceCount = 0;
    int pos = textCursor.position();

    textCursor.select(QTextCursor::LineUnderCursor);

    pos -= textCursor.selectionStart();

    QString line = textCursor.selectedText();
    QString textToComplete = getTextToComplete(pos, line, spaceCount);

    spaceCount = 0;

    if (textToComplete.isEmpty())
    {
        QCompleter::popup()->hide();
        return;
    }

    QString objToLookup;

    int dot = textToComplete.lastIndexOf('.');

    if (dot != -1)
    {
        objToLookup    = textToComplete.mid(0, dot);
        textToComplete = textToComplete.mid(dot + 1, pos);
    }

    QMultiMap<QString, GtpyFunction> completions;
    completions = getCompletions(textToComplete, objToLookup);

    if (completions.isEmpty())
    {
        QCompleter::popup()->hide();
        return;
    }
    else
    {
        m_model->setFound(completions);

        m_indexOfCompletion += dot + 1;
        m_sizeOfCompletion   = textToComplete.size();

        cursorRect.setWidth(QCompleter::popup()->sizeHintForColumn(0) +
                            QCompleter::popup()->verticalScrollBar()->
                            sizeHint().width());

        cursorRect.translate(0, 8);

        complete(cursorRect);
    }

    foreach (QString name, completions.keys())
    {
        if (name.contains(' '))
        {
            spaceCount = name.count(' ');
            return;
        }
    }
}

QString
GtpyCompleter::getTextToComplete(int pos, QString line, int spaces)
{
    if (line.isEmpty() || pos < 0 || spaces < 0)
    {
        return QString();
    }

    static int firstIndex = 0;

    QString pattern = QString("(?(DEFINE)(?<qoutes>\".*\"|\'.*\'|#.*))"
                              "(?(DEFINE)(?<braces>\\(.*\\)|\\[.*\\]|{.*}))"
                              "(?(DEFINE)(?<words>\\w+[-\\w]*(\\s|$)))"
                              "(?(qoutes)\\K|"
                              "(?(?=(?&words){1," +
                              QString::number(spaces + 1) + "})"
                              "(?&words){1," +
                              QString::number(spaces + 1) + "}|"
                              "(\\w+(?&braces)?[\\.-]?)+\\s?(?&words){0," +
                              QString::number(spaces) + "})"
                              ")");

    QRegularExpression regExp;

    regExp.setPattern(pattern);

    line = line.left(pos);

    firstIndex = line.lastIndexOf(regExp);

    QRegularExpressionMatch match = regExp.match(line, firstIndex);

    if (match.capturedEnd() < pos)
    {
        return QString();
    }

    m_indexOfCompletion = firstIndex;

    return match.captured();
}

QMultiMap<QString, GtpyFunction>
GtpyCompleter::getCompletions(QString textToCompare, QString objToLookup)
{
    if (textToCompare.isEmpty() && objToLookup.isEmpty())
    {
        return QMultiMap<QString, GtpyFunction>();
    }

    textToCompare = textToCompare.toLower();

    static QString oldTextToCompare;

    static QString oldObjToLookup;

    static QMultiMap<QString, GtpyFunction> completions;

    QMultiMap<QString, GtpyFunction> map;

    if (textToCompare == QStringLiteral("import") ||
            textToCompare == QStringLiteral("from"))
    {
        GtpyContextManager* python = GtpyContextManager::instance();
        map = python->introspection(m_contextId, objToLookup, true);
    }
    else if (completions.isEmpty() || oldTextToCompare.isEmpty() ||
             !textToCompare.startsWith(oldTextToCompare) ||
             oldObjToLookup != objToLookup)
    {
        GtpyContextManager* python = GtpyContextManager::instance();

        if (textToCompare.contains(QRegularExpression("from|import")))
        {
            map = python->introspection(m_contextId, objToLookup, true);
        }
        else
        {
            map = python->introspection(m_contextId, objToLookup);
        }
    }
    else
    {
        map = completions;
    }

    completions.clear();

    for (int i = 0; i < map.size(); i++)
    {
        QString name = map.keys().at(i);

        if (name.toLower().startsWith(textToCompare.toLower()))
        {
            completions.insert(name, map.values().at(i));
        }
    }

    oldTextToCompare = textToCompare;
    oldObjToLookup   = objToLookup;

    return completions;
}

QAbstractItemView*
GtpyCompleter::getPopup() const
{
    return QCompleter::popup();
}

QString
GtpyCompleter::completionText(const QModelIndex& index) const
{
    if (m_model == Q_NULLPTR)
    {
        return QString();
    }

    return m_model->completion(index);
}

QString
GtpyCompleter::completionName(const QModelIndex& index) const
{
    if (m_model == Q_NULLPTR)
    {
        return QString();
    }

    return m_model->completionName(index);
}

int
GtpyCompleter::completionCursorOffset(const QModelIndex& index) const
{
    if (m_model == Q_NULLPTR)
    {
        return 0;
    }

    return m_model->completionCursorOffset(index);
}

QModelIndex
GtpyCompleter::firstIndex() const
{
    return QCompleter::currentIndex();
}

QModelIndex
GtpyCompleter::currentIndex() const
{
    return QCompleter::popup()->currentIndex();
}

void
GtpyCompleter::insertCompletion(QTextCursor& textCursor)
{
    QModelIndex index = currentIndex();

    if (!index.isValid())
    {
        index = firstIndex();

        if (!index.isValid())
        {
            return;
        }
    }

    QString completion = completionText(index);
    int cursorOffset   = completionCursorOffset(index);

    QTextCursor tc = textCursor;
    tc.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);

    if (tc.selectedText() == ".")
    {
        tc = textCursor;
    }
    else
    {
        tc = textCursor;

        tc.select(QTextCursor::LineUnderCursor);

        int indexOfLine = tc.selectionStart();

        QString selection = tc.selectedText().mid(m_indexOfCompletion);

        // obtain the distance of insertion start and end
        int offset = compareStrings(selection, completion);

        if (offset == 0)
        {
            QString name = completionName(index);

            offset = compareStrings(selection, name);

            if (offset == 0)
            {
                offset = m_sizeOfCompletion;
            }
        }
        else
        {
            if (!completion.isEmpty())
            {
                QString c;

                if (0 <= offset && offset < completion.size())
                {
                    c = completion.at(offset);
                }

                if (c.contains(QRegularExpression("(\\)|\\]|})")))
                {
                    completion = completion.left(offset);

                    if (cursorOffset > 0)
                    {
                        cursorOffset -= completion.size() + offset;
                    }
                }
            }
        }

        // insertion start
        tc.setPosition(m_indexOfCompletion + indexOfLine,
                       QTextCursor::MoveAnchor);
        // insertion end
        tc.setPosition(m_indexOfCompletion + offset + indexOfLine,
                       QTextCursor::KeepAnchor);
    }

    tc.insertText(completion);
    tc.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, cursorOffset);

    textCursor = tc;
}

void
GtpyCompleter::setCompletionPrefix(const QString& prefix)
{
    QCompleter::setCompletionPrefix(prefix);
}

int
GtpyCompleter::compareStrings(const QString& s1, const QString& s2)
{
    int i;

    for (i = 0; i < s1.size() && i < s2.size(); i++)
    {
        QChar s = s1.at(i).toLower();
        QChar c = s2.at(i).toLower();

        if (s == c)
        {
            continue;
        }
        else
        {
            break;
        }
    }

    return i;
}

void
GtpyCompleter::completionToolTip(const QModelIndex& index)
{
    if (QCompleter::popup()->isVisible())
    {
        QPoint pos = QCompleter::popup()->pos();
        QRect rect = QCompleter::popup()->visualRect(index);
        QPoint toolTipPos(pos.x() + QCompleter::popup()->width(),
                          pos.y() + rect.y() - rect.height());

        QToolTip::showText(toolTipPos, m_model->toolTipText(index));
    }
}
