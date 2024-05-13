/**
 * gcc class.c -g $(python3-config --includes --ldflags --libs)  -lpython3.10     -o class
 * https://docs.python.org/3/c-api/arg.html#c.Py_BuildValue Building values
 */

#include <Python.h>
#include <stdio.h>
#include <stdbool.h>

PyObject *pName = NULL;
PyObject *pModule = NULL;
PyObject *pInstance = NULL;
PyObject *pDict = NULL;
PyObject *pClass = NULL;
PyObject *pVal = NULL;
PyObject* sys = NULL;
PyObject* sys_path = NULL;
PyObject* folder_path = NULL;

/*
 * Загрузка интерпритатора python и модуля class.py в него.
 */
PyObject *
python_init() {
    // Инициализировать интерпретатор Python
    Py_Initialize();

    do {
        // Загрузка модуля sys
        sys = PyImport_ImportModule("sys");
        sys_path = PyObject_GetAttrString(sys, "path");
        // Путь до наших исходников Python
        // folder_path = PyUnicode_FromString((const char*) "./src/python");
        folder_path = PyUnicode_FromString((const char*) ".");
        PyList_Append(sys_path, folder_path);

        // Создание Unicode объекта из UTF-8 строки
        pName = PyUnicode_FromString("class1");
        if (!pName) {
            break;
        }

        // Загрузить модуль class
        pModule = PyImport_Import(pName);
        if (!pModule) {
            break;
        }

        // Словарь объектов содержащихся в модуле
        pDict = PyModule_GetDict(pModule);
        if (!pDict) {
            break;
        }

        // Загрузка объекта Class из class.py
        pClass = PyDict_GetItemString(pDict, (const char *) "Class");
        if (!pClass) {
            break;
        }

        // Проверка pClass на годность.
        if (!PyCallable_Check(pClass)) {
            break;
        }

        // Указатель на Class
        pInstance = PyObject_CallObject(pClass, NULL);

        return pInstance;
    } while (0);

    // Печать ошибки
    PyErr_Print();
}

/*
 * Освобождение ресурсов интерпритатора python
 */
void
python_clear() {
    // Вернуть ресурсы системе
    Py_XDECREF(pInstance);
    Py_XDECREF(pDict);

    Py_XDECREF(pModule);
    Py_XDECREF(pName);

    Py_XDECREF(folder_path);
    Py_XDECREF(sys_path);
    Py_XDECREF(sys);
    Py_XDECREF(pClass);

    if (pVal) {
        Py_XDECREF(pVal);
    }


    
    // Выгрузка интерпритатора Python
    Py_Finalize();
}

// PyObject *args = PyTuple_New(0);
// PyObject *kwargs = Py_BuildValue("{s:i}", "b", 5)
// PyObject *function = PyObject_GetAttrString(add_module, "add");
// result = PyObject_Call(function, args, kwargs);




int get_second2( int a1 ) {

   int ret = 0; 
   printf("get_second(%d):\n",a1);

    pVal = PyObject_CallMethod(pInstance, (char *) "add", (char *) "(i)", a1);
    if (pVal != NULL) {
        if (PyLong_Check(pVal)) {
            ret = _PyLong_AsInt(pVal);
        }
        
        Py_XDECREF(pVal);
    } else {
        PyErr_Print();
    }
    // Do something with the result
    return ret;
}

int get_second( int a1, int a2 ) {

   int ret = 0; 
   printf("get_second(%d):\n",a1);

    pVal = PyObject_CallMethod(pInstance, (char *) "add", (char *) "(ii)", a1,a2);
    if (pVal != NULL) {
        if (PyLong_Check(pVal)) {
            ret = _PyLong_AsInt(pVal);
        }
        
        Py_XDECREF(pVal);
    } else {
        PyErr_Print();
    }
    // Do something with the result
    return ret;
}


/**
 * Передача строки в качестве аргумента и получение строки назад
 */
char *
python_class_get_str(char *val) {
    char *ret = NULL;

    pVal = PyObject_CallMethod(pInstance, (char *) "get_value", (char *) "(s)", val);
    if (pVal != NULL) {
        PyObject* pResultRepr = PyObject_Repr(pVal);
        
        // Если полученную строку не скопировать, то после очистки ресурсов python её не будет.
        ret = strdup(PyBytes_AS_STRING(PyUnicode_AsEncodedString(pResultRepr, "utf-8", "ERROR")));
        
        Py_XDECREF(pResultRepr);
        Py_XDECREF(pVal);
    } else {
        PyErr_Print();
    }

    return ret;
}


/*
                      ========== Проверить  ==========

 PyObject *PyObject_Call(PyObject *callable, PyObject *args, PyObject *kwargs)
    Return value: New reference. Part of the Stable ABI.

    Call a callable Python object callable, with arguments given by the tuple args, 
    and named arguments given by the dictionary kwargs.

    args must not be NULL; use an empty tuple if no arguments are needed. 
    If no named arguments are needed, kwargs can be NULL.

    Return the result of the call on success, or raise an exception and return NULL on failure.

*/

void
get_class_vector(){

/*
PyObject *PyObject_VectorcallMethod(
    PyObject *name,  -  The name of the method is given as a Python string name. 
    PyObject *const *args, : The object whose method is called is args[0], 
                             and the args array starting at args[1] represents the arguments of the call.  
                             Должен быть хотя бы один позиционный аргумент.

    size_t nargsf,       s the number of positional arguments including args[0], 
                        plus PY_VECTORCALL_ARGUMENTS_OFFSET if the value of args[0] 
                        may temporarily be changed. 
                        Keyword arguments can be passed just like in PyObject_Vectorcall().   
    PyObject *kwnames)  is a tuple containing the names of the keyword arguments;
                        in other words, the keys of the kwargs dict.
                         These names must be strings (instances of str or a subclass) and they must be unique. 
                         If there are no keyword arguments, then kwnames can instead be NULL.

 
*/
    
    puts("set_params(self,77,b=88,d=99)");
    PyObject *name = Py_BuildValue("s","set_params");
    PyObject *kwnames = Py_BuildValue( "(s,s)", "b","d");
    PyObject *keyfunc = Py_BuildValue( "i",77);
    PyObject *a2 = Py_BuildValue( "i", 88);
    PyObject *a3 = Py_BuildValue( "i", 99);
    PyObject *args[] = { pInstance, keyfunc, a2,a3 };
    
    size_t nargsf = 2 | PY_VECTORCALL_ARGUMENTS_OFFSET;
    PyObject *ret = PyObject_VectorcallMethod(name , args, nargsf , kwnames);
    Py_DECREF(keyfunc);
    Py_DECREF(kwnames);
    Py_DECREF(name);
    Py_DECREF(a2);
    Py_DECREF(a3);

}


void
python_class_set_params1(int a){

    // PyObject *dict = Py_BuildValue("{s:i,s:i}", "a", 42, "b",43);

    PyObject *p1 =  Py_BuildValue("i", a);
    PyObject *name = Py_BuildValue("s","set_params");
    // PyObject* dumpstr = PyString_FromString("dumps");

    pVal = PyObject_CallMethodObjArgs(pInstance, name, p1, NULL);

    if (pVal == NULL) {
        PyErr_Print();
    }

    Py_XDECREF(p1);
    Py_XDECREF(pVal);
    Py_XDECREF(name);
}

void
python_class_set_params(int a, int b){

    // PyObject *dict = Py_BuildValue("{s:i,s:i}", "a", 42, "b",43);

    PyObject *p1 =  Py_BuildValue("i", a);
    PyObject *p2 =  Py_BuildValue("i", b);
    PyObject *name = Py_BuildValue("s","set_params");
    // PyObject* dumpstr = PyString_FromString("dumps");

    pVal = PyObject_CallMethodObjArgs(pInstance, name, p1, p2, NULL);

    if (pVal == NULL) {
        PyErr_Print();
    }

    Py_XDECREF(p1);
    Py_XDECREF(p2);
    Py_XDECREF(pVal);
    Py_XDECREF(name);
}

/**
 * Передача bool в качестве аргумента и получение bool назад
 */
bool
python_class_get_bool(bool val) {
    bool ret = false;

    //printf("val %d\n", val);
    pVal = PyObject_CallMethod(pInstance, (char *) "get_bool", (char *) "(i)", val);
    if (pVal != NULL) {
        if (PyBool_Check(pVal)) {
            ret = PyObject_IsTrue(pVal);
        }
        
        Py_XDECREF(pVal);
    } else {
        PyErr_Print();
    }

    return ret;
}

/**
 * Передача int в качестве аргумента и получение int назад
 */
int 
python_class_get_int(int val) {
    int ret = 0;

    pVal = PyObject_CallMethod(pInstance, (char *) "get_value", (char *) "(i)", val);
    if (pVal != NULL) {
        if (PyLong_Check(pVal)) {
            ret = _PyLong_AsInt(pVal);
        }
        
        Py_XDECREF(pVal);
    } else {
        PyErr_Print();
    }

    return ret;
}

/**
 * Передача double в качестве аргумента и получение double назад
 */
double 
python_class_get_double(double val) {
    double ret = 0.0;

    pVal = PyObject_CallMethod(pInstance, (char *) "get_value", (char *) "(f)", val);
    if (pVal != NULL) {
        if (PyFloat_Check(pVal)) {
            ret = PyFloat_AS_DOUBLE(pVal);
        }
        
        Py_XDECREF(pVal);
    } else {
        PyErr_Print();
    }

    return ret;
}

/**
 * Получение значения переменной содержащей значение типа int
 */
int 
python_class_get_val(char *val) {
    int ret = 0;

    pVal = PyObject_GetAttrString(pInstance, (char *) val);
    if (pVal != NULL) {
        if (PyLong_Check(pVal)) {
            ret = _PyLong_AsInt(pVal);
        }
        
        Py_XDECREF(pVal);
    } else {
        PyErr_Print();
    }

    return ret;
}

int
main() {
    puts("Test class:");
    
    if (!python_init()) {
        puts("python_init error");
        return -1;
    }
    
    puts("Init:");
    printf("\ta: %d\n", python_class_get_val("a"));
    printf("\tb: %d\n", python_class_get_val("b"));
    printf("\tc: %d\n", python_class_get_val("c"));
    printf("\td: %d\n", python_class_get_val("d"));
 

    puts("Strings:");
    char * res = python_class_get_str("Hello from Python!");
    printf("\tString: %s\n", res);

    free(res);
    
    puts("Bools:");
    printf("\tbool: %s\n", python_class_get_bool(false) ? "true" : "false");
    printf("\tbool: %s\n", python_class_get_bool(true) ? "true" : "false");

    puts("Digits:");
    printf("\tint: %d\n", python_class_get_int(32));
    printf("\tdouble: %f\n", python_class_get_double(23.123456789));
    
    puts("Class Attrs:");
    printf("\ta: %d\n", python_class_get_val("a"));
    printf("\tb: %d\n", python_class_get_val("b"));
    printf("\tc: %d\n", python_class_get_val("c"));
 


    printf("два аргумента:\n");
    printf("ADD: %d\n", get_second(2,3));


    printf("one аргумент:\n");
    printf("ADD: %d\n", get_second2(2));

 

    puts("Class Attrs before:");
    printf("\ta: %d\n", python_class_get_val("a"));
    printf("\tb: %d\n", python_class_get_val("b"));

    printf("set аргумент: \n");
 
    python_class_set_params(5,10);
    puts("Class Attrs:");
    printf("\ta: %d\n", python_class_get_val("a"));
    printf("\tb: %d\n", python_class_get_val("b"));


    python_class_set_params1(510);
    puts("Class Attrs:");
    printf("\ta: %d\n", python_class_get_val("a"));
    printf("\tb: %d\n", python_class_get_val("b"));
    printf("\tc: %d\n", python_class_get_val("c"));
    printf("\td: %d\n", python_class_get_val("d"));


    puts("Class vector:");
    get_class_vector();
    printf("\ta: %d\n", python_class_get_val("a"));
    printf("\tb: %d\n", python_class_get_val("b"));
    printf("\tc: %d\n", python_class_get_val("c"));
    printf("\td: %d\n", python_class_get_val("d"));


    python_clear();
    
    return 0;
}
