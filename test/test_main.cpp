//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
// _🪦🐮🐌_

#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>


#if defined(SYS_STRING_USE_PYTHON)

    #define PY_SSIZE_T_CLEAN
    #include <Python.h>

#endif


#if defined(__ANDROID__)

#include <dlfcn.h>
#include <jni.h>

extern "C"
{
    typedef int JNI_CreateJavaVM_t(JavaVM ** p_vm, JNIEnv ** p_env, void * vm_args);
    
    __attribute__((visibility("default"))) void AddSpecialSignalHandlerFn() { }
}

static auto load_jvm() -> std::pair<JavaVM *, JNIEnv *>
{
    JavaVMOption opt[] = {
        { "-Djava.library.path=/data/local/tmp", nullptr },
        { "-verbose:jni", nullptr }
    };

    JavaVMInitArgs args = {
        JNI_VERSION_1_6,
        std::size(opt),
        opt,
        JNI_FALSE
    };

    void * libart = dlopen("libart.so", RTLD_NOW);
    if (!libart) 
    {
        fprintf(stderr, "%s\n", dlerror());
        abort();
    }

    auto JNI_CreateJavaVM = (JNI_CreateJavaVM_t *)dlsym(libart, "JNI_CreateJavaVM");
    if (!JNI_CreateJavaVM)
    {
        fprintf(stderr, "No JNI_CreateJavaVM: %s\n", dlerror());
        abort();
    }

    std::pair<JavaVM *, JNIEnv *> ret;
    if (JNI_CreateJavaVM(&ret.first, &ret.second, &args))
    {
        fprintf(stderr, "Failed to create VM\n");
        abort();
    }
    return ret;
}

JavaVM * g_vm;
JNIEnv * g_env;

#endif

#if defined(SYS_STRING_USE_PYTHON) && defined(PYPY_VERSION)

extern "C" {
    void rpython_startup_code(void);
    int pypy_setup_home(char *home, int verbose);
}

#endif

int main(int argc, char** argv)
{
    #if defined(__ANDROID__)
        auto [vm, env] = load_jvm();
        g_vm = vm;
        g_env = env;
    #endif
    
    #if defined (_WIN32)
        SetConsoleOutputCP(CP_UTF8);
    #endif

    #if defined(SYS_STRING_USE_PYTHON)
        #if !defined(PYPY_VERSION)
            Py_Initialize();
        #else
            rpython_startup_code();
            pypy_setup_home(nullptr, 1);
        #endif
    #endif

    
    
    return doctest::Context(argc, argv).run();
}


