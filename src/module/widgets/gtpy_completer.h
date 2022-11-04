/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_completer.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_COMPLETER_H
#define GTPY_COMPLETER_H

#include <QCompleter>
#include <QTextCursor>

#include "gtpy_contextmanager.h"

class GtpyCompleterModel;
/**
 * @brief The GtpyCompleter class
 */
class GtpyCompleter: public QCompleter
{
    Q_OBJECT

public:   
    /**
     * @brief GtpyCompleter
     * @param contextId Id of the context the completer is used for.
     * @param widget Parent widget.
     */
    explicit GtpyCompleter(int contextId, QWidget* widget = nullptr);

    /**
     * @brief Handle completion if completion is selected.
     * @param textCursor Current text cursor.
     * @param cursorRect
     */
    void handleTabCompletion(QTextCursor textCursor, QRect cursorRect);

    /**
     * @brief Returns the popup that shows the completions.
     * @return The popup that shows the completions.
     */
    QAbstractItemView* getPopup() const;

    /**
     * @brief Returns the text of the selected completion.
     * @param index Index of the selected completion.
     * @return The text of the selected completion.
     */
    QString completionText(const QModelIndex& index) const;

    /**
     * @brief Returns the name of the selected completion.
     * @param index Index of the selected completion.
     * @return The name of the selected completion.
     */
    QString completionName(const QModelIndex& index) const;

    /**
     * @brief Returns the cursor offset of the selected completion.
     * @param index Index of the selected completion.
     * @return The cursor offset of the selected completion.
     */
    int completionCursorOffset(const QModelIndex &index) const;

    /**
     * @brief Returns the first index of the model.
     * @return The first index of the model.
     */
    QModelIndex firstIndex() const;

    /**
     * @brief Returns the current index of the model.
     * @return The current index of the model.
     */
    QModelIndex currentIndex() const;

    /**
     * @brief Inserts the completion.
     * @param textCursor Current text cursor.
     */
    void insertCompletion(QTextCursor& textCursor);

private:
    /**
     * @brief Sets the completion prefix.
     * @param prefix Completion Prefix.
     */
    void setCompletionPrefix(const QString& prefix);

    /**
     * @brief Returns the current word to perform the completion on
     * @param pos Position of the textCursor
     * @param line Current text of the line the textCursor is in
     * @param spaces How many spaces should be included in the search
     * @return Word to perform completion on
     */
    QString getTextToComplete(int pos, QString line, int spaces = 0);

    /**
     * @brief Returns all completions found for 'textToComplete'. Stores the
     * completions for the next call to minimize searching in a large map
     * @param textToCompare Word for which completions are searched for
     * @param objToLookup An object to look up completions for in introspection
     * @return Matching completions
     */
    QMultiMap<QString, GtpyFunction> getCompletions(QString textToCompare,
                                                    QString objToLookup);

    /**
     * @brief Compares two Strings and returns the size of the shared part,
     *  searching case insensetive from the beginning. Used for inserting
     *  completions
     * @param s1 String 1
     * @param s2 String 2
     * @return Size of the equal part
     */
    int compareStrings(QString s1, QString s2);

    /// Id of Context
    int m_contextId;

    /// Completer Model
    GtpyCompleterModel* m_model;

    /// starting position of the completion - used for inserting completion
    int m_indexOfCompletion;
    /// size of completion - used for inserting completion
    int m_sizeOfCompletion;


private slots:
    /**
     * @brief Sets the tooltip for the selected item.
     * @param index Index of the selected item.
     */
    void completionToolTip(const QModelIndex& index);
};

#endif // GTPY_COMPLETER_H
