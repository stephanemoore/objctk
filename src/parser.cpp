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

#include "parser.h"

#include "lexer.h"
#include "typenode.h"
#include "typenode-subtypes.h"

#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

using namespace objctk;

static inline objctk_lexeme mergedLexeme(objctk_lexeme l1, objctk_lexeme l2) {
  size_t minOffset = std::min(l1.offset, l2.offset);
  size_t maxOffset = std::max(l1.offset + l1.length, l2.offset + l2.length);
  objctk_lexeme lexeme = {
    .offset = minOffset,
    .length = (maxOffset - minOffset),
  };
  return lexeme;
}

typedef struct objctk_parserstate {
  objctk_lexerstate lexerState;
  _objctk_parsestatus status;
} objctk_parserstate;

static inline objctk_parserstate makeParserState(const char *input) {
  objctk_parserstate parserState = {
    .lexerState = makeLexerState(input),
    .status = {
      .status_code = objctk_statuscode_NoError,
    },
  };
  return parserState;
}

#define BASIC_TYPE_MAPPING(code, type) case code: return type
static objctk_typecategory typeCategoryFromBasicTypeCode(char code) {
  switch (code) {
    BASIC_TYPE_MAPPING('c', OBJCTKTypeCategorySignedChar);
    BASIC_TYPE_MAPPING('i', OBJCTKTypeCategorySignedInt);
    BASIC_TYPE_MAPPING('s', OBJCTKTypeCategorySignedShort);
    BASIC_TYPE_MAPPING('l', OBJCTKTypeCategorySignedLong);
    BASIC_TYPE_MAPPING('q', OBJCTKTypeCategorySignedLongLong);
    BASIC_TYPE_MAPPING('C', OBJCTKTypeCategoryUnsignedChar);
    BASIC_TYPE_MAPPING('I', OBJCTKTypeCategoryUnsignedInt);
    BASIC_TYPE_MAPPING('S', OBJCTKTypeCategoryUnsignedShort);
    BASIC_TYPE_MAPPING('L', OBJCTKTypeCategoryUnsignedLong);
    BASIC_TYPE_MAPPING('Q', OBJCTKTypeCategoryUnsignedLongLong);
    BASIC_TYPE_MAPPING('f', OBJCTKTypeCategoryFloat);
    BASIC_TYPE_MAPPING('d', OBJCTKTypeCategoryDouble);
    BASIC_TYPE_MAPPING('B', OBJCTKTypeCategoryBool);
    default:
      return OBJCTKTypeCategoryUnknown;
  }
}
#undef BASIC_TYPE_MAPPING

#define LOCAL_LEXEME_BUFFER(typeEncoding, lexemeBufferName, lexeme) \
  char lexemeBufferName[lexeme.length + 1]; \
  memcpy(lexemeBufferName, typeEncoding + lexeme.offset, lexeme.length); \
  lexemeBufferName[lexeme.length] = '\0'

static inline void logUnexpectedToken(objctk_parserstate *parserState, objctk_token token) {
  LOCAL_LEXEME_BUFFER(parserState->lexerState.input, unexpectedLexeme, token.value);
  printf("Unexpected token:  %d ('%s')\n", token.name, unexpectedLexeme);
}

static _objctk_typenode_ptr parseCompositeType(objctk_parserstate *parserState, const size_t starting_offset, const objctk_token *startingToken);

static _objctk_typenode_ptr parseTypeFromToken(objctk_parserstate *parserState, objctk_token token) {
  _objctk_typenode_ptr typeNode = nullptr;
  const char *input = parserState->lexerState.input;
  switch (token.name) {
    case OBJCTKTokenNameBasicType:
      typeNode = _objctk_typenode_ptr(new _objctk_typenode(token.value, typeCategoryFromBasicTypeCode(input[token.value.offset])));
      break;
    case OBJCTKTokenNameUnknownType:
      typeNode = _objctk_typenode_ptr(new _objctk_typenode(token.value, OBJCTKTypeCategoryUnknown));
      break;
    case OBJCTKTokenNameVoidType:
      typeNode = _objctk_typenode_ptr(new _objctk_typenode(token.value, OBJCTKTypeCategoryVoid));
      break;
    case OBJCTKTokenNameCharacterStringType:
      typeNode = _objctk_typenode_ptr(new pointernode(token.value, OBJCTKTypeCategoryCharacterString, nullptr));
      break;
    case OBJCTKTokenNameStructDeclarationStart:
      typeNode = parseCompositeType(parserState, token.value.offset, &token);
      break;
    case OBJCTKTokenNameUnionDeclarationStart:
      typeNode = parseCompositeType(parserState, token.value.offset, &token);
      break;
    case OBJCTKTokenNamePointerType: {
      objctk_token nextToken = lexer_nextToken(&(parserState->lexerState));
      _objctk_typenode_ptr subtypeNode = parseTypeFromToken(parserState, nextToken);
      typeNode = _objctk_typenode_ptr(new pointernode(token.value, OBJCTKTypeCategoryPointer, subtypeNode));
      break;
    }
    case OBJCTKTokenNameBitfieldType: {
      size_t bitfield_size = (size_t)atoi(input + (token.value.offset + 1));
      typeNode = _objctk_typenode_ptr(new bitfieldnode(token.value, bitfield_size));
      break;
    }
    case OBJCTKTokenNameArrayDeclarationStart: {
      size_t array_size = (size_t)atoi(input + (token.value.offset + 1));
      objctk_token nextToken = lexer_nextToken(&(parserState->lexerState));
      _objctk_typenode_ptr subtypeNode = parseTypeFromToken(parserState, nextToken);
      objctk_token terminatingToken = lexer_nextToken(&(parserState->lexerState));
      if (terminatingToken.name != OBJCTKTokenNameArrayDeclarationEnd) {
        logUnexpectedToken(parserState, terminatingToken);
      }
      objctk_substring fullSubstring = mergedLexeme(token.value, terminatingToken.value);
      typeNode = _objctk_typenode_ptr(new arraynode(fullSubstring, subtypeNode, array_size));
      break;
    }
    case OBJCTKTokenNameObjCObjectPointerType: {
      objctk_substring typeName = token.value;
      typeName.offset++;
      typeName.length--;
      typeNode = _objctk_typenode_ptr(new objectpointernode(token.value, typeName));
      break;
    }
    case OBJCTKTokenNameObjCClassPointerType:
      typeNode = _objctk_typenode_ptr(new pointernode(token.value, OBJCTKTypeCategoryClass, nullptr));
      break;
    case OBJCTKTokenNameObjCSelectorType:
      typeNode = _objctk_typenode_ptr(new pointernode(token.value, OBJCTKTypeCategorySelector, nullptr));
      break;
    default:
      break;
  }
  return typeNode;
}

static _objctk_typenode_ptr parseCompositeType(objctk_parserstate *parserState, const size_t starting_offset, const objctk_token *startingToken) {
  objctk_substring substring = {
    .offset = starting_offset, // parserState->lexerState.index ?
    .length = 0,
  };

  // The terminating token is inferred from the starting token.
  int terminatingTokenName = OBJCTKTokenNameEOF;
  objctk_substring compositeTypeName = makeRange(0, 0);
  if (startingToken != NULL) {
    objctk_lexeme startingTokenValue = startingToken->value;
    compositeTypeName = makeRange(startingTokenValue.offset + 1, startingTokenValue.length - 2);
    switch (startingToken->name) {
      case OBJCTKTokenNameStructDeclarationStart:
        terminatingTokenName = OBJCTKTokenNameStructDeclarationEnd;
        break;
      case OBJCTKTokenNameUnionDeclarationStart:
        terminatingTokenName = OBJCTKTokenNameUnionDeclarationEnd;
        break;
      default:
        break;
    }
  }

  const char *input = parserState->lexerState.input;
  _objctk_typenode_list typeNodes;
  while (true) {
    objctk_token token = lexer_nextToken(&(parserState->lexerState));

    if ((token.name == terminatingTokenName) || (token.name == OBJCTKTokenNameEOF)) {
      break;
    }

    _objctk_typenode_ptr typeNode = parseTypeFromToken(parserState, token);
    if (typeNode.get() != NULL) {
      typeNodes.push_back(typeNode);
      continue;
    }
    logUnexpectedToken(parserState, token);
  }

  if ((startingToken == NULL) && (typeNodes.size() == 1)) {
    return typeNodes[0];
  }

  substring.length = (parserState->lexerState.index - substring.offset);
  _objctk_typenode_ptr typeNode = _objctk_typenode_ptr(new compositetypenode(substring, OBJCTKTypeCategoryTopLevel, typeNodes, compositeTypeName));
  return typeNode;
}

namespace objctk {

_objctk_typeparseresult parseTypeEncoding(const char *typeEncoding) {
  objctk_parserstate parserState = makeParserState(typeEncoding);
  _objctk_typenode_ptr typeNode = parseCompositeType(&parserState, 0, NULL);
  _objctk_typeparseresult result = {
    .node = typeNode,
    .status = parserState.status
  };
  return result;
}

}
