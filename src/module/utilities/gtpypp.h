/* GTlab - Gas Turbine laboratory
 * Source File: gtpypp.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Author: Martin Siggel (martin.siggel@dlr.de)
 */

#ifndef GTPYPP_H_
#define GTPYPP_H_

#include <Python.h>
#include <PythonQtConversion.h>

#include <string>
#include <QString>

#include "gtpy_extendedwrapper.h"

/**
 * Basic C++ Wrapper for the Python C-API
 *
 * It serves as an automatic reference counted RAII-style
 * wrapper to reduce the number of manual Py_INCREF / Py_DECREF
 */
template <typename PyObjectType = PyObject>
class PyPPObjectT
{
public:
    PyPPObjectT() : pythonObj(nullptr) {}

    static PyPPObjectT fromQString(const QString& s)
    {
        return PyPPObjectT::NewRef(PythonQtConv::QStringToPyObject(s));
    }

    static PyPPObjectT fromQObject(QObject* obj)
    {
        return PyPPObjectT::NewRef(
            PythonQt::priv()->wrapQObject(obj)
        );
    }

    static PyPPObjectT fromQVariant(const QVariant& var)
    {
        return PyPPObjectT::NewRef(
            PythonQtConv::QVariantToPyObject(var)
        );
    }

    static PyPPObjectT fromString(const char* s)
    {
        return PyPPObjectT::NewRef(PyUnicode_FromString(s));
    }

    static PyPPObjectT fromLong(long value)
    {
        return PyPPObjectT::NewRef(PyLong_FromLong(value));
    }

    static PyPPObjectT NewRef(PyObjectType* o)
    {
        return PyPPObjectT(o, PyPPObjectT::NEW);
    }

    static PyPPObjectT Borrow(PyObjectType* o)
    {
        return PyPPObjectT(o, PyPPObjectT::BORROWED);
    }

    PyPPObjectT(const PyPPObjectT& other) : pythonObj(other.pythonObj)
    {
        Py_XINCREF(pythonObj);
    }

    PyPPObjectT& operator=(PyPPObjectT&& other)
    {
        Py_XDECREF(pythonObj);
        pythonObj = other.pythonObj;
        other.pythonObj = nullptr;

        return *this;
    }

    PyPPObjectT& operator=(const PyPPObjectT& other)
    {
        PyPPObjectT tmp(other);
        *this = std::move(tmp);
        assert(tmp.pythonObj == nullptr);
        return *this;
    }

    PyPPObjectT(PyPPObjectT&& other)
    {
        pythonObj = other.pythonObj;
        other.pythonObj = nullptr;
    }


    /**
     *  Transfers ownership to the caller.
     *  After release, this object is null.
     *
     *  Used, when a python API function steals ownership
     */
    PyObjectType* release()
    {
        auto retval = pythonObj;

        // setting to nullptr effectively avoids calling DECREF
        pythonObj = nullptr;
        return retval;
    }

    PyObjectType* get() const
    {
        return pythonObj;
    }

    PyObjectType* operator->()
    {
        return get();
    }

    ~PyPPObjectT()
    {
        Py_XDECREF(pythonObj);
    }

    operator bool() const
    {
        return (PyObject*)(get()) != Py_None && get() != nullptr;
    }

private:
    enum RefType
    {
        BORROWED,
        NEW
    };

    explicit PyPPObjectT(PyObjectType* obj, RefType type) : pythonObj(obj)
    {
        if (type == BORROWED)
        {
            Py_XINCREF(pythonObj);
        }
    }

    PyObjectType* pythonObj;
};

using PyPPObject = PyPPObjectT<PyObject>;


/// List functions

inline PyPPObject PyPPList_New(Py_ssize_t len)
{
    return PyPPObject::NewRef(PyList_New(len));
}

inline Py_ssize_t PyPPList_Size(const PyPPObject& list)
{
    return PyList_Size(list.get());
}

inline PyPPObject PyPPList_GetItem(const PyPPObject& list, Py_ssize_t index)
{
    return PyPPObject::Borrow(PyList_GetItem(list.get(), index));
}

// cppcheck-suppress constParameter
inline int PyPPList_Sort(PyPPObject& o)
{
    return PyList_Sort(o.get());
}

// does not steal!
// cppcheck-suppress constParameter
inline int PyPPList_Append(PyPPObject& list, const PyPPObject& item)
{
    return PyList_Append(list.get(), item.get());
}

/// Dict functions

inline PyPPObject PyPPDict_New()
{
    return PyPPObject::NewRef(PyDict_New());
}

inline PyPPObject PyPPDict_Copy(const PyPPObject& o)
{
    return PyPPObject::NewRef(PyDict_Copy(o.get()));
}

// cppcheck-suppress constParameter
inline int PyPPDict_Merge(PyPPObject& a, const PyPPObject& b, int override)
{
    return PyDict_Merge(a.get(), b.get(), override);
}

inline void PyPPDict_Clear(const PyPPObject& dict)
{
    PyDict_Clear(dict.get());
}

inline PyPPObject PyPPDict_GetItem(const PyPPObject& dict, const PyPPObject& str)
{
    return PyPPObject::Borrow(PyDict_GetItem(dict.get(), str.get()));
}

inline PyPPObject PyPPDict_GetItem(const PyPPObject& dict, const char* s)
{
    return PyPPDict_GetItem(dict, PyPPObject::fromString(s));
}

// cppcheck-suppress constParameter
inline int PyPPDict_SetItem(PyPPObject& dict, const char *key, const PyPPObject& val)
{
    return PyDict_SetItemString(dict.get(), key, val.get());
}

// cppcheck-suppress constParameter
inline int PyPPDict_SetItem(PyPPObject& dict, const PyPPObject& key, const PyPPObject& val)
{
    return PyDict_SetItem(dict.get(), key.get(), val.get());
}

inline int PyPPDict_Check(const PyPPObject& p)
{
    return PyDict_Check(p.get());
}

inline PyPPObject PyPPDict_Keys(const PyPPObject& p)
{
    return PyPPObject::NewRef(PyDict_Keys(p.get()));
}

// cppcheck-suppress constParameter
inline int PyPPDict_DelItem(PyPPObject& p, const char *key)
{
    return PyDict_DelItemString(p.get(), key);
}

/// Tuple functions

inline bool PyPPTuple_Check(const PyPPObject& o)
{
    return PyTuple_Check(o.get());
}

inline Py_ssize_t PyPPTuple_Size(const PyPPObject& o)
{
    return PyTuple_Size(o.get());
}

inline PyPPObject PyPPTuple_New(Py_ssize_t size)
{
    return PyPPObject::NewRef(PyTuple_New(size));
}

inline PyPPObject PyPPTuple_GetItem(const PyPPObject& tuple, Py_ssize_t pos)
{
    return PyPPObject::Borrow(PyTuple_GetItem(tuple.get(), pos));
}

// set item steals the object, so we need to move in o
// cppcheck-suppress constParameter
inline int PyPPTuple_SetItem(PyPPObject& p, Py_ssize_t pos, PyPPObject&& o)
{
    return PyTuple_SetItem(p.get(), pos, o.release());
}

/// Object Protocol API

inline PyPPObject PyPPObject_GetAttr(const PyPPObject& o, const char* attrName)
{
    return PyPPObject::NewRef(PyObject_GetAttrString(o.get(), attrName));
}

inline PyPPObject PyPPObject_GetAttr(const PyPPObject& o, const PyPPObject& attrName)
{
    return PyPPObject::NewRef(PyObject_GetAttr(o.get(), attrName.get()));
}

inline PyPPObject PyPPObject_Dir(const PyPPObject& o)
{
    return PyPPObject::NewRef(PyObject_Dir(o.get()));
}

inline int PyPPObject_TypeCheck(const PyPPObject& o, PyTypeObject* type)
{
    return PyObject_TypeCheck(o.get(), type);
}

inline PyPPObject PyPPObject_Repr(const PyPPObject& o)
{
    return PyPPObject::NewRef(PyObject_Repr(o.get()));
}

inline PyPPObject PyPPObject_Str(const PyPPObject& obj)
{
    return PyPPObject::NewRef(PyObject_Str(obj.get()));
}

inline PyPPObject PyPPObject_GenericGetAttr(const PyPPObject& o, const PyPPObject& name)
{
    return PyPPObject::NewRef(PyObject_GenericGetAttr(o.get(), name.get()));
}

inline int PyPPObject_RichCompareBool(const PyPPObject& o1, const PyPPObject& o2, int opid)
{
    return PyObject_RichCompareBool(o1.get(), o2.get(), opid);
}

/// Mapping Protocol

inline int PyPPMapping_Check(const PyPPObject& o)
{
    return PyMapping_Check(o.get());
}

/// Operating System Utilities API
inline PyPPObject PyPPSys_GetObject(const char *name)
{
    return PyPPObject::Borrow(PySys_GetObject(name));
}

/// Integer API
inline int PyPPLong_Check(const PyPPObject& obj)
{
    return PyLong_Check(obj.get());
}

inline long PyPPLong_AsLong(const PyPPObject& obj)
{
    return PyLong_AsLong(obj.get());
}

/// Boolean API

inline int PyPPBool_Check(const PyPPObject& o)
{
    return PyBool_Check(o.get());
}

/// Unicode API
inline int PyPPUnicode_Check(const PyPPObject& obj)
{
    return PyUnicode_Check(obj.get());
}

inline const char* PyPPUnicode_AsUTF8(const PyPPObject& obj)
{
    return PyUnicode_AsUTF8(obj.get());
}

/// Qt API

inline QString PyPPString_AsQString(const PyPPObject& obj)
{
    return QString::fromUtf8(PyPPUnicode_AsUTF8(obj));
}

inline QString PyPPObject_AsQString(const PyPPObject& obj)
{
   auto strObj = PyPPUnicode_Check(obj) ? obj : PyPPObject_Str(obj);
   return PyPPString_AsQString(strObj);
}


inline QVariant PyPPObject_AsQVariant(const PyPPObject& obj)
{
    if (obj && PyPPObject_TypeCheck(obj, &GtpyExtendedWrapperModule::GtpyExtendedWrapper_Type))
    {
        // We need to get the QObject Python Wrapper from this type first
        auto wrapper = (GtpyExtendedWrapperModule::GtpyExtendedWrapper*)obj.get();
        assert(wrapper);
        return QVariant::fromValue(wrapper->getObject());
    }
    else
    {
        return PythonQtConv::PyObjToQVariant(obj.get());
    }
}


/// Importing API
inline PyPPObject PyPPImport_ImportModule(const char *name)
{
    return PyPPObject::NewRef(PyImport_ImportModule(name));
}

inline PyPPObject PyPPImport_GetModuleDict()
{
    return PyPPObject::Borrow(PyImport_GetModuleDict());
}



/// Low Level API

inline PyPPObject PyPPThreadState_GetDict()
{
    return PyPPObject::Borrow(PyThreadState_GetDict());
}

// cppcheck-suppress constParameter
inline int PyPPModule_AddObject(PyPPObject& module, const char *name, PyPPObject&& value)
{
    auto ret = PyModule_AddObject(module.get(), name, value.get());

    if (ret == 0)
    {
        // The module is now owner
        value.release();
    }

    return ret;
}

inline int PyPPModule_AddFunctions(const PyPPObject& module, PyMethodDef* def)
{
    return PyModule_AddFunctions(module.get(), def);
}

inline PyPPObject PyPPModule_Create(PyModuleDef* def)
{
    return PyPPObject::NewRef(PyModule_Create(def));
}

inline bool PyPPModule_Check(const PyPPObject& module)
{
    return PyModule_Check(module.get());
}

inline PyPPObject PyPPModule_GetDict(const PyPPObject& module)
{
    return PyPPObject::Borrow(PyModule_GetDict(module.get()));
}

/// Object calling API
inline int PyPPCallable_Check(const PyPPObject& o)
{
    return PyCallable_Check(o.get());
}


inline PyPPObject PyPPObject_Call(const PyPPObject& callable,
                                  const PyPPObject& args, const PyPPObject& kwargs = {})
{
    PyObject* obj = PyObject_Call(callable.get(), args.get(), kwargs.get());
    return PyPPObject::NewRef(obj);
}

template <typename ...FormatArgs>
inline PyPPObject PyPPObject_CallMethod(const PyPPObject& obj, const char *name,
                                 const char *format, FormatArgs&& ...args)
{
    PyObject* res = PyObject_CallMethod(obj.get(), name, format, std::forward<FormatArgs>(args)...);
    return PyPPObject::NewRef(res);
}

/// Reflection API

inline PyPPObject PyPPEval_GetGlobals()
{
    return PyPPObject::Borrow(PyEval_GetGlobals());
}

#endif // GTPYPP_H_
