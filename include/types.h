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

#ifndef OBJCTK_TYPES__
#define OBJCTK_TYPES__

#include "macros.h"

#include <string.h>

/** An enum describing an encoding type. */
OBJCTK_ENUM(objctk_typecategory, signed int,
  OBJCTKTypeCategoryUnknown = 0,
  OBJCTKTypeCategorySignedChar,
  OBJCTKTypeCategorySignedInt,
  OBJCTKTypeCategorySignedShort,
  OBJCTKTypeCategorySignedLong,
  OBJCTKTypeCategorySignedLongLong,
  OBJCTKTypeCategoryUnsignedChar,
  OBJCTKTypeCategoryUnsignedInt,
  OBJCTKTypeCategoryUnsignedShort,
  OBJCTKTypeCategoryUnsignedLong,
  OBJCTKTypeCategoryUnsignedLongLong,
  OBJCTKTypeCategoryFloat,
  OBJCTKTypeCategoryDouble,
  OBJCTKTypeCategoryBool,
  OBJCTKTypeCategoryVoid,
  OBJCTKTypeCategoryCharacterString,
  OBJCTKTypeCategoryObject,
  OBJCTKTypeCategoryClass,
  OBJCTKTypeCategorySelector,
  OBJCTKTypeCategoryArray,
  OBJCTKTypeCategoryStruct,
  OBJCTKTypeCategoryUnion,
  OBJCTKTypeCategoryBitField,
  OBJCTKTypeCategoryPointer,

  // Special type.
  OBJCTKTypeCategoryTopLevel,
);

/** Defines a range over a serial data. */
typedef struct objctk_range {
  /** The beginning of the range. */
  size_t offset;
  /** The length of the range. */
  size_t length;
} objctk_range;

#endif
