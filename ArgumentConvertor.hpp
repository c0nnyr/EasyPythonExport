#ifndef ARGUMENT_CONVERTOR_HPP
#define ARGUMENT_CONVERTOR_HPP

#include <string>

namespace easy_python_export_helper
{
    template <typename T>
    class Convertor
    {
    public:
        static T convertFromPyObject(PyObject *obj)
        {
            assert(false);
            return T();
        }
        static PyObject *convertToPyObject(T t)
        {
            assert(false);
            Py_RETURN_NONE;
        }
    };
    
    template <typename T>
    class ConvertorInterface
    {
    public:
        using PureType = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
        static PureType convertFromPyObject(PyObject *obj, int &param_ind)
        {
            assert(PyTuple_GET_SIZE(obj) > param_ind);
            return Convertor<PureType>::convertFromPyObject(PyTuple_GetItem(obj, param_ind++));
        }
        
        static PyObject *convertToPyObject(PureType t)
        {
            return Convertor<PureType>::convertToPyObject(t);
        }
    };
    
    template <>
    class Convertor<int>
    {
    public:
        using T=int;
        using CONST_T_REF = const T&;
        static CONST_T_REF convertFromPyObject(PyObject *obj)
        {
            if (!PyInt_CheckExact(obj))
            {
                assert(false);
                return CONST_T_REF(0);
            }
            return CONST_T_REF(PyInt_AsLong(obj));
        }
        static PyObject *convertToPyObject(CONST_T_REF t)
        {
            return PyInt_FromLong(t);
        }
    };
    template <>
    class Convertor<float>
    {
    public:
        using T=float;
        using CONST_T_REF = const T&;
        static CONST_T_REF convertFromPyObject(PyObject *obj)
        {
            if (!PyFloat_CheckExact(obj))
            {
                assert(false);
                return CONST_T_REF(0);
            }
            return CONST_T_REF(PyFloat_AsDouble(obj));
        }
        static PyObject *convertToPyObject(CONST_T_REF t)
        {
            return PyFloat_FromDouble(t);
        }
    };
    template <>
    class Convertor<double>
    {
    public:
        using T=double;
        using CONST_T_REF = const T&;
        static CONST_T_REF convertFromPyObject(PyObject *obj)
        {
            if (!PyFloat_CheckExact(obj))
            {
                assert(false);
                return CONST_T_REF(0);
            }
            return CONST_T_REF(PyFloat_AsDouble(obj));
        }
        static PyObject *convertToPyObject(CONST_T_REF t)
        {
            return PyFloat_FromDouble(t);
        }
    };
    template <>
    class Convertor<bool>
    {
    public:
        using T=bool;
        using CONST_T_REF = const T&;
        static CONST_T_REF convertFromPyObject(PyObject *obj)
        {
            if (obj != Py_False && obj != Py_True)
            {
                assert(false);
                return CONST_T_REF(false);
            }
            return CONST_T_REF(obj != Py_False);
        }
        static PyObject *convertToPyObject(CONST_T_REF t)
        {
            auto ret = t ? Py_True : Py_False;
			Py_INCREF(ret);
			return ret;
        }
    };
    template <>
    class Convertor<std::string>
    {
    public:
        using T=std::string;
        using CONST_T_REF = const T&;
        static CONST_T_REF convertFromPyObject(PyObject *obj)
        {
            if (!PyString_CheckExact(obj))
            {
                assert(false);
                return CONST_T_REF("");
            }
            return CONST_T_REF(PyString_AsString(obj));
        }
        static PyObject *convertToPyObject(CONST_T_REF t)
        {
            return PyString_FromString(t.c_str());
        }
    };
    template <>
    class Convertor<const char*>
    {
    public:
        using T=const char*;
        using CONST_T_REF = const T&;
        static CONST_T_REF convertFromPyObject(PyObject *obj)
        {
            if (!PyString_CheckExact(obj))
            {
                assert(false);
                return CONST_T_REF("");
            }
            return CONST_T_REF(PyString_AsString(obj));
        }
        static PyObject *convertToPyObject(CONST_T_REF t)
        {
            return PyString_FromString(t);
        }
    };

    
}
#endif /* ARGUMENT_CONVERTOR_HPP */
