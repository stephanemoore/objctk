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

#ifndef OBJCTK_TYPE_NODE__
#define OBJCTK_TYPE_NODE__

#include "internal-types.h"

#include <string.h>
#include <queue>
#include <vector>

struct _objctk_typenode;

typedef std::shared_ptr<_objctk_typenode> _objctk_typenode_ptr;
typedef std::vector<_objctk_typenode_ptr> _objctk_typenode_list;

struct _objctk_typenode {
private:
  objctk_substring m_substring;
  objctk_typecategory m_type_category;

public:
  _objctk_typenode(const objctk_substring substring, const objctk_typecategory typeCategory) :  m_substring(substring), m_type_category(typeCategory) {}
  virtual ~_objctk_typenode() {}

  objctk_typecategory typeCategory() { return m_type_category; }

  objctk_substring substring() { return m_substring; }

  virtual int typeSize() {
    objctk_typecategory typeCategory = this->typeCategory();
    switch (typeCategory) {
      case OBJCTKTypeCategorySignedChar:
      case OBJCTKTypeCategoryUnsignedChar:
        return sizeof(char);
      case OBJCTKTypeCategorySignedInt:
      case OBJCTKTypeCategoryUnsignedInt:
        return sizeof(int);
      case OBJCTKTypeCategorySignedShort:
      case OBJCTKTypeCategoryUnsignedShort:
        return sizeof(short);
      case OBJCTKTypeCategorySignedLong:
      case OBJCTKTypeCategoryUnsignedLong:
        return sizeof(long);
      case OBJCTKTypeCategorySignedLongLong:
      case OBJCTKTypeCategoryUnsignedLongLong:
        return sizeof(long long);
      case OBJCTKTypeCategoryFloat:
        return sizeof(float);
      case OBJCTKTypeCategoryDouble:
        return sizeof(double);
      case OBJCTKTypeCategoryBool:
        return sizeof(char);
      case OBJCTKTypeCategoryVoid:
        return 0;
      default:
        break;
    }
    return -1;
  }

  virtual objctk_substring typeName() { return makeRange(0, 0); }

  virtual _objctk_typenode_ptr referencedType() { return nullptr; }

  virtual _objctk_typenode_list memberTypes() { return _objctk_typenode_list(); }
};

#endif
