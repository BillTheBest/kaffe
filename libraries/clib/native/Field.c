/*
 * java.lang.reflect.Field.c
 *
 * Copyright (c) 1996,97 T. J. Wilkinson & Associates, London, UK.
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 * Written by Tim Wilkinson <tim@tjwassoc.co.uk>
 */

#include "config.h"
#include "config-std.h"
#include "config-mem.h"
#include "../../../kaffe/kaffevm/gtypes.h"
#include "../../../kaffe/kaffevm/access.h"
#include "../../../kaffe/kaffevm/constants.h"
#include "../../../kaffe/kaffevm/object.h"
#include "../../../kaffe/kaffevm/classMethod.h"
#include "../../../kaffe/kaffevm/itypes.h"
#include "../../../kaffe/kaffevm/support.h"
#include "../../../kaffe/kaffevm/soft.h"
#include "../../../kaffe/kaffevm/baseClasses.h"
#include "../../../kaffe/kaffevm/exception.h"
#include "java_io_InputStream.h"
#include "java_io_PrintStream.h"
#include "java_lang_System.h"
#include "java_lang_Boolean.h"
#include "java_lang_Byte.h"
#include "java_lang_Character.h"
#include "java_lang_Short.h"
#include "java_lang_Integer.h"
#include "java_lang_Long.h"
#include "java_lang_Float.h"
#include "java_lang_Double.h"
#include "java_lang_reflect_Field.h"
#include <native.h>
#include "defs.h"

static
char*
getFieldAddress(Hjava_lang_reflect_Field* this, struct Hjava_lang_Object* obj)
{
        Hjava_lang_Class* clas;
        Field* fld;

        clas = (Hjava_lang_Class*)unhand(this)->clazz;
        fld = CLASS_FIELDS(clas) + unhand(this)->slot;

        if (unhand(this)->slot < CLASS_NSFIELDS(clas)) {
                return (FIELD_ADDRESS(fld));
        }
        else {
                if (obj == NULL) {
                        SignalError("java.lang.NullPointerException", "");
                }
                if  (!soft_instanceof(clas, obj)) {
                        SignalError("java.lang.IllegalArgumentException","");
                }
                return (((char*)(obj)) + FIELD_OFFSET(fld));
        }
}

/* WHAT WITH SECURITY RESTRICTIONS !!!??? */


jint 
java_lang_reflect_Field_getModifiers(struct Hjava_lang_reflect_Field * this)
{
        Hjava_lang_Class* clas;
        Field* fld;

        clas = (Hjava_lang_Class*) unhand(this)->clazz;
        fld = CLASS_FIELDS(clas) + unhand(this)->slot;

        return ((jint)(fld->accflags & ACC_MASK));
}


struct Hjava_lang_Object*
java_lang_reflect_Field_get(Hjava_lang_reflect_Field* this, struct Hjava_lang_Object* obj)
{
        Hjava_lang_Class* clas;
        Field* fld;
        char* base;

        clas = (Hjava_lang_Class*) unhand(this)->clazz;
        fld = CLASS_FIELDS(clas) + unhand(this)->slot;
        base = getFieldAddress(this, obj);

	if ( fld->type == _Jv_intClass ) {
		return execute_java_constructor(0,javaLangIntegerClass,"(I)V",*((jint*)base));
	}
	else if ( fld->type == _Jv_shortClass ) {
		return execute_java_constructor(0,javaLangShortClass,"(S)V",*(jshort*)base);
	}
	else if ( fld->type == _Jv_longClass ) {
		return execute_java_constructor(0,javaLangLongClass,"(J)V",*(jlong*)base);
	}
	else if ( fld->type == _Jv_byteClass ) {
		return execute_java_constructor(0,javaLangByteClass,"(B)V",*(jbyte*)base);
	}
	else if ( fld->type == _Jv_booleanClass ) {
		return execute_java_constructor(0,javaLangBooleanClass,"(Z)V",*(jbool*)base);
	}
	else if ( fld->type == _Jv_charClass ) {
		return execute_java_constructor(0,javaLangCharacterClass,"(C)V",*(jchar*)base);
	}
	else if ( fld->type == _Jv_floatClass ) {
		return execute_java_constructor(0,javaLangFloatClass,"(F)V",*(jfloat*)base);
	}
	else if ( fld->type == _Jv_doubleClass ) {
		return execute_java_constructor(0,javaLangDoubleClass,"(D)V",*(jdouble*)base);
	}
	else {
		/* If fld->type isn't one of the above then it's either a
		 * reference or unresolved (and so a reference).
		 */
		return (*(struct Hjava_lang_Object**)base);
	}
}


jbool 
java_lang_reflect_Field_getBoolean(struct Hjava_lang_reflect_Field* this, struct Hjava_lang_Object* obj)
{
        Hjava_lang_Class* clas;
        Field* fld;
        char* base;

        clas = (Hjava_lang_Class*) unhand(this)->clazz;
        fld = CLASS_FIELDS(clas) + unhand(this)->slot;
        base = getFieldAddress(this, obj);

        if ( fld->type == _Jv_booleanClass ) {
                return *(jbool*)base;
	}
        else {
                SignalError("java.lang.IllegalArgumentException", "");
	}
}


jbyte 
java_lang_reflect_Field_getByte(struct Hjava_lang_reflect_Field* this, struct Hjava_lang_Object* obj)
{
        Hjava_lang_Class* clas;
        Field* fld;
        char* base;

        clas = (Hjava_lang_Class*) unhand(this)->clazz;
        fld = CLASS_FIELDS(clas) + unhand(this)->slot;
        base = getFieldAddress(this, obj);

        if ( fld->type == _Jv_byteClass ) {
                return *(jbyte*)base;
	}
        else {
                SignalError("java.lang.IllegalArgumentException", "");
	}
}

jchar 
java_lang_reflect_Field_getChar(struct Hjava_lang_reflect_Field* this, struct Hjava_lang_Object* obj)
{
        Hjava_lang_Class* clas;
        Field* fld;
        char* base;

        clas = (Hjava_lang_Class*) unhand(this)->clazz;
        fld = CLASS_FIELDS(clas) + unhand(this)->slot;
        base = getFieldAddress(this, obj);

        if ( fld->type == _Jv_charClass ) {
                return *(jchar*)base;
	}
        else {
                SignalError("java.lang.IllegalArgumentException", "");
	}
}


jshort 
java_lang_reflect_Field_getShort(struct Hjava_lang_reflect_Field* this, struct Hjava_lang_Object* obj)
{
        Hjava_lang_Class* clas;
        Field* fld;
        char* base;

        clas = (Hjava_lang_Class*) unhand(this)->clazz;
        fld = CLASS_FIELDS(clas) + unhand(this)->slot;
        base = getFieldAddress(this, obj);

        if ( fld->type == _Jv_shortClass ) {
                return *(jshort*)base;
	}
        else if ( fld->type == _Jv_byteClass ) {
                return (jshort)(*(jbyte*)base);
	}
        else {
                SignalError("java.lang.IllegalArgumentException", "");
	}
}

jint 
java_lang_reflect_Field_getInt(struct Hjava_lang_reflect_Field* this, struct Hjava_lang_Object* obj)
{
        Hjava_lang_Class* clas;
        Field* fld;
        char* base;

        clas = (Hjava_lang_Class*) unhand(this)->clazz;
        fld = CLASS_FIELDS(clas) + unhand(this)->slot;
        base = getFieldAddress(this, obj);

        if ( fld->type == _Jv_intClass ) {
                return *(jint*)base;
	}
        else if ( fld->type == _Jv_shortClass ) {
                return (jint)(*(jshort*)base);
	}
        else if ( fld->type == _Jv_byteClass ) {
                return (jint)(*(jbyte*)base);
	}
        else if ( fld->type == _Jv_charClass ) {
                return (jint)(*(jchar*)base);
	}
        else {
                SignalError("java.lang.IllegalArgumentException", "");
	}
}

jlong 
java_lang_reflect_Field_getLong(struct Hjava_lang_reflect_Field* this, struct Hjava_lang_Object* obj)
{
        Hjava_lang_Class* clas;
        Field* fld;
        char* base;

        clas = (Hjava_lang_Class*) unhand(this)->clazz;
        fld = CLASS_FIELDS(clas) + unhand(this)->slot;
        base = getFieldAddress(this, obj);

        if ( fld->type == _Jv_longClass ) {
                return *(jlong*)base;
	}
        else if ( fld->type == _Jv_intClass ) {
                return (jlong)(*(jint*)base);
	}
        else if ( fld->type == _Jv_shortClass ) {
                return (jlong)(*(jshort*)base);
	}
        else if ( fld->type == _Jv_byteClass ) {
                return (jlong)(*(jbyte*)base);
	}
        else if ( fld->type == _Jv_charClass ) {
                return (jlong)(*(jchar*)base);
	}
        else {
                SignalError("java.lang.IllegalArgumentException", "");
	}
}

jfloat 
java_lang_reflect_Field_getFloat(struct Hjava_lang_reflect_Field* this, struct Hjava_lang_Object* obj)
{
        Hjava_lang_Class* clas;
        Field* fld;
        char* base;

        clas = (Hjava_lang_Class*) unhand(this)->clazz;
        fld = CLASS_FIELDS(clas) + unhand(this)->slot;
        base = getFieldAddress(this, obj);

        if ( fld->type == _Jv_floatClass ) {
                return *(jfloat*)base;
	}
        else if ( fld->type == _Jv_longClass ) {
                return (jfloat)(*(jlong*)base);
	}
        else if ( fld->type == _Jv_intClass ) {
                return (jfloat)(*(jint*)base);
	}
        else if ( fld->type == _Jv_shortClass ) {
                return (jfloat)(*(jshort*)base);
	}
        else if ( fld->type == _Jv_byteClass ) {
                return (jfloat)(*(jbyte*)base);
	}
        else if ( fld->type == _Jv_charClass ) {
                return (jfloat)(*(jchar*)base);
	}
        else {
                SignalError("java.lang.IllegalArgumentException", "");
	}
}

jdouble 
java_lang_reflect_Field_getDouble(struct Hjava_lang_reflect_Field* this, struct Hjava_lang_Object* obj)
{
        Hjava_lang_Class* clas;
        Field* fld;
        char* base;

        clas = (Hjava_lang_Class*) unhand(this)->clazz;
        fld = CLASS_FIELDS(clas) + unhand(this)->slot;
        base = getFieldAddress(this, obj);

        if ( fld->type == _Jv_doubleClass ) {
                return *(jdouble*)base;
	}
        else if ( fld->type == _Jv_floatClass ) {
                return (jdouble)(*(jfloat*)base);
	}
        else if ( fld->type == _Jv_longClass ) {
                return (jdouble)(*(jlong*)base);
	}
        else if ( fld->type == _Jv_intClass ) {
                return (jdouble)(*(jint*)base);
	}
        else if ( fld->type == _Jv_shortClass ) {
                return (jdouble)(*(jshort*)base);
	}
        else if ( fld->type == _Jv_byteClass ) {
                return (jdouble)(*(jbyte*)base);
	}
        else if ( fld->type == _Jv_charClass ) {
                return (jdouble)(*(jchar*)base);
	}
        else {
                SignalError("java.lang.IllegalArgumentException", "");
	}
}


void 
java_lang_reflect_Field_setBoolean(struct Hjava_lang_reflect_Field* this, struct Hjava_lang_Object* obj, jbool val )
{
        Hjava_lang_Class* clas;
        Field* fld;
        char* base;

        clas = (Hjava_lang_Class*) unhand(this)->clazz;
        fld = CLASS_FIELDS(clas) + unhand(this)->slot;
        base = getFieldAddress(this, obj);

        if (fld->accflags & ACC_FINAL) {
                SignalError("java.lang.IllegalAccessException", "field is final");
        }

        if ( fld->type == _Jv_booleanClass ) {
                *(jbool*)base = val;
	}
        else {
                SignalError("java.lang.IllegalArgumentException", "");
	}
}

void 
java_lang_reflect_Field_setByte(struct Hjava_lang_reflect_Field* this, struct Hjava_lang_Object* obj, jbyte val )
{
        Hjava_lang_Class* clas;
        Field* fld;
        char* base;

        clas = (Hjava_lang_Class*) unhand(this)->clazz;
        fld = CLASS_FIELDS(clas) + unhand(this)->slot;
        base = getFieldAddress(this, obj);

        if (fld->accflags & ACC_FINAL) {
                SignalError("java.lang.IllegalAccessException", "field is final");
        }

        if ( fld->type == _Jv_byteClass ) {
                *(jbyte*)base = val;
	}
        else if ( fld->type == _Jv_shortClass ) {
                *(jshort*)base = (jshort)val;
	}
        else if ( fld->type == _Jv_intClass ) {
                *(jint*)base = (jint)val;
	}
        else if ( fld->type == _Jv_longClass ) {
                *(jlong*)base = (jlong)val;
	}
        else if ( fld->type == _Jv_floatClass ) {
                *(jfloat*)base = (jfloat)val;
	}
        else if ( fld->type == _Jv_doubleClass ) {
                *(jdouble*)base =(jdouble)val;
	}
        else {
                SignalError("java.lang.IllegalArgumentException", "");
	}
}

void 
java_lang_reflect_Field_setChar(struct Hjava_lang_reflect_Field* this, struct Hjava_lang_Object* obj, jchar val )
{
        Hjava_lang_Class* clas;
        Field* fld;
        char* base;

        clas = (Hjava_lang_Class*) unhand(this)->clazz;
        fld = CLASS_FIELDS(clas) + unhand(this)->slot;
        base = getFieldAddress(this, obj);

        if (fld->accflags & ACC_FINAL) {
                SignalError("java.lang.IllegalAccessException", "field is final");
        }

        if ( fld->type == _Jv_charClass ) {
                *(jchar*)base = val;
	}
        else if ( fld->type == _Jv_intClass ) {
                *(jint*)base = (jint)val;
	}
        else if ( fld->type == _Jv_longClass ) {
                *(jlong*)base = (jlong)val;
	}
        else if ( fld->type == _Jv_floatClass ) {
                *(jfloat*)base = (jfloat)val;
	}
        else if ( fld->type == _Jv_doubleClass ) {
                *(jdouble*)base =(jdouble)val;
	}
        else {
                SignalError("java.lang.IllegalArgumentException", "");
	}
}

void 
java_lang_reflect_Field_setShort(struct Hjava_lang_reflect_Field* this, struct Hjava_lang_Object* obj, jshort val )
{
        Hjava_lang_Class* clas;
        Field* fld;
        char* base;

        clas = (Hjava_lang_Class*) unhand(this)->clazz;
        fld = CLASS_FIELDS(clas) + unhand(this)->slot;
        base = getFieldAddress(this, obj);

        if (fld->accflags & ACC_FINAL) {
                SignalError("java.lang.IllegalAccessException", "field is final");
        }

        if ( fld->type == _Jv_shortClass ) {
                *(jshort*)base = val;
	}
        else if ( fld->type == _Jv_intClass ) {
                *(jint*)base = (jint)val;
	}
        else if ( fld->type == _Jv_longClass ) {
                *(jlong*)base = (jlong)val;
	}
        else if ( fld->type == _Jv_floatClass ) {
                *(jfloat*)base = (jfloat)val;
	}
        else if ( fld->type == _Jv_doubleClass ) {
                *(jdouble*)base =(jdouble)val;
	}
        else {
                SignalError("java.lang.IllegalArgumentException", "");
	}
}

void 
java_lang_reflect_Field_setInt(struct Hjava_lang_reflect_Field* this, struct Hjava_lang_Object* obj, jint val )
{
        Hjava_lang_Class* clas;
        Field* fld;
        char* base;

        clas = (Hjava_lang_Class*) unhand(this)->clazz;
        fld = CLASS_FIELDS(clas) + unhand(this)->slot;
        base = getFieldAddress(this, obj);

        if (fld->accflags & ACC_FINAL) {
                SignalError("java.lang.IllegalAccessException", "field is final");
        }

        if ( fld->type == _Jv_intClass ) {
                *(jint*)base = val;
	}
        else if ( fld->type == _Jv_longClass ) {
                *(jlong*)base = (jlong)val;
	}
        else if ( fld->type == _Jv_floatClass ) {
                *(jfloat*)base = (jfloat)val;
	}
        else if ( fld->type == _Jv_doubleClass ) {
                *(jdouble*)base =(jdouble)val;
	}
        else {
                SignalError("java.lang.IllegalArgumentException", "");
	}
}

void 
java_lang_reflect_Field_setLong(struct Hjava_lang_reflect_Field* this, struct Hjava_lang_Object* obj, jlong val )
{
        Hjava_lang_Class* clas;
        Field* fld;
        char* base;

        clas = (Hjava_lang_Class*) unhand(this)->clazz;
        fld = CLASS_FIELDS(clas) + unhand(this)->slot;
        base = getFieldAddress(this, obj);

        if (fld->accflags & ACC_FINAL) {
                SignalError("java.lang.IllegalAccessException", "field is final");
        }

        if ( fld->type == _Jv_longClass ) {
                *(jlong*)base = val;
	}
        else if ( fld->type == _Jv_floatClass ) {
                *(jfloat*)base = (jfloat)val;
	}
        else if ( fld->type == _Jv_doubleClass ) {
                *(jdouble*)base = (jdouble)val;
	}
        else {
                SignalError("java.lang.IllegalArgumentException", "");
	}
}

void 
java_lang_reflect_Field_setFloat(struct Hjava_lang_reflect_Field* this, struct Hjava_lang_Object* obj, jfloat val )
{
        Hjava_lang_Class* clas;
        Field* fld;
        char* base;

        clas = (Hjava_lang_Class*) unhand(this)->clazz;
        fld = CLASS_FIELDS(clas) + unhand(this)->slot;
        base = getFieldAddress(this, obj);

        if (fld->accflags & ACC_FINAL) {
                SignalError("java.lang.IllegalAccessException", "field is final");
        }

        if ( fld->type == _Jv_floatClass ) {
                *(jfloat*)base = val;
	}
        else if ( fld->type == _Jv_doubleClass ) {
                *(jdouble*)base =(jdouble)val;
	}
        else {
                SignalError("java.lang.IllegalArgumentException", "");
	}
}

void 
java_lang_reflect_Field_setDouble(struct Hjava_lang_reflect_Field* this, struct Hjava_lang_Object* obj, jdouble val )
{
        Hjava_lang_Class* clas;
        Field* fld;
        char* base;

        clas = (Hjava_lang_Class*) unhand(this)->clazz;
        fld = CLASS_FIELDS(clas) + unhand(this)->slot;
        base = getFieldAddress(this, obj);

        if (fld->accflags & ACC_FINAL) {
                SignalError("java.lang.IllegalAccessException", "field is final");
        }

        if ( fld->type == _Jv_doubleClass ) {
                *(jdouble*)base = val;
	}
        else {
                SignalError("java.lang.IllegalArgumentException", "");
	}
}

void
java_lang_reflect_Field_set(Hjava_lang_reflect_Field* this, struct Hjava_lang_Object* obj, struct Hjava_lang_Object* val)
{
        Hjava_lang_Class* clas, *clazz;
        Field* fld;
        char* base;
	errorInfo info;
	Hjava_lang_Class* ftype;

        clas = (Hjava_lang_Class*) unhand(this)->clazz;
        fld = CLASS_FIELDS(clas) + unhand(this)->slot;

        if (fld->accflags & ACC_FINAL) {
                SignalError("java.lang.IllegalAccessException", "field is final");
        }
	
	ftype = resolveFieldType(fld, clas, &info);
	if (ftype == 0) {
		throwError(&info);
	}

	if (FIELD_ISREF(fld) && (val == NULL || soft_instanceof(ftype, val))) {
		base = getFieldAddress(this, obj);
		*(struct Hjava_lang_Object**)base = val;
		return;
	}
	
	clazz = OBJECT_CLASS(val);

	if ( clazz == javaLangIntegerClass ) {
		java_lang_reflect_Field_setInt(this, obj, unhand(((Hjava_lang_Integer *)val))->value );
	}
	else if ( clazz == javaLangBooleanClass ) {
		java_lang_reflect_Field_setBoolean(this, obj, unhand(((Hjava_lang_Boolean *)val))->value );
	}
	else if ( clazz == javaLangByteClass ) {
		java_lang_reflect_Field_setByte(this, obj, unhand(((Hjava_lang_Byte *)val))->value );
	}
	else if ( clazz == javaLangShortClass ) {
		java_lang_reflect_Field_setShort(this, obj, unhand(((Hjava_lang_Short *)val))->value );
	}
	else if ( clazz == javaLangCharacterClass ) {
		java_lang_reflect_Field_setChar(this, obj, unhand(((Hjava_lang_Character *)val))->value );
	}
	else if ( clazz == javaLangLongClass ) {
		java_lang_reflect_Field_setLong(this, obj, unhand(((Hjava_lang_Long *)val))->value );
	}
	else if ( clazz == javaLangFloatClass ) {
		java_lang_reflect_Field_setFloat(this, obj, unhand(((Hjava_lang_Float *)val))->value );
	}
	else if ( clazz == javaLangDoubleClass ) {
		java_lang_reflect_Field_setDouble(this, obj, unhand(((Hjava_lang_Double *)val))->value );
	}
	else {
		SignalError("java.lang.IllegalArgumentException", "can't assign object to a primitive type");
        }
}
