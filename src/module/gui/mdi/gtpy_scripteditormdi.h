/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scripteditormdi.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 28.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPY_SCRIPTEDITORMDI_H
#define GTPY_SCRIPTEDITORMDI_H

#include <gt_mdiitem.h>

#include "gtpy_file.h"
#include "gtpy_scriptrunnable.h"

class QLabel;
class QPushButton;
class GtpyScriptEditor;

class GtpyScriptEditorMdi  : public GtMdiItem
{
    Q_OBJECT

public:
    Q_INVOKABLE GtpyScriptEditorMdi();

    ~GtpyScriptEditorMdi() override;

    QIcon icon() const override;

    void setData(GtObject* obj) override;

private:
    bool m_isEvaluating{false};

    int m_contextId{-1};

    GtpyScriptEditor* m_scriptEdit{nullptr};

    QPushButton* m_evalBtn{nullptr};

    QLabel* m_evalLabel{nullptr};

    QPointer<GtpyScriptRunnable> m_runnable{nullptr};

    QPointer<GtpyFile> m_file{nullptr};

    void evalScript();
};

#endif // GTPY_SCRIPTEDITORMDI_H
