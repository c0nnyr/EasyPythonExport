#ifndef EASY_PYTHON_EXPORT_HPP
#define EASY_PYTHON_EXPORT_HPP

#include <functional>
#include <vector>
#include <Python.h>
#include "ArgumentConvertor.hpp"

namespace easy_python_export_helper
{
	class PythonExportCollector
	{
	public:
		using Methods = std::vector<PyMethodDef>;
		using Properties = std::vector<PyGetSetDef>;
		PythonExportCollector(Methods &methods, Properties &properties, PyTypeObject &type)
		:_methods(methods)
		,_properties(properties)
		,_type(type)
		{}
		
		void add(const PyMethodDef &method)
		{
			_methods.push_back(method);
		}
		void add(const PyGetSetDef &property)
		{
			_properties.push_back(property);
		}
		
		void flush()
		{
			if (!_methods.empty())
			{
				_methods.push_back(PyMethodDef{nullptr, nullptr, 0, nullptr});
				_type.tp_methods = _methods.data();
			}
			if (!_properties.empty())
			{
				_properties.push_back(PyGetSetDef{nullptr, nullptr, nullptr, nullptr});
				_type.tp_getset = _properties.data();
			}
		}
	private:
		Methods &_methods;
		Properties &_properties;;
		PyTypeObject &_type;
	};

	template <typename Cls, typename ReturnType, typename ...Arguments>
	class FunctionWrapper
	{
	public:
		using MethodType = ReturnType (Cls::*)(Arguments...);
		FunctionWrapper(MethodType method):_method(method)
		{}
		
		PyObject* runMethod(Cls *ptr, PyObject *argument)
		{
            if (!PyTuple_CheckExact(argument))
            {
                assert(false);
                Py_RETURN_NONE;
            }
            int param_ind = 0;
            return easy_python_export_helper::ConvertorInterface<ReturnType>::convertToPyObject(
                (ptr->*_method)(easy_python_export_helper::ConvertorInterface<Arguments>::convertFromPyObject(argument, param_ind)...));
		}
		PyObject* runGetProperty(Cls *ptr)
		{
			return easy_python_export_helper::ConvertorInterface<ReturnType>::convertToPyObject((ptr->*_method)());
		}
	private:
		MethodType _method;
	};
    template <typename Cls, typename ...Arguments>
    class FunctionWrapper<Cls, void, Arguments...>
    {
    public:
        using MethodType = void (Cls::*)(Arguments...);
        FunctionWrapper(MethodType method):_method(method)
        {}
        
        PyObject* runMethod(Cls *ptr, PyObject *argument)
        {
            if (!PyTuple_CheckExact(argument))
            {
                assert(false);
                Py_RETURN_NONE;
            }
            int param_ind = 0;
            (ptr->*_method)(easy_python_export_helper::ConvertorInterface<Arguments>::convertFromPyObject(argument, param_ind)...);
            Py_RETURN_NONE;
        }
        int runSetProperty(Cls *ptr, PyObject *argument)
        {
			assert(sizeof...(Arguments) == 1);
            (ptr->*_method)(easy_python_export_helper::ConvertorInterface<Arguments>::convertFromPyObject(argument)...);
            return 0;
        }
    private:
        MethodType _method;
    };
	// Normal functions. use DummyClass to make the interface consistent
	class DummyClass{};
	template <typename ReturnType, typename ...Arguments>
	class FunctionWrapper<DummyClass, ReturnType, Arguments...>
	{
	public:
		using MethodType = ReturnType (*)(Arguments...);
		FunctionWrapper(MethodType method):_method(method)
		{}
		
		PyObject* runMethod(void *ptr, PyObject *argument)//ptr is of no use
		{
            if (!PyTuple_CheckExact(argument))
            {
                assert(false);
                Py_RETURN_NONE;
            }
            int param_ind = 0;
            return easy_python_export_helper::ConvertorInterface<ReturnType>::convertToPyObject(
                _method(easy_python_export_helper::ConvertorInterface<Arguments>::convertFromPyObject(argument, param_ind)...));
		}
		PyObject* runGetProperty(void *ptr)
		{
			return easy_python_export_helper::ConvertorInterface<ReturnType>::convertToPyObject(_method());
		}
	private:
		MethodType _method;
	};
    template <typename ...Arguments>
    class FunctionWrapper<DummyClass, void, Arguments...>
    {
    public:
        using MethodType = void (*)(Arguments...);
        FunctionWrapper(MethodType method):_method(method)
        {}
        
        PyObject* runMethod(void *ptr, PyObject *argument)//ptr is of no use
        {
            if (!PyTuple_CheckExact(argument))
            {
                assert(false);
                Py_RETURN_NONE;
            }
            int param_ind = 0;
            _method(easy_python_export_helper::ConvertorInterface<Arguments>::convertFromPyObject(argument, param_ind)...);
            Py_RETURN_NONE;
        }
        int runSetProperty(void *ptr, PyObject *argument)
        {
			assert(sizeof...(Arguments) == 1);
            _method(easy_python_export_helper::ConvertorInterface<Arguments>::convertFromPyObject(argument)...);
            return 0;
        }
    private:
        MethodType _method;
    };
    
    template <typename Cls, typename ReturnType, typename ...Arguments>
    FunctionWrapper<Cls, ReturnType, Arguments...> createFunctionWrapper(ReturnType (Cls::*method)(Arguments...))
    {
        return FunctionWrapper<Cls, ReturnType, Arguments...>(method);
    }
    template <typename ReturnType, typename ...Arguments>
    FunctionWrapper<DummyClass, ReturnType, Arguments...> createFunctionWrapper(ReturnType (*method)(Arguments...))
    {
        return FunctionWrapper<DummyClass, ReturnType, Arguments...>(method);
    }
}
#define _PYTHON_EXPORT_HELPER_CONCATENATE(x, y) _PYTHON_EXPORT_HELPER_CONCATENATE2(x, y)
#define _PYTHON_EXPORT_HELPER_CONCATENATE2(x, y) x##y
#define _PYTHON_EXPORT_HELPER_STRING(x) _PYTHON_EXPORT_HELPER_STRING2(x)
#define _PYTHON_EXPORT_HELPER_STRING2(x) #x
#define _PYTHON_EXPORT_HELPER_RANDOME_VARIABLE_NAME(prefix) _PYTHON_EXPORT_HELPER_CONCATENATE(prefix, __LINE__)
//cls may be A::B like, does not support
#define _PYTHON_EXPORT_HELPER_PYTHON_CLASS_NAME(module, cls) \
	_PYTHON_EXPORT_HELPER_CONCATENATE(autogenerate_python_class_,\
	_PYTHON_EXPORT_HELPER_CONCATENATE(module, _PYTHON_EXPORT_HELPER_CONCATENATE(_, cls)))
#define _PYTHON_EXPORT_HELPER_INIT_NAME(module, cls) \
	_PYTHON_EXPORT_HELPER_CONCATENATE(autogenerate_init_, \
	_PYTHON_EXPORT_HELPER_CONCATENATE(module, _PYTHON_EXPORT_HELPER_CONCATENATE(_, cls)))
#define _PYTHON_EXPORT_HELPER_PYTHON_TYPE_NAME(module, cls) \
	_PYTHON_EXPORT_HELPER_CONCATENATE(autogenerate_python_type_, \
	_PYTHON_EXPORT_HELPER_CONCATENATE(module, _PYTHON_EXPORT_HELPER_CONCATENATE(_, cls)))
#define _PYTHON_EXPORT_HELPER_MODULE_CLASS_NAME(module, cls) module.cls
#define _PYTHON_EXPORT_HELPER_METHOD_WRAPPER(func)\
[](PyObject *self, PyObject *argument)\
{\
    return easy_python_export_helper::createFunctionWrapper((&origin_class::func)).runMethod(&((python_class*)self)->inst, argument);\
}
#define _PYTHON_EXPORT_HELPER_PROPERTY_GET_METHOD_WRAPPER(func)\
[](PyObject *self, void *)\
{\
    return easy_python_export_helper::createFunctionWrapper((&origin_class::func)).runGetProperty(&((python_class*)self)->inst);\
}
#define _PYTHON_EXPORT_HELPER_PROPERTY_SET_METHOD_WRAPPER(func)\
[](PyObject *self, PyObject *argument, void *)\
{\
    return easy_python_export_helper::createFunctionWrapper((&origin_class::func)).runSetProperty(&((python_class*)self)->inst, argument);\
}

#define EXPORT_PYTHON(module, cls)\
	extern void _PYTHON_EXPORT_HELPER_INIT_NAME(module, cls)();\
	_PYTHON_EXPORT_HELPER_INIT_NAME(module, cls)();

#define ADD_METHOD(func) \
	collector.add(\
	PyMethodDef{#func,\
    _PYTHON_EXPORT_HELPER_METHOD_WRAPPER(func),\
	METH_VARARGS,\
	""});
#define ADD_METHOD_STATIC(func) \
	collector.add(\
	PyMethodDef{#func,\
    _PYTHON_EXPORT_HELPER_METHOD_WRAPPER(func),\
	METH_STATIC|METH_VARARGS,\
	""});
#define ADD_PROPERTY_RO(name) \
	collector.add(\
	PyGetSetDef{(char*)#name,\
	_PYTHON_EXPORT_HELPER_PROPERTY_GET_METHOD_WRAPPER(_PYTHON_EXPORT_HELPER_CONCATENATE(Get, name)),\
    nullptr,\
	(char*)""});
#define ADD_PROPERTY_RW(name) \
	collector.add(\
	PyGetSetDef{(char*)#name,\
	_PYTHON_EXPORT_HELPER_PROPERTY_GET_METHOD_WRAPPER(_PYTHON_EXPORT_HELPER_CONCATENATE(Get, name)),\
	_PYTHON_EXPORT_HELPER_PROPERTY_SET_METHOD_WRAPPER(_PYTHON_EXPORT_HELPER_CONCATENATE(Set, name)),\
	(char*)""});


#define BEGIN_DEFINE_EXPORT_PYTHON_CLASS(module, cls)\
	namespace easy_python_export\
	{\
		struct _PYTHON_EXPORT_HELPER_PYTHON_CLASS_NAME(module, cls) : PyObject\
		{\
			cls inst;\
		};\
	}\
	extern void _PYTHON_EXPORT_HELPER_INIT_NAME(module, cls)()\
	{\
		using python_class = easy_python_export::_PYTHON_EXPORT_HELPER_PYTHON_CLASS_NAME(module, cls);\
		using origin_class = cls;\
		static PyTypeObject _PYTHON_EXPORT_HELPER_PYTHON_TYPE_NAME(module, cls) = {\
			PyObject_HEAD_INIT(NULL)\
			0,                               \
			_PYTHON_EXPORT_HELPER_STRING(_PYTHON_EXPORT_HELPER_MODULE_CLASS_NAME(module, cls)),\
			sizeof(python_class),            \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE ,    \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   0,                                \
		   PyType_GenericNew,                \
		};\
		auto &type_class = _PYTHON_EXPORT_HELPER_PYTHON_TYPE_NAME(module, cls);\
		static easy_python_export_helper::PythonExportCollector::Methods methods;\
		static easy_python_export_helper::PythonExportCollector::Properties properties;\
		easy_python_export_helper::PythonExportCollector collector(\
			methods,\
			properties,\
			type_class\
		);\
		auto module_str = _PYTHON_EXPORT_HELPER_STRING(module);\
		auto cls_str = _PYTHON_EXPORT_HELPER_STRING(cls);

#define END_DEFINE_EXPORT_PYTHON_CLASS()\
		collector.flush();\
		PyObject *module_inst = Py_InitModule3(module_str, nullptr, nullptr);\
		assert(module_inst);\
		if (PyType_Ready(&type_class) < 0)\
			assert(false);\
		Py_INCREF(&type_class);\
		PyModule_AddObject(module_inst, cls_str,\
			(PyObject *)&type_class);\
	}

#endif//EASY_PYTHON_EXPORT_HPP
