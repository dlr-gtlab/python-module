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
#include "gt_xmlexpr.h"

namespace {


bool handleElement(const QDomNode& node,
                   const QStringList& classNames,
                   QList<QDomElement>& results,
                   bool allowNestedClassElements)
{
    QDomElement elem = node.toElement();
    QString c = elem.attribute(gt::xml::S_CLASS_TAG);

    if (classNames.contains(c))
    {
        results.append(elem);
        return allowNestedClassElements;
    }
    return true;
}


// internal helper
void findElementsByClass(const QDomNode& node,
                         const QStringList& classNames,
                         QList<QDomElement>& results,
                         bool allowNestedClassElements = false)
{
    if (node.isElement())
    {
        if (!handleElement(node, classNames, results, allowNestedClassElements))
        {
            return;
        }
    }

    QDomNode child = node.firstChild();
    while (!child.isNull())
    {
        if (child.isElement())
        {
            if (!handleElement(child, classNames, results, allowNestedClassElements))
            {
                child = child.nextSibling();
                continue;
            }
        }

        findElementsByClass(child, classNames, results, allowNestedClassElements);
        child = child.nextSibling();
    }
}

/**
 * @brief normalizePropertyContainerId
 * Important change of the update is the replacement of the names
 * of the property elements.
 * Therefore a map is used to collect the old and new names of the entries.
 * @param container - QDomElement of the container property
 * @param formerNameKey - identifier if the property in the old structure
 * @param replaceMap - map to collect the required renamings
 */
void normalizePropertyContainerId(
    QDomElement const& container, QString const& formerNameKey,
    QMap<QString, QString>& replaceMap)
{
    QDomNode child = container.firstChild();

    while (!child.isNull())
    {
        if (child.isElement() == false)
        {
            child = child.nextSibling();
            continue;
        }

        QDomElement prop = child.toElement();

        if (prop.tagName() != gt::xml::S_PROPERTY_TAG)
        {
            child = child.nextSibling();
            continue;
        }

        QString oldUUID = prop.attribute(gt::xml::S_NAME_TAG);

        if (oldUUID.isEmpty())
        {
            child = child.nextSibling();
            continue;
        }

        // Search for sub element <property name="$$formerNameKey$$">NewName</property>
        QDomElement nameElem;
        QDomNode sub = prop.firstChild();

        while (!sub.isNull())
        {
            QDomElement e = sub.toElement();
            if (!e.isNull()
                && e.tagName() == gt::xml::S_PROPERTY_TAG
                && e.attribute(gt::xml::S_NAME_TAG) == formerNameKey)
            {
                nameElem = e;
                break;
            }
            sub = sub.nextSibling();
        }

        if (!nameElem.isNull())
        {
            QString newName = nameElem.text().trimmed();

            // keep connection info
            replaceMap.insert(oldUUID, newName);

            // Replace UUID in name attribute
            prop.setAttribute(gt::xml::S_NAME_TAG, newName);

            // Remove subelement
            prop.removeChild(nameElem);
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
        QDomElement c = elem.firstChildElement(gt::xml::S_PROPERTYCONT_TAG);
        while (!c.isNull())
        {
            QString name = c.attribute(gt::xml::S_NAME_TAG);

            if (name == "input_args" || name == "output_args")
            {
                normalizePropertyContainerId(c, gt::xml::S_NAME_TAG, replaceMap);
            }

            c = c.nextSiblingElement(gt::xml::S_PROPERTYCONT_TAG);
        }
    }

    // Step 3: Replace all uuids in the document
    replaceUUIDsInTextNodes(root, replaceMap);

    return true;
}
