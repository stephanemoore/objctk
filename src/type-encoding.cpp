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

#include "type-encoding.h"

#include "parser.h"

#include <stdlib.h>

#define OBJCTK_EARLY_RETURN_ON_NULL(value, fallback) \
  if (value == NULL) { return fallback; }

using namespace objctk;

static inline objctk_range invalidRange() {
  objctk_range invalidRange = {
    .offset = UINT_MAX,
    .length = UINT_MAX,
  };
  return invalidRange;
}

objctk_typecategory objctk_typenode_getTypeCategory(objctk_typenode node) {
  OBJCTK_EARLY_RETURN_ON_NULL(node, OBJCTKTypeCategoryUnknown);
  return node->typeCategory();
}

int objctk_typenode_getTypeSize(objctk_typenode node) {
  OBJCTK_EARLY_RETURN_ON_NULL(node, -1);
  return node->typeSize();
}

objctk_range objctk_typenode_getRange(objctk_typenode node) {
  OBJCTK_EARLY_RETURN_ON_NULL(node, invalidRange());
  return node->substring();
}

objctk_range objctk_typenode_getNameRange(objctk_typenode node) {
  OBJCTK_EARLY_RETURN_ON_NULL(node, invalidRange());
  return node->typeName();
}

objctk_typenode objctk_typenode_getReferencedType(objctk_typenode node) {
  OBJCTK_EARLY_RETURN_ON_NULL(node, NULL);
  _objctk_typenode_ptr refencedTypeNodePtr = node->referencedType();
  return refencedTypeNodePtr.get();
}

objctk_typenode *objctk_typenode_copyMemberTypeList(objctk_typenode node, unsigned int *outCount) {
  OBJCTK_EARLY_RETURN_ON_NULL(node, NULL);
  _objctk_typenode_list list = node->memberTypes();

  unsigned int count = list.size();
  if (outCount != NULL) {
    *outCount = count;
  }

  if (count == 0) {
    return NULL;
  }

  objctk_typenode *memberTypeList = (objctk_typenode *)malloc(count * sizeof(objctk_typenode));

  unsigned int index = 0;
  for (_objctk_typenode_list::iterator iter = list.begin(); iter != list.end(); iter++) {
    _objctk_typenode_ptr typeNodePtr = *iter;
    memberTypeList[index] = typeNodePtr.get();
    ++index;
  }
  return memberTypeList;
}

objctk_typeparseresult objctk_parseTypeEncoding(const char *typeEncoding) {
  OBJCTK_EARLY_RETURN_ON_NULL(typeEncoding, NULL);
  objctk_typeparseresult parseResult = new _objctk_typeparseresult();
  *parseResult = parseTypeEncoding(typeEncoding);
  return parseResult;
}

objctk_statuscode objctk_typeparseresult_getStatusCode(objctk_typeparseresult parseResult) {
  OBJCTK_EARLY_RETURN_ON_NULL(parseResult, objctk_statuscode_InvalidInput);
  _objctk_parsestatus status = parseResult->status;
  return status.status_code;
}

char *objctk_typeparseresult_copyErrorDescription(objctk_typeparseresult parseResult) {
  OBJCTK_EARLY_RETURN_ON_NULL(parseResult, NULL);
  _objctk_parsestatus status = parseResult->status;
  std::string errorDescription = status.error_description;
  char *copiedErrorDescription = (char *)malloc(errorDescription.length() + 1);
  std::strcpy(copiedErrorDescription, errorDescription.c_str());
  return copiedErrorDescription;
}

objctk_typenode objctk_typeparseresult_getParsedType(objctk_typeparseresult parseResult) {
  OBJCTK_EARLY_RETURN_ON_NULL(parseResult, NULL);
  _objctk_typenode_ptr typeNodePtr = parseResult->node;
  return typeNodePtr.get();
}

OBJCTK_EXTERN void objctk_typeparseresult_release(objctk_typeparseresult parseResult) {
  delete parseResult;
}
