/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_code.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 14.11.2025
 * Author: Jens Schmeink (DLR AT-TWK)
 */

#include "gtpy_moduleupgrader.h"

#include "gt_logging.h"

namespace {

    void findElementsByClass(const QDomNode& node,
                             const QStringList& classNames,
                             QList<QDomElement>& results)
    {
        QDomNode child = node.firstChild();
        while (!child.isNull()) {

            if (child.isElement()) {
                QDomElement elem = child.toElement();

                if (!elem.isNull()) {
                    QString classValue = elem.attribute("class");

                    if (classNames.contains(classValue)) {
                        results.append(elem);
                        // laut deiner Anforderung:
                        // solche Einträge haben KEINE weiteren relevanten Untereinträge,
                        // aber wir durchsuchen trotzdem nicht den Baum *innerhalb* dieses Elements:
                        child = child.nextSibling();
                        continue;
                    }
                }
            }

            // rekursiv in die nächste Ebene
            findElementsByClass(child, classNames, results);

            child = child.nextSibling();
        }
    }

    void normalizePropertyContainer(QDomElement container)
    {
        QDomNode child = container.firstChild();

        while (!child.isNull()) {
            if (child.isElement()) {
                QDomElement prop = child.toElement();

                if (prop.tagName() == "property") {

                    // 1. find sub propety "name"
                    QDomElement nameElem;
                    QDomNode sub = prop.firstChild();

                    while (!sub.isNull())
                    {
                        if (sub.isElement())
                        {
                            QDomElement e = sub.toElement();
                            if (e.attribute("name") == "name")
                            {
                                nameElem = e;
                                break;
                            }
                        }
                        sub = sub.nextSibling();
                    }

                    if (!nameElem.isNull())
                    {
                        QString newName = nameElem.text().trimmed();

                        // 2. name="UUID" → name="OP"
                        prop.setAttribute("name", newName);

                        // 3. Remove sub property <property name="name">…</property>
                        prop.removeChild(nameElem);
                    }
                }
            }

            child = child.nextSibling();
        }
    }

}

bool
gtpy::module_upgrader::to_2_0_0::run(QDomElement& root,
                                     const QString& targetPath)
{
    // only do updates for process elements
    if (!targetPath.endsWith(".gttask")) return true;

    if (root.isNull())
    {
        gtError() << QObject::tr("Invalid .gttask file!");
        return false;
    }

    QList<QDomElement> calcElements;
    findElementsByClass(root, {"GtpyScriptCalculator", "GtpyTask"}, calcElements);

    if (calcElements.isEmpty()) return true;

    gtTrace() << QObject::tr("Update task file") << targetPath;

    for (auto e : calcElements)
    {
        normalizePropertyContainer(e);
    }

    return true;
}
