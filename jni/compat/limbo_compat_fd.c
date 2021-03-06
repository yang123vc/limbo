#include <jni.h>
#include <stdio.h>
#include "limbo_logutils.h"
#include <dlfcn.h>
#include <fcntl.h>
#include <errno.h>
#include <malloc.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <unwind.h>
#include <dlfcn.h>
#include "limbo_compat_fd.h"
#include "limbo_compat.h"


//
int close_fd(int fd) {

	JNIEnv *env;
	jmethodID methodID;
	jint jfd, jres;
	int res = -1;

	if (jvm == NULL) {
		LOGE("Jvm not initialized");
		return -1;
	}
	jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
	if (rs != JNI_OK) {
		LOGE("Could not get env");
		return -1;
	}

	jclass cls = (*env)->GetObjectClass(env, jobj);

	methodID = (*env)->GetMethodID(env, cls, "close_fd", "(I)I");

	jres = (jint)(*env)->CallIntMethod(env, jobj, methodID, fd);
	res = (int) res;

//	(*env)->DeleteLocalRef(env, jfd);
//	(*env)->DeleteLocalRef(env, cls);
//	(*env)->DeleteLocalRef(env, jres);
	//(*env)->DeleteLocalRef(env, rs);

	//XXX: Not sure if we can dettach yet while this thread will continue to run
	//(*jvm)->DetachCurrentThread(jvm);

	return res;

}
int get_fd(const char * filepath) {

	int fd = -1;

	JNIEnv *env;
	jmethodID methodID;
	jint jfd;

	if (jvm == NULL) {
		LOGE("Jvm not initialized");
		return -1;
	}
	jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
	if (rs != JNI_OK) {
		LOGE("Could not get env");
		return -1;
	}
	jstring jfilepath = (*env)->NewStringUTF(env, filepath);

	jclass cls = (*env)->GetObjectClass(env, jobj);

	methodID = (*env)->GetMethodID(env, cls, "get_fd", "(Ljava/lang/String;)I");

	jfd = (jint)(*env)->CallIntMethod(env, jobj, methodID, jfilepath);
	fd = (int) jfd;

//	(*env)->DeleteLocalRef(env, jfd);
//	(*env)->DeleteLocalRef(env, cls);
//	(*env)->DeleteLocalRef(env, jfilepath);

	//(*jvm)->DetachCurrentThread(jvm);

	return fd;
}


int android_open(const char *path, int flags) {
	int fd;
	fd = get_fd(path);
	return fd;
}
int android_openm(const char *path, int flags, mode_t mode) {
	return android_open(path, flags);
}
//
int android_close(int fd) {
	int res = -1;
//First we try via android to close
	res = close_fd(fd);
//And we try Natively also
	res = close(fd);
	return res;
}

