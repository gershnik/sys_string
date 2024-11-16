//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
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
        Py_Initialize();
    #endif

    return doctest::Context(argc, argv).run();
}


