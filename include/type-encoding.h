/*
The MIT License (MIT)

Copyright (c) 2015 Stephane Moore

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef OBJCTK_TYPE_ENCODING__
#define OBJCTK_TYPE_ENCODING__

#include "macros.h"
#include "types.h"

#import <memory>
#import <string>

/** An opaque type describing a type node. */
typedef struct _objctk_typenode *objctk_typenode;

/** An opaque type describing the result of parsing an Objective-C type encoding. */
typedef struct _objctk_typeparseresult *objctk_typeparseresult;

/** A list of status codes describing the result of parsing an Objective-C type encoding. */
OBJCTK_ENUM(objctk_statuscode, signed int,
  objctk_statuscode_NoError = 0,
  objctk_statuscode_InvalidInput = -1,
  objctk_statuscode_EncounteredInvalidToken = -2,
);

/** Returns the type category of a type node. */
OBJCTK_EXTERN objctk_typecategory objctk_typenode_getTypeCategory(objctk_typenode node);

/** Returns the range that the type node occupies in the type encoding from which it was parsed. */
OBJCTK_EXTERN objctk_range objctk_typenode_getRange(objctk_typenode node);

/**
 * Returns the range of the type name of the type node in the type encoding from which it was parsed
 * or the range { UINT_MAX, UINT_MAX } if the type node has no associated type name.
 */
OBJCTK_EXTERN objctk_range objctk_typenode_getNameRange(objctk_typenode node);

/**
 * Returns a type node representing the referenced type of the input type node or NULL if the type
 * node has no referenced type.
 */
OBJCTK_EXTERN objctk_typenode objctk_typenode_getReferencedType(objctk_typenode node);

/**
 * Creates and returns a list of type nodes representing the member types of the input type node or NULL
 * if the input type node has no member types.
 */
OBJCTK_EXTERN objctk_typenode *objctk_typenode_copyMemberTypeList(objctk_typenode node, unsigned int *outCount);

/**
 * Parses an input type encoding.
 */
OBJCTK_EXTERN objctk_typeparseresult objctk_parseTypeEncoding(const char *typeEncoding);

/**
 * Returns the status code of a parse result.
 */
OBJCTK_EXTERN objctk_statuscode objctk_typeparseresult_getStatusCode(objctk_typeparseresult parseResult);

/**
 * Returns a copy of the error description of a parse result if it exists.
 */
OBJCTK_EXTERN char *objctk_typeparseresult_copyErrorDescription(objctk_typeparseresult parseResult);

/**
 * Returns the root type node of a parse result.
 */
OBJCTK_EXTERN objctk_typenode objctk_typeparseresult_getParsedType(objctk_typeparseresult parseResult);

/**
 * Frees the memory associated with a parse result.
 */
OBJCTK_EXTERN void objctk_typeparseresult_release(objctk_typeparseresult parseResult);

#endif
