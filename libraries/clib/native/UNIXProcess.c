/*
 * kaffe.lang.UNIXProcess.c
 *
 * Copyright (c) 1996, 1997
 *	Transvirtual Technologies, Inc.  All rights reserved.
 *
 * See the file "license.terms" for information on usage and redistribution 
 * of this file. 
 */

/* Note: this file uses JNI throughout. */

#include "config.h"
#include "config-std.h"
#include "config-mem.h"
#include "config-io.h"
#include "config-signal.h"
#include <jni.h>
#include <jsyscall.h>
#include <native.h>
#include <files.h>
#include "../../../include/system.h"
#include "kaffe_lang_UNIXProcess.h"

static void
freevec(char **v)
{
	char **p = v;
	if (v) {
		while (*p) KFREE(*p++);
		KFREE(v);
	}
}

jint
Java_kaffe_lang_UNIXProcess_forkAndExec(JNIEnv* env, jobject proc, jarray args, jarray envs)
{
	jint pid;
	char** argv;
	char** arge;
	jint ioes[4];
	int arglen;
	int envlen;
	int i;
	int rc;
	jclass ioexc_class = (*env)->FindClass(env, "java.io.IOException");
	jclass proc_class;
	jfieldID pidField;
	/* the names given to the stream in Java */
	const char *fd_names[] = { "stdin_fd", 
				  "stdout_fd", 
				  "stderr_fd", 
				  "sync_fd"};

	arglen = (args ? (*env)->GetArrayLength(env, args) : 0);
	envlen = (envs ? (*env)->GetArrayLength(env, envs) : 0);

	if (arglen < 1) {
		(*env)->ThrowNew(env, ioexc_class, "No such file");
		return -1;
	}

	/* Build arguments and environment */
	argv = KCALLOC(arglen + 1, sizeof(jbyte*));
	if (!argv) {
		errorInfo info;
		postOutOfMemory(&info);
		throwError(&info);
	}
	for (i = 0; i < arglen; i++) {
		jstring argi;
		const jbyte *argichars;

		argi = (jstring)(*env)->GetObjectArrayElement(env, args, i);
		argichars = (*env)->GetStringUTFChars(env, argi, NULL);
		argv[i] = KMALLOC(strlen(argichars) + 1);
		if (argv[i]) {
			strcpy(argv[i], argichars);
		}
		(*env)->ReleaseStringUTFChars(env, argi, argichars);
		if (!argv[i]) {
			errorInfo info;

			freevec(argv);
			postOutOfMemory(&info);
			throwError(&info);
		}
	}

	if (envlen > 0) {
		arge = KCALLOC(envlen + 1, sizeof(jbyte*));
		if (!arge) {
			errorInfo info;

			freevec(argv);
			postOutOfMemory(&info);
			throwError(&info);
		}
	} else
		arge = NULL;

	for (i = 0; i < envlen; i++) {
		jstring envi;
		const jbyte *envichars;

		envi = (jstring)(*env)->GetObjectArrayElement(env, envs, i);
		envichars = (*env)->GetStringUTFChars(env, envi, NULL);
		arge[i] = KMALLOC(strlen(envichars) + 1);
		if (arge[i]) {
			strcpy(arge[i], envichars);
		}
		(*env)->ReleaseStringUTFChars(env, envi, envichars);
		if (!arge[i]) {
			errorInfo info;

			freevec(argv);
			freevec(arge);
			postOutOfMemory(&info);
			throwError(&info);
		}
	}

	/* Search program in path */
	rc = ENOENT;
	if (strstr (argv[0], file_separator) == NULL) {
		char **var;
		char *ptr, *nptr;

		/* Search PATH variable */
		ptr = NULL;
		for (var = arge; var != NULL; var++) {
			if (strncmp (*var, "PATH=", 5) == 0) {
				ptr = *var + 5;
				break;
			}
		}
		if (ptr == NULL) {
			ptr = getenv ("PATH");
		}

		/* Search program in path */
		for (; ptr != NULL; ptr = nptr) {
			char *file;

			nptr = strstr (ptr, path_separator);
			if (nptr == NULL) {
				file = KMALLOC (strlen (ptr)
						+ strlen (file_separator)
						+ strlen (argv[0]) + 1);
				if (!file) {
					errorInfo info;

					freevec(argv);
					freevec(arge);
					postOutOfMemory(&info);
					throwError(&info);
				}
				strcpy (file, ptr);
				strcat (file, file_separator);
			}
			else if (nptr == ptr) {
				/* Assume empty allow current dir */
				file = KMALLOC (strlen (argv[0]) + 1);
				nptr += strlen (path_separator);
			}
			else {
				file = KMALLOC (nptr - ptr
						+ strlen (file_separator)
						+ strlen (argv[0]) + 1);
				if (!file) {
					errorInfo info;

					freevec(argv);
					freevec(arge);
					postOutOfMemory(&info);
					throwError(&info);
				}
				strncpy (file, ptr, nptr - ptr);
				file[nptr - ptr] = 0;
				strcat (file, file_separator);
				nptr += strlen (path_separator);
			}
			strcat (file, argv[0]);
			if (access (file, X_OK) != -1) {
				rc = 0;
				nptr = NULL;
			}
			KFREE (file);
		}
	}
	else if (access (argv[0], X_OK) != -1) {
		rc = 0;
	}
	if (!rc) {
		rc = KFORKEXEC(argv, arge, ioes, &pid);
	}

	freevec(argv);
	freevec(arge);
	if (rc) {
		(*env)->ThrowNew(env, ioexc_class, SYS_ERROR(rc));
		return (-1);
	}

	/* get kaffe.lang.UNIXProcess class */
	proc_class = (*env)->GetObjectClass(env, proc);

	/* Save PID */
	pidField = (*env)->GetFieldID(env, proc_class, "pid", "I");
	(*env)->SetIntField(env, proc, pidField, pid);

	/*
	 * Note: even though it is likely that `pfd_field' and `fd_field'
	 * will be identical throughout the loop, this code does not assume
	 * that.  Hence, it will work even when used with reloaded classes.
	 */
	for (i = 0; i < 4; i++) {
		jfieldID pfd_field = (*env)->GetFieldID(env, proc_class, 
			    fd_names[i], "java.io.FileDescriptor");
		jobject fdi = (*env)->GetObjectField(env, proc, pfd_field);
		jfieldID fd_field = (*env)->GetFieldID(env, 
					(*env)->GetObjectClass(env, fdi), 
					"fd", "I");
		(*env)->SetIntField(env, fdi, fd_field, ioes[i]);
	}

	return (0);
}

/*
 * Destroy this process
 */
void 
Java_kaffe_lang_UNIXProcess_destroy(JNIEnv* env, jobject proc)
{
#if defined(HAVE_KILL)
	jfieldID pid = (*env)->GetFieldID(env, 
				(*env)->GetObjectClass(env, proc), 
				"pid", "I");
	KKILL((*env)->GetIntField(env, proc, pid), SIGTERM);
#else
	unimp("kill() not provided");
#endif
}

/*
 * Wait for a process to exit.
 */
jint
Java_kaffe_lang_UNIXProcess_execWait(JNIEnv* env, jobject process)
{
	int npid;
	int status = -1;
	jfieldID pid = (*env)->GetFieldID(env, 
				(*env)->GetObjectClass(env, process), 
				"pid", "I");
	KWAITPID((*env)->GetIntField(env, process, pid), &status, 0, &npid);
	return (status);
}

/*
 * Send a process a signal
 */
void
Java_kaffe_lang_UNIXProcess_sendSignal0(JNIEnv *e, jclass c, jint pid, jint sig)
{
#if defined(HAVE_KILL)
	KKILL(pid, sig);
#else
	unimp("kill() not provided");
#endif
}

