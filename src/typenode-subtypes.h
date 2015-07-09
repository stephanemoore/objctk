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

#ifndef OBJCTK_TYPE_NODE_SUBTYPES__
#define OBJCTK_TYPE_NODE_SUBTYPES__

#include "typenode.h"

#include <string.h>

namespace objctk {

/**
 * A parse tree node representing a bitfield type.
 */
class bitfieldnode : public _objctk_typenode {
  const size_t m_size;
public:
  bitfieldnode(const objctk_substring substring, size_t size) : _objctk_typenode(substring, OBJCTKTypeCategoryBitField), m_size(size) {}
};

/**
 * A parse tree node representing a pointer type.
 */
class pointernode : public _objctk_typenode {
  const _objctk_typenode_ptr m_referenced_type;
public:
  pointernode(const objctk_substring substring, const objctk_typecategory typeCategory, const _objctk_typenode_ptr typeNode) : _objctk_typenode(substring, typeCategory), m_referenced_type(typeNode) {}

  virtual _objctk_typenode_ptr referencedType() { return m_referenced_type; }
};

/**
 * A parse tree node representing an array type.
 */
class arraynode : public pointernode {
  const size_t m_size;
public:
  arraynode(const objctk_substring substring, const _objctk_typenode_ptr Node, const size_t size) : pointernode(substring, OBJCTKTypeCategoryArray, Node), m_size(size) {}
};

/**
 * A parse tree node representing an object pointer type.
 */
class objectpointernode : public _objctk_typenode {
  const objctk_substring m_type_name;
public:
  objectpointernode(const objctk_substring substring, const objctk_substring typeName) : _objctk_typenode(substring, OBJCTKTypeCategoryObject), m_type_name(typeName) {}

  virtual objctk_substring typeName() { return m_type_name; }
};

/**
 * A parse tree node representing a composite type.
 */
class compositetypenode : public _objctk_typenode {
  const _objctk_typenode_list m_member_types;
  const objctk_substring m_type_name;
public:
  compositetypenode(const objctk_substring substring, const objctk_typecategory typeCategory, const _objctk_typenode_list memberTypes, const objctk_substring typeName) : _objctk_typenode(substring, typeCategory), m_member_types(memberTypes), m_type_name(typeName) {}

  virtual objctk_substring typeName() { return m_type_name; }
  virtual _objctk_typenode_list memberTypes() { return m_member_types; }
};

}

#endif
