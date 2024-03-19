#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include <gt_object.h>
#include <gtpy_contextmanager.h>

class MyObject : public GtObject
{
    Q_OBJECT
public:
    MyObject()
    {
        setObjectName("MyObjectName");
    }
};

struct TestPythonContext
{
    TestPythonContext() : context_id (GtpyContextManager::instance()->createNewContext(GtpyContextManager::ScriptEditorContext))
    {
    }

    ~TestPythonContext()
    {
        GtpyContextManager::instance()->deleteContext(context_id);
    }

    int id() const
    {
        return context_id;
    }

    PyPPObject obj() const
    {
        return PyPPObject::Borrow(GtpyContextManager::instance()->contextPointer(context_id));
    }

    int context_id;
};

#endif // TEST_HELPER_H
