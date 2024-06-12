/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionitemwidget.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 16.09.2021
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPYCOLLECTIONITEMWIDGET_H
#define GTPYCOLLECTIONITEMWIDGET_H


#include <QDialog>

class QFrame;
class GtCollectionItem;

/**
 * @brief The GtpyCollectionItemWidget class
 */
class GtpyCollectionItemWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief GtpyCollectionItemWidget
     * @param collectionItem Collection item.
     * @param parent Parent widget.
     */
    explicit GtpyCollectionItemWidget(const GtCollectionItem& collectionItem,
                                      QWidget* parent = nullptr);
private:
    /// Filter string.
    QString m_filter;

    /**
     * @brief Returns the filter string.
     * @param str
     * @return The filter string.
     */
    QString filterString(const QString& str);

    /**
     * @brief Creates a new frame and returns it.
     * @return A new frame.
     */
    QFrame* newFrame();

    /**
     * @brief Creates a new box with the given title and the given content.
     * @param title Title of the new box.
     * @param content Content of the new box.
     * @return A new box.
     */
    QFrame* newBox(const QString& title, const QString& content);

    /**
     * @brief Creates a new file box.
     * @param fileNames Name of the files.
     * @return A new file box.
     */
    QFrame* newFileBox(const QStringList& fileNames);

    /**
     * @brief Creates a new file layout.
     * @param filename Name of the file.
     * @return A new file layout.
     */
    QLayout* newFileLayout(const QString& filename);
};

#endif // GTPYCOLLECTIONITEMWIDGET_H
