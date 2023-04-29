// Wrapper.cpp
#include <jni.h>
#include "helloworld.h"

extern "C" JNIEXPORT void JNICALL
Java_com_example_linklibtest_MyClassWrapper_myFunction(JNIEnv *env, jobject instance) {
   // MyClass *myClass = new MyClass();
   // myClass->myFunction();
	std::cout << "About to invoke zevel" << std::endl;
	__android_log_print(ANDROID_LOG_DEBUG, "MyTag", "zevel");
	Zevel * zevel = new Zevel(8);
	auto moved_zevel = std::move(*zevel);
    delete zevel;
}
