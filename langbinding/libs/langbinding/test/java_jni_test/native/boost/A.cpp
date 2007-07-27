#include <jni.h>
#include <cstdio>
#include <cassert>

extern "C" JNIEXPORT void JNICALL foobar(JNIEnv *, jclass)
{
    fputs("Here we are, in foobar().",stderr);
}

namespace
{
    JNINativeMethod jniMethods[] = {
        { "foo", "()V", &foobar }
        };
}

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    fputs("A.cpp:JNI_OnLoad...",stderr);
    
    JNIEnv * env = 0;
    assert( JNI_EVERSION != vm->GetEnv((void**)&env,JNI_VERSION_1_2) );
    
    jclass system_C = env->FindClass("java/lang/System");
    jmethodID systemGetSecurityManager_M
        = env->GetStaticMethodID(system_C,"getSecurityManager","()Ljava/lang/SecurityManager");
    jclass securityManager_C = env->FindClass("java/lang/SecurityManager");
    jmethodID securityManagerCurrentClassLoader_M
        = env->GetMethodID(securityManager_C,"currentClassLoader","()Ljava/lang/ClassLoader");
    
    jobject securityManager = env->CallStaticObjectMethod(system_C,systemGetSecurityManager_M);
    jobject classLoader = env->CallObjectMethod(securityManager,securityManagerCurrentClassLoader_M);
    
    FILE * f = std::fopen("../native/boost/A.class","rb"); assert( f );
    std::fseek(f,0,SEEK_END);
    long bufferLength = std::ftell(f)+1;
    assert( bufferLength > 0 );
    jbyte * buffer = new jbyte[bufferLength];
    std::rewind(f);
    assert( std::fread(buffer,1,bufferLength,f) == bufferLength );
    std::fclose(f);
    jclass a_C = env->DefineClass("boost/A",classLoader,buffer,bufferLength);
    delete[] buffer;
    assert( a_C );
    
    assert( 0 == env->RegisterNatives(a_C,jniMethods,1) );
    
    return JNI_VERSION_1_2;
}
