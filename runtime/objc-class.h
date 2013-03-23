/*
 * Copyright (c) 1999 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * Portions Copyright (c) 1999 Apple Computer, Inc.  All Rights
 * Reserved.  This file contains Original Code and/or Modifications of
 * Original Code as defined in and that are subject to the Apple Public
 * Source License Version 1.1 (the "License").  You may not use this file
 * except in compliance with the License.  Please obtain a copy of the
 * License at http://www.apple.com/publicsource and read it before using
 * this file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON- INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
/*
 *	objc-class.h
 *	Copyright 1988-1996, NeXT Software, Inc.
 */

#ifndef _OBJC_CLASS_H_
#define _OBJC_CLASS_H_

#import <objc/objc.h>
/* 
 *	Class Template
 */
struct objc_class {			
	struct objc_class *isa;	
	struct objc_class *super_class;	
	const char *name;		
	long version;
	long info;
	long instance_size;
	struct objc_ivar_list *ivars;

#if defined(Release3CompatibilityBuild)
	struct objc_method_list *methods;
#else
	struct objc_method_list **methodLists;
#endif

	struct objc_cache *cache;
 	struct objc_protocol_list *protocols;
};
#define CLS_GETINFO(cls,infomask)	((cls)->info & infomask)
#define CLS_SETINFO(cls,infomask)	((cls)->info |= infomask)

#define CLS_CLASS		0x1L
#define CLS_META		0x2L
#define CLS_INITIALIZED		0x4L
#define CLS_POSING		0x8L
#define CLS_MAPPED		0x10L
#define CLS_FLUSH_CACHE		0x20L
#define CLS_GROW_CACHE		0x40L
#define CLS_METHOD_ARRAY        0x100L
/* 
 *	Category Template
 */
typedef struct objc_category *Category;

struct objc_category {
	char *category_name;
	char *class_name;
	struct objc_method_list *instance_methods;
	struct objc_method_list *class_methods;
 	struct objc_protocol_list *protocols;
};
/* 
 *	Instance Variable Template
 */
typedef struct objc_ivar *Ivar;

struct objc_ivar_list {
	int ivar_count;
#ifdef __alpha__
	int space;
#endif
	struct objc_ivar {
		char *ivar_name;
		char *ivar_type;
		int ivar_offset;
#ifdef __alpha__
		int space;
#endif
	} ivar_list[1];			/* variable length structure */
};

OBJC_EXPORT Ivar object_setInstanceVariable(id, const char *name, void *);
OBJC_EXPORT Ivar object_getInstanceVariable(id, const char *name, void **);

/* 
 *	Method Template
 */
typedef struct objc_method *Method;

struct objc_method_list {
#if defined(Release3CompatibilityBuild)
        struct objc_method_list *method_next;
#else
	struct objc_method_list *obsolete;
#endif

	int method_count;
#ifdef __alpha__
	int space;
#endif
	struct objc_method {
		SEL method_name;
		char *method_types;
                IMP method_imp;
	} method_list[1];		/* variable length structure */
};

/* Protocol support */

@class Protocol;

struct objc_protocol_list {
	struct objc_protocol_list *next;
	int count;
	Protocol *list[1];
};

/* Definitions of filer types */

#define _C_ID		'@'
#define _C_CLASS	'#'
#define _C_SEL		':'
#define _C_CHR		'c'
#define _C_UCHR		'C'
#define _C_SHT		's'
#define _C_USHT		'S'
#define _C_INT		'i'
#define _C_UINT		'I'
#define _C_LNG		'l'
#define _C_ULNG		'L'
#define _C_FLT		'f'
#define _C_DBL		'd'
#define _C_BFLD		'b'
#define _C_VOID		'v'
#define _C_UNDEF	'?'
#define _C_PTR		'^'
#define _C_CHARPTR	'*'
#define _C_ARY_B	'['
#define _C_ARY_E	']'
#define _C_UNION_B	'('
#define _C_UNION_E	')'
#define _C_STRUCT_B	'{'
#define _C_STRUCT_E	'}'

/* Structure for method cache - allocated/sized at runtime */

typedef struct objc_cache *	Cache;

#define CACHE_BUCKET_NAME(B)  ((B)->method_name)
#define CACHE_BUCKET_IMP(B)   ((B)->method_imp)
#define CACHE_BUCKET_VALID(B) (B)
struct objc_cache {
	unsigned int mask;            /* total = mask + 1 */
	unsigned int occupied;        
	Method buckets[1];
};

/* operations */
OBJC_EXPORT id class_createInstance(Class, unsigned idxIvars);
OBJC_EXPORT id class_createInstanceFromZone(Class, unsigned idxIvars, void *z);

OBJC_EXPORT void class_setVersion(Class, int);
OBJC_EXPORT int class_getVersion(Class);

OBJC_EXPORT Ivar class_getInstanceVariable(Class, const char *);
OBJC_EXPORT Method class_getInstanceMethod(Class, SEL);
OBJC_EXPORT Method class_getClassMethod(Class, SEL);

OBJC_EXPORT void class_addMethods(Class, struct objc_method_list *);
OBJC_EXPORT void class_removeMethods(Class, struct objc_method_list *);

OBJC_EXPORT Class class_poseAs(Class imposter, Class original);

OBJC_EXPORT unsigned method_getNumberOfArguments(Method);
OBJC_EXPORT unsigned method_getSizeOfArguments(Method);
OBJC_EXPORT unsigned method_getArgumentInfo(Method m, int arg, const char **type, int *offset);
OBJC_EXPORT const char * NSModulePathForClass (Class aClass);

// usage for nextMethodList
//
// void *iterator = 0;
// struct objc_method_list *mlist;
// while ( mlist = class_nextMethodList( cls, &iterator ) )
//    ;
#define OBJC_NEXT_METHOD_LIST 1
OBJC_EXPORT struct objc_method_list *class_nextMethodList(Class, void **);

typedef void *marg_list;

#if hppa

#define marg_malloc(margs, method) \
	do { \
		unsigned int _sz = (7 + method_getSizeOfArguments(method)) & ~7; \
		char *_ml = (char *)malloc(_sz + sizeof(marg_list)); \
		void	**_z ; \
		margs = (marg_list *)(_ml + _sz); \
		_z = margs; \
		*_z = (marg_list)_ml; \
	} while (0)

#define marg_free(margs) \
	do { \
		void	**_z = margs; \
		free(*_z); \
	} while (0)
	
#define marg_adjustedOffset(method, offset) \
	( (!offset) ? -(sizeof(id)) : offset)

#else

#if defined(__ppc__) || defined(ppc)
#define marg_prearg_size	128
#else
#define marg_prearg_size	0
#endif

#define marg_malloc(margs, method) \
	do { \
		margs = (marg_list *)malloc (marg_prearg_size + ((7 + method_getSizeOfArguments(method)) & ~7)); \
	} while (0)


#define marg_free(margs) \
	do { \
		free(margs); \
	} while (0)
	
#define marg_adjustedOffset(method, offset) \
	(marg_prearg_size + offset)

#endif /* hppa */


#define marg_getRef(margs, offset, type) \
	( (type *)((char *)margs + marg_adjustedOffset(method,offset) ) )

#define marg_getValue(margs, offset, type) \
	( *marg_getRef(margs, offset, type) )

#define marg_setValue(margs, offset, type, value) \
	( marg_getValue(margs, offset, type) = (value) )

/* Load categories and non-referenced classes from libraries. */
#if defined(NeXT_PDO)
#if defined(__hpux__) || defined(hpux)

#define OBJC_REGISTER_SYMBOL(NAME) asm("._" #NAME "=0\n .globl ._" #NAME "\n")
#define OBJC_REFERENCE_SYMBOL(NAME) asm(".SPACE $PRIVATE$\n\t.SUBSPA $DATA$\n\t.word ._" #NAME "\n\t.SPACE $TEXT$\n\t.SUBSPA $CODE$\n")
#define OBJC_REGISTER_CATEGORY(NAME) asm("._" #NAME "=0\n .globl ._" #NAME "\n")
#define OBJC_REFERENCE_CATEGORY(NAME) asm(".SPACE $PRIVATE$\n\t.SUBSPA $DATA$\n\t.word ._" #NAME "\n\t.SPACE $TEXT$\n\t.SUBSPA $CODE$\n")
#define OBJC_REFERENCE_CLASS_CATEGORY(CL, CAT) asm(".SPACE $PRIVATE$\n\t.SUBSPA $DATA$\n\t.word .objc_category_name_" #CL "_" #CAT "\n\t.SPACE $TEXT$\n\t.SUBSPA $CODE$\n")
#define OBJC_REFERENCE_CLASS(NAME) asm(".SPACE $PRIVATE$\n\t.SUBSPA $DATA$\n\t.word .objc_class_name_" #NAME "\n\t.SPACE $TEXT$\n\t.SUBSPA $CODE$\n")

#elif defined(__osf__)

#define OBJC_REGISTER_SYMBOL(NAME) asm(".globl ._" #NAME "\n\t.align 3\n._" #NAME ":\n\t.quad 0\n")
#define OBJC_REFERENCE_SYMBOL(NAME) asm(".align 3\n\t.quad ._" #NAME "\n")
#define OBJC_REGISTER_CATEGORY(NAME) asm(".globl ._" #NAME "\n\t.align 3\n._" #NAME ":\n\t.quad 0\n")
#define OBJC_REFERENCE_CATEGORY(NAME) asm(".align 3\n\t.quad ._" #NAME "\n")
#define OBJC_REFERENCE_CLASS_CATEGORY(CL, CAT) asm(".align 3\n\t.quad .objc_category_name_" #CL "_" #CAT "\n")
#define OBJC_REFERENCE_CLASS(NAME) asm(".quad .objc_class_name_" #NAME "\n")

#else	/* Solaris || SunOS */

#define OBJC_REGISTER_SYMBOL(NAME) asm("._" #NAME "=0\n .globl ._" #NAME "\n")
#define OBJC_REFERENCE_SYMBOL(NAME) asm(".global ._" #NAME "\n")
#define OBJC_REGISTER_CATEGORY(NAME) asm("._" #NAME "=0\n .globl ._" #NAME "\n")
#define OBJC_REFERENCE_CATEGORY(NAME) asm(".global ._" #NAME "\n")
#define OBJC_REFERENCE_CLASS_CATEGORY(CL, CAT) asm(".global .objc_category_name_" #CL "_" #CAT "\n")
#define OBJC_REFERENCE_CLASS(NAME) asm(".global .objc_class_name_" #NAME "\n")

#endif /* __hpux__ || hpux */
#endif /* NeXT_PDO */

#endif /* _OBJC_CLASS_H_ */
