/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_examplewrapper.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 18.02.2020
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "PythonQt.h"
#include "PythonQtConversion.h"

#include "gt_object.h"
#include "gt_abstractproperty.h"
#include "gt_calculatorhelperfactory.h"

#include "gtpy_createhelperfunction.h"
#include "gtpy_propertysetter.h"
#include "gtpy_decorator.h"

#include "gtpy_extendedwrapper.h"
#include "gtpypp.h"

#include <QRegularExpression>

using namespace GtpyExtendedWrapperModule;

static QString
pyValidGtPropertyId(QString id)
{
    return id.replace(QRegularExpression("[^A-Za-z0-9]+"), "");
}

static QString
pointerAdress(QObject* obj)
{
    QString retVal = obj->metaObject()->className();
    retVal += " ";
    retVal += QString("0x%1").arg((quintptr)obj,
                                  QT_POINTER_SIZE * 2, 16, QChar('0'));
    return retVal;
}

static ternaryfunc pythonqt_slot_call = nullptr;

PyObjectAPIReturn
PythonQtSlotFunction_MyCall(PyObject* func, PyObject* argsPy, PyObject* kw)
{
    auto args = PyPPObject::Borrow(argsPy);
    if (PyPPTuple_Check(args))
    {
        int size = PyPPTuple_Size(args);

        for (int i = 0; i < size; i++)
        {
            auto arg = PyPPTuple_GetItem(args, i);

            if (!arg || !PyPPObject_TypeCheck(arg, &GtpyExtendedWrapper_Type)) continue;

            GtpyExtendedWrapper* wrapper = (GtpyExtendedWrapper*)arg.get();
            auto wrappedObj = PyPPObject::Borrow((PyObject*)wrapper->_obj);
            PyPPTuple_SetItem(args, i, std::move(wrappedObj));

        }
    }

    return pythonqt_slot_call(func, args.get(), kw);
}

void
GtpyCustomization::customizeSlotCalling()
{
    pythonqt_slot_call = PythonQtSlotFunction_Type.tp_call;
    PythonQtSlotFunction_Type.tp_call = PythonQtSlotFunction_MyCall;
}

static void
GtpyExtendedWrapper_dealloc(GtpyExtendedWrapper* self)
{
    if (self->_obj)
    {
        if (self->forcePythonOwnership && self->_obj->_obj)
        {
            self->_obj->_obj->deleteLater();
        }

        Py_DECREF(self->_obj);
        self->_obj = nullptr;
    }

    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObjectAPIReturn
GtpyExtendedWrapper_new(PyTypeObject* type, PyObject* argsIn,
                        PyObject* /*kwds*/)
{


    if (!argsIn)
    {
        QString error =  "__init__(self, GtObject) missing 1 "
                         "required positional argument: "
                         "'" + QString(GtObject::staticMetaObject.className()) +
                         "'";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return nullptr;
    }

    GtpyExtendedWrapper* self = (GtpyExtendedWrapper*)type->tp_alloc(type, 0);
    self->forcePythonOwnership = false;

    auto args = PyPPObject::Borrow(argsIn);
    int argsCount = PyPPTuple_Size(args);

    if (argsCount < 1)
    {
        QString error =  "__init__(self, GtObject) missing 1 "
                         "required positional argument: "
                         "'" + QString(GtObject::staticMetaObject.className()) +
                         "'";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return nullptr;
    }
    else if (argsCount > 1)
    {
        QString error = "__init__(slef, " +
                        QString(GtObject::staticMetaObject.className()) +
                        ") takes 2 positional arguments but ";
        error += QString::number(argsCount);
        error += " were given";

        PyErr_SetString(PyExc_TypeError, error.toStdString().c_str());

        return nullptr;
    }

    auto obj = PyPPTuple_GetItem(args, 0);

    if (obj && (obj->ob_type->tp_base == &PythonQtInstanceWrapper_Type ||
                obj->ob_type == &PythonQtInstanceWrapper_Type))
    {
        // transfer owner to pyQtwrapper
        auto pyQtwrapper = PyPPObjectT<PythonQtInstanceWrapper>::NewRef(
            static_cast<PythonQtInstanceWrapper*>((void*)obj.release()));


        assert(pyQtwrapper);

        if (pyQtwrapper->_obj)
        {
            self->_obj = pyQtwrapper.release();
        }
    }

    return (PyObjectAPIReturn)self;
}

int
GtpyExtendedWrapper_init(GtpyExtendedWrapper* /*self*/, PyObject* /*args*/,
                         PyObject* /*kwds*/)
{
    return 0;
}

static PyObjectAPIReturn
GtpyExtendedWrapper_richcompare(GtpyExtendedWrapper* self,
                                PyObject* other, int code)
{
    return PythonQtInstanceWrapper_Type.tp_richcompare((PyObject*)self->_obj,
            other, code);
}

static PyObjectAPIReturn
object___dir__(PyObject* self/*, PyObject* Py_UNUSED(ignored)*/)
{
    GtpyExtendedWrapper* wrapper = (GtpyExtendedWrapper*)self;

    PyPPObject result;

    auto dict = PyPPObject_GetAttr(PyPPObject::Borrow(self), "__dict__");

    if (dict && PyPPDict_Check(dict))
    {
        result = PyPPDict_Keys(dict);
    }

    if (!result)
    {
        result = PyPPList_New(0);
    }

    auto wrappedKeys = PyPPObject_Dir(PyPPObject::Borrow((PyObject*)wrapper->_obj));

    if (wrappedKeys)
    {
        int count = PyPPList_Size(wrappedKeys);


        for (int i = 0; i < count; i++)
        {
            auto key = PyPPList_GetItem(wrappedKeys, i);
            PyPPList_Append(result, key);
        }
    }

    ///look for create helper function
    QStringList helperList = gtCalculatorHelperFactory->connectedHelper(
                                 Py_TYPE(wrapper->_obj)->tp_name);

    foreach (QString helperName, helperList)
    {
        QString temp = "create" + helperName;

        PyPPList_Append(result, PyPPObject::fromQString(temp));
    }

    PyPPList_Sort(result);

    return result.release();
}

static PyMethodDef
GtpyExtendedWrapper_methods[] =
{
    {
        "__dir__", (PyCFunction)object___dir__, METH_NOARGS,
        "GtpyExtendedWrapper dir() implementation"
    },
    {nullptr, nullptr, 0, nullptr}  /* Sentinel */
};

static int
GtpyExtendedWrapper_setattro(PyObject* obj, PyObject* name, PyObject* value)
{
    QString strName = QString(PyUnicode_AsUTF8(name));

    if (strName.isEmpty())
    {
        return -1;
    }

    QString error;
    GtpyExtendedWrapper* wrapper = (GtpyExtendedWrapper*)obj;

    QObject* qObj = nullptr;
    GtObject* gtObj = nullptr;

    if (!wrapper->_obj)
    {
        error = "Invalid " + QString(GtObject::staticMetaObject.className()) +
                " instance";
        PyErr_SetString(PyExc_AttributeError, error.toLatin1().data());

        return -1;
    }

    if (wrapper->_obj->_obj)
    {
        qObj = qobject_cast<QObject*>(wrapper->_obj->_obj.data());
    }

    if (!qObj)
    {
        error = "Object is no " + QString(QObject::staticMetaObject.className())
                + " instance";

        PyErr_SetString(PyExc_AttributeError, error.toLatin1().data());

        return -1;
    }

    gtObj = qobject_cast<GtObject*>(qObj);

    if (gtObj)
    {
        ///Set GtProperty
        QList<GtAbstractProperty*> propList = gtObj->fullPropertyList();

        foreach (GtAbstractProperty* prop, propList)
        {
            QString pyName = pyValidGtPropertyId(prop->ident());

            if (pyName == strName)
            {
                if (PyObject_TypeCheck(value, &GtpyExtendedWrapper_Type))
                {
                    value = (PyObject*)reinterpret_cast<GtpyExtendedWrapper*>(value)->_obj;
                }

                GtpyDecorator decorator;
                decorator.setPropertyValue(
                    gtObj, prop->ident(),
                    PythonQtConv::PyObjToQVariant(value));

                return 0;
            }
        }
    }

    ///Set child error
    QObjectList children = wrapper->_obj->_obj->children();

    for (int i = 0; i < children.count(); i++)
    {
        QObject* child = children.at(i);

        if (child->objectName() == strName)
        {
            error = "It is not allowed to overwrite the child element " +
                    child->objectName() + " (" + pointerAdress(child) + ")";

            PyErr_SetString(PyExc_ValueError, error.toLatin1().data());
            return -1;
        }
    }

    return PythonQtInstanceWrapper_Type.tp_setattro((PyObject*)wrapper->_obj,
            name, value);
}


static PyObject*
GtpyExtendedWrapper_getattro(PyObject* obj, PyObject* name)
{
    GtpyExtendedWrapper* wrapper = (GtpyExtendedWrapper*)obj;

    // Check if the wrapped objects are valid.
    if (!wrapper->_obj || !wrapper->_obj->_obj)
    {
        PyErr_SetString(PyExc_AttributeError, "invalid instance");
        return nullptr;
    }

    // Check if the given name is a string object
    if (!PyUnicode_Check(name))
    {
        PyErr_SetString(PyExc_AttributeError, "invalid attribute name");
        return nullptr;
    }

    QString strName{PyUnicode_AsUTF8(name)};

    if(strName.isEmpty())
    {
        return nullptr;
    }

    // Cast wrapped object to QObject
    QObject* qObj = qobject_cast<QObject*>(wrapper->_obj->_obj.data());

    if (!qObj)
    {
        PyErr_SetString(PyExc_AttributeError, "Wrapped object is invalid");
        return nullptr;
    }

    // Get attribute object of GtpyExtendedWrapper
    PyErr_Clear();
    auto attr = PyPPObject_GenericGetAttr(PyPPObject::Borrow(obj), PyPPObject::Borrow(name));
    PyErr_Clear();

    // If __dict__ is called fill dict with GtProperties
    if (strName == "__dict__")
    {
        PyPPObject dict;

        // Create a dict object
        if (!attr || !PyPPDict_Check(attr))
        {
            dict = PyPPDict_New();
        }
        else
        {
            dict = PyPPDict_Copy(attr);
        }

        // Get the __dict__ of the wrapped object and merge it with the
        // created dict
        auto pyQtWrapperDict = PyPPObject_GetAttr(
            PyPPObject::Borrow((PyObject*)wrapper->_obj), PyPPObject::Borrow(name));
        PyErr_Clear();

        if (pyQtWrapperDict)
        {
            PyPPDict_Merge(dict, pyQtWrapperDict, false);
        }

        // Add the GtProperty instances to the dict
        if (GtObject* gtObj = qobject_cast<GtObject*>(qObj))
        {
            auto propList = gtObj->fullPropertyList();
            for (auto* prop : qAsConst(propList))
            {
                QString propId{pyValidGtPropertyId(prop->ident())};

                if (auto o = PyPPObject_GetAttr(PyPPObject::Borrow(obj), propId.toLatin1().data()))
                {
                    PyPPDict_SetItem(dict, propId.toLatin1().data(), o);
                }
            }
        }

        return dict.release();
    }

    // Return the attribute of GtpyExtendedWrapper if it is valid
    if (attr)
    {
        return attr.release();
    }

    // If the attribute is a child object, it will be wrapped and returned
    auto children = wrapper->_obj->_obj->children();
    for (auto* child : qAsConst(children))
    {
        if (child->objectName() == strName)
        {
            auto pyQtWrapper = PyPPObject::NewRef(PythonQt::priv()->wrapQObject(child));

            if (pyQtWrapper)
            {
                auto childArg = PyPPTuple_New(1);
                PyPPTuple_SetItem(childArg, 0, std::move(pyQtWrapper));

                // Create a new GtpyExtendedWrapper object
                return PyObject_CallObject((PyObject*) &GtpyExtendedWrapper_Type, childArg.get());
            }
        }
    }

    // Get attribute object of PythonQtInstanceWrapper
    PyObject* pyQtWrapperAttr = PyObject_GetAttr(
                (PyObject*)wrapper->_obj, name);
    PyErr_Clear();

    if (pyQtWrapperAttr)
    {
        return pyQtWrapperAttr;
    }

    // Get GtProperties values or setter methods
    if (GtObject* gtObj = qobject_cast<GtObject*>(qObj))
    {
        auto propList = gtObj->fullPropertyList();
        for (auto* prop : qAsConst(propList))
        {
            QString propId{pyValidGtPropertyId(prop->ident())};

            if (!propId.isEmpty())
            {
                if (propId == strName)
                {
                    return PyPPObject::fromQVariant(prop->valueToVariant()).release();
                }

                if (strName.startsWith("set"))
                {
                    // Name of setter methods without string validation,
                    // in order to keep old scripts running
                    QString oldSetterName{prop->ident()};
                    oldSetterName.replace(0, 1, oldSetterName.at(0).toUpper());
                    oldSetterName.prepend("set");

                    propId.replace(0, 1, oldSetterName.at(0).toUpper());
                    propId.prepend("set");

                    if (propId == strName || oldSetterName == strName)
                    {
                        return GtpyPropertySetter_New(prop->ident(), obj,
                                                      nullptr);
                    }
                }
            }
        }
    }

    // Get create helper methods
    QStringList helperList = gtCalculatorHelperFactory->connectedHelper(
                                 Py_TYPE(wrapper->_obj)->tp_name);

    for (const auto& helperName : qAsConst(helperList))
    {
        QString createHelperName{"create" + helperName};

        if (strName == createHelperName)
        {
            auto childArg = PyPPTuple_New(2);

            PyPPTuple_SetItem(childArg, 0, PyPPObject::fromQString(helperName));
            PyPPTuple_SetItem(childArg, 1, PyPPObject::Borrow(obj));

//          return GtpyCreateHelperFunction_Type.tp_new(
//                                   &GtpyCreateHelperFunction_Type, childArg,
//                                   nullptr);
            return PyObject_CallObject((PyObject*) &GtpyCreateHelperFunction_Type, childArg.get());
        }
    }

    // Set error string because no attribute with the given name was found
    QString error{"%1 (%2) has no attribute named '%3'"};
    error = error.arg(qObj->objectName(), pointerAdress(qObj), strName);
    PyErr_SetString(PyExc_AttributeError, error.toLatin1().data());
    return nullptr;
}

static PyObjectAPIReturn
GtpyExtendedWrapper_repr(PyObject* self)
{
    GtpyExtendedWrapper* wrapper = (GtpyExtendedWrapper*)self;
    return PythonQtInstanceWrapper_Type.tp_repr((PyObject*)wrapper->_obj);
}

static long
GtpyExtendedWrapper_hash(GtpyExtendedWrapper* self)
{
    return PythonQtInstanceWrapper_Type.tp_hash((PyObject*)self->_obj);
}

static PyObjectAPIReturn
GtpyExtendedWrapper_str(PyObject* self)
{
    GtpyExtendedWrapper* wrapper = (GtpyExtendedWrapper*)self;
    return PythonQtInstanceWrapper_Type.tp_str((PyObject*)wrapper->_obj);
}

static int
GtpyExtendedWrapper_builtin_nonzero(PyObject* self)
{
    GtpyExtendedWrapper* wrapper = (GtpyExtendedWrapper*)self;
    return (wrapper->_obj->_wrappedPtr ==
            nullptr && wrapper->_obj->_obj == nullptr) ? 0 : 1;
}

static PyNumberMethods
GtpyExtendedWrapper_as_number =
{
    0,      /* nb_add */
    0,      /* nb_subtract */
    0,      /* nb_multiply */
    0,      /* nb_remainder */
    0,      /* nb_divmod */
    0,      /* nb_power */
    0,      /* nb_negative */
    0,      /* nb_positive */
    0,      /* nb_absolute */
    GtpyExtendedWrapper_builtin_nonzero,      /* nb_nonzero / nb_bool in Py3K */
    0,      /* nb_invert */
    0,      /* nb_lshift */
    0,      /* nb_rshift */
    0,    /* nb_and */
    0,    /* nb_xor */
    0,    /* nb_or */
    0,      /* nb_int */
    0,      /* nb_long  / nb_reserved in Py3K */
    0,      /* nb_float */
    0,      /* nb_inplace_add */
    0,      /* nb_inplace_subtract */
    0,      /* nb_inplace_multiply */
    0,      /* nb_inplace_remainder */
    0,      /* nb_inplace_power */
    0,      /* nb_inplace_lshift */
    0,      /* nb_inplace_rshift */
    0,      /* nb_inplace_and */
    0,      /* nb_inplace_xor */
    0,      /* nb_inplace_or */
    0,      /* nb_floor_divide */
    0,      /* nb_true_divide */
    0,      /* nb_inplace_floor_divide */
    0,      /* nb_inplace_true_divide */
    0,      /* nb_index in Py3K */
};

PyTypeObject
GtpyExtendedWrapperModule::GtpyExtendedWrapper_Type =
{
    PyVarObject_HEAD_INIT(NULL, 0)
    "GtObjectWrapperModuleC.GtpyExtendedWrapper", /*tp_name*/
    sizeof(GtpyExtendedWrapper),                  /*tp_basicsize*/
    0,                                            /*tp_itemsize*/
    (destructor)GtpyExtendedWrapper_dealloc,      /*tp_dealloc*/
    0, /*tp_print*/
    0, /*tp_getattr*/
    0, /*tp_setattr*/
    0, /*tp_compare*/
    GtpyExtendedWrapper_repr,       /*tp_repr*/
    &GtpyExtendedWrapper_as_number, /*tp_as_number*/
    0, /*tp_as_sequence*/
    0, /*tp_as_mapping*/
    (hashfunc)GtpyExtendedWrapper_hash, /*tp_hash */
    0, /*tp_call*/
    GtpyExtendedWrapper_str,      /*tp_str*/
    GtpyExtendedWrapper_getattro, /*tp_getattro*/
    GtpyExtendedWrapper_setattro, /*tp_setattro*/
    0,                            /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE
    , /*tp_flags*/
    "GtpyExtendedWrapper object", /* tp_doc */
    0, /* tp_traverse */
    0, /* tp_clear */
    (richcmpfunc)GtpyExtendedWrapper_richcompare, /* tp_richcompare */
    0, /* tp_weaklistoffset */
    0, /* tp_iter */
    0, /* tp_iternext */
    GtpyExtendedWrapper_methods, /* tp_methods */
    0, /* tp_members */
    0, /* tp_getset */
    0, /* tp_base */
    0, /* tp_dict */
    0, /* tp_descr_get */
    0, /* tp_descr_set */
    0, /* tp_dictoffset */
    (initproc)GtpyExtendedWrapper_init, /* tp_init */
    0, /* tp_alloc */
    GtpyExtendedWrapper_new, /* tp_new */

};

QObject* GtpyExtendedWrapper::getObject() const
{
    if (!_obj) return nullptr;
    return _obj->_obj;
}
