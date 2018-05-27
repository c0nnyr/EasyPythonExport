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
#define SIMPLE_EXPORT_TEMPLATE(cpp_type, check_py, convert_from_py, convert_to_py, default_val)\
    template <>\
    class Convertor<cpp_type>\
    {\
    public:\
        using T=cpp_type;\
        using CONST_T_REF = const T&;\
        static CONST_T_REF convertFromPyObject(PyObject *obj)\
        {\
            if (!check_py(obj))\
            {\
                assert(false);\
                return CONST_T_REF(default_val);\
            }\
            return CONST_T_REF(convert_from_py(obj));\
        }\
		static PyObject *convertToPyObject(CONST_T_REF t)\
        {\
            return convert_to_py(t);\
        }\
    };
	SIMPLE_EXPORT_TEMPLATE(short, PyInt_CheckExact, PyInt_AsLong, PyInt_FromLong, 0)
	SIMPLE_EXPORT_TEMPLATE(int, PyInt_CheckExact, PyInt_AsLong, PyInt_FromLong, 0)
	SIMPLE_EXPORT_TEMPLATE(long, PyInt_CheckExact, PyInt_AsLong, PyInt_FromLong, 0)
	SIMPLE_EXPORT_TEMPLATE(unsigned short, PyInt_CheckExact, PyInt_AsLong, PyInt_FromLong, 0)
	SIMPLE_EXPORT_TEMPLATE(unsigned int, PyInt_CheckExact, PyInt_AsLong, PyInt_FromLong, 0)
	SIMPLE_EXPORT_TEMPLATE(unsigned long, PyInt_CheckExact, PyInt_AsLong, PyInt_FromLong, 0)
	SIMPLE_EXPORT_TEMPLATE(float, PyFloat_CheckExact, PyFloat_AsDouble, PyFloat_FromDouble, 0)
	SIMPLE_EXPORT_TEMPLATE(double, PyFloat_CheckExact, PyFloat_AsDouble, PyFloat_FromDouble, 0)
	SIMPLE_EXPORT_TEMPLATE(const char*, PyString_CheckExact, PyString_AsString, PyString_FromString, "")

#undef SIMPLE_EXPORT_TEMPLATE
}
#endif /* ARGUMENT_CONVERTOR_HPP */
