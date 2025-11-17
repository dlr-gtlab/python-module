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
    // internal helper
    void findElementsByClass(const QDomNode& node,
                             const QStringList& classNames,
                             QList<QDomElement>& results,
                             bool allowNestedClassElements = false)
    {
        QDomNode child = node.firstChild();
        while (!child.isNull())
        {
            if (child.isElement())
            {
                QDomElement elem = child.toElement();
                if (!elem.isNull())
                {
                    QString c = elem.attribute("class");

                    if (classNames.contains(c))
                    {
                        results.append(elem);

                        if (!allowNestedClassElements)
                        {
                            child = child.nextSibling();
                            continue;
                        }
                    }
                }
            }

            findElementsByClass(child, classNames, results, allowNestedClassElements);
            child = child.nextSibling();
        }
    }

    void normalizePropertyContainerId(
        QDomElement container, QString const& formerNameKey,
        QMap<QString, QString>& replaceMap)
    {
        QDomNode child = container.firstChild();

        while (!child.isNull())
        {
            if (child.isElement())
            {
                QDomElement prop = child.toElement();

                if (prop.tagName() == "property")
                {
                    QString oldUUID = prop.attribute("name");

                    // Search for sub element <property name="$$formerNameKey$$">NewName</property>
                    QDomElement nameElem;
                    QDomNode sub = prop.firstChild();

                    while (!sub.isNull())
                    {
                        if (sub.isElement())
                        {
                            QDomElement e = sub.toElement();
                            if (e.attribute("name") == formerNameKey)
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

                        // keep connection info
                        replaceMap.insert(oldUUID, newName);

                        // Replace UUID in name attribute
                        prop.setAttribute("name", newName);

                        // Remove subelement
                        prop.removeChild(nameElem);
                    }
                }
            }

            child = child.nextSibling();
        }
    }

    void replaceUUIDsInTextNodes(QDomNode node,
                                 const QMap<QString, QString>& replaceMap)
    {
        QDomNode child = node.firstChild();

        while (!child.isNull())
        {

            if (child.isText())
            {
                QString txt = child.nodeValue();
                QString newTxt = txt;

                for (auto it = replaceMap.begin(); it != replaceMap.end(); ++it)
                {
                    newTxt.replace(it.key(), it.value());
                }

                if (newTxt != txt)
                    child.setNodeValue(newTxt);
            }

            replaceUUIDsInTextNodes(child, replaceMap);
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

    QList<QDomElement> found;

    // Step 1: Find elements with class=...
    findElementsByClass(root, {"GtpyScriptCalculator", "GtpyTask"}, found);

    // Step 2: Replace UUIDs in input/output_args and build map
    QMap<QString, QString> replaceMap;

    for (QDomElement& elem : found)
    {
        QDomElement c = elem.firstChildElement("property-container");
        while (!c.isNull()) {
            QString name = c.attribute("name");

            if (name == "input_args" ||
                name == "output_args")
            {
                normalizePropertyContainerId(c, "name", replaceMap);
            }

            c = c.nextSiblingElement("property-container");
        }
    }

    // Step 3: Replace all uuids in the document
    replaceUUIDsInTextNodes(root, replaceMap);

    return true;
}
