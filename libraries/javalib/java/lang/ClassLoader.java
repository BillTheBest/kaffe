/*
 * Java core library component.
 *
 * Copyright (c) 1997, 1998
 *      Transvirtual Technologies, Inc.  All rights reserved.
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file.
 */

package java.lang;

import java.io.InputStream;
import java.io.ByteArrayInputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.SystemClassLoader;

abstract public class ClassLoader
{
	private boolean initialized = false;

protected ClassLoader() {
	System.getSecurityManager().checkCreateClassLoader();
	init();
	initialized = true;
}

final protected Class defineClass(String name, byte data[], int offset, int length) {
	return (defineClass0(name, data, offset, length));
}

final protected Class defineClass(byte data[], int offset, int length) {
	return (defineClass(null, data, offset, length));
}

native private Class defineClass0(String name, byte data[], int offset, int length);

final protected Class findLoadedClass(String name) {
	try {
		return (loadClass(name, true));	// Probably not what's wanted ...
	}
	catch (ClassNotFoundException _) {
		return (null);
	}
}

final protected Class findSystemClass(String name) throws ClassNotFoundException {
	return findSystemClass0(name);
}

native private Class findSystemClass0(String name);

public URL getResource(String name) {
	return (null);	// Default implementation just returns null
}

public InputStream getResourceAsStream(String name) {
	return (null); // Default implementation just returns null
}

final public static URL getSystemResource(String name) {
	try {
		return (new URL("system", "", 0, name));
	}
	catch (MalformedURLException _) {
		return (null);
	}
}

public static final InputStream getSystemResourceAsStream(String name) {
	byte[] data = getSystemResourceAsBytes0(name);
	if (data == null) {
		return (null);
	}
	else {
		return (new ByteArrayInputStream(data));
	}
}

/**
 *  This is not part of the public interface.
 */
native public static byte[] getSystemResourceAsBytes0(String name);

native private void init();

public Class loadClass(String name) throws ClassNotFoundException {
	return (loadClass(name, true));
}

abstract protected Class loadClass(String name, boolean resolve) throws ClassNotFoundException;

final protected void resolveClass(Class c) {
	resolveClass0(c);
}

native private void resolveClass0(Class cls);

final protected void setSigners(Class cl, Object signers[]) {
	// Signer's are not currently supported.
}

}
