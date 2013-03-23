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
// Copyright 1988-1996 NeXT Software, Inc.

#if defined(WIN32)
    #if defined(NSBUILDINGOBJC)
        #define OBJC_EXPORT __declspec(dllexport) extern
    #else
        #if !defined(OBJC_EXPORT)
            #define OBJC_EXPORT __declspec(dllimport) extern
        #endif
    #endif
    #if !defined(OBJC_IMPORT)
        #define OBJC_IMPORT __declspec(dllimport) extern
    #endif
#endif

#if !defined(OBJC_EXPORT)
    #define OBJC_EXPORT extern
#endif

#if !defined(OBJC_IMPORT)
    #define OBJC_IMPORT extern
#endif


// obsolete
#if !defined(NEXTPDO)
    #if defined(WIN32)
        #define NEXTPDO __declspec(dllimport) extern
    #else
        #define NEXTPDO extern
    #endif
#endif
