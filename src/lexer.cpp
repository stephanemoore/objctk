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

#include "lexer.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

using namespace objctk;

static objctk_token makeToken(const int name, const objctk_lexeme value) {
  objctk_token token = {
    .name = name,
    .value = value,
  };
  return token;
}

static void lexer_nextChar(objctk_lexerstate *state) {
  size_t index = state->index;
  const char *input = state->input;
  const size_t inputLength = state->inputLength;
  state->lastChar = (index < inputLength) ? input[index] : '\0';
  state->peekChar = ((index + 1) < inputLength) ? input[index + 1] : '\0';

  objctk_lexeme *lexeme = &(state->lexeme);
  lexeme->length = lexeme->length + 1;
  state->index = index + 1;
}

static void lexer_extendLexemeUntilCharacter(objctk_lexerstate *state, const signed char ch) {
  while (state->peekChar != ch) {
    if (state->lastChar == '\0') {
      break;
    }
    lexer_nextChar(state);
  }
  lexer_nextChar(state);
}

static void lexer_consumeNumber(objctk_lexerstate *state) {
  while (isdigit(state->peekChar)) {
    lexer_nextChar(state);
  }
}

static objctk_token lexer_consumeType(objctk_lexerstate *state) {
  switch (state->lastChar) {
    case 'v': // void
      { return makeToken(OBJCTKTokenNameVoidType, state->lexeme); }
    case 'c': // char
    case 'i': // int
    case 's': // short
    case 'l': // long
    case 'q': // long long
    case 'C': // unsigned char
    case 'I': // unsigned int
    case 'S': // unsighed short
    case 'L': // unsigned long
    case 'Q': // unsigned long long
    case 'f': // float
    case 'd': // double
    case 'B': // C++ bool or C99 _Bool
      { return makeToken(OBJCTKTokenNameBasicType, state->lexeme); }
    case '*': // character string
      { return makeToken(OBJCTKTokenNameCharacterStringType, state->lexeme); }
    case '@': // Objective-C object
      {
        if (state->peekChar == '"') {
          lexer_nextChar(state);
          lexer_extendLexemeUntilCharacter(state, '"');
        }
        return makeToken(OBJCTKTokenNameObjCObjectPointerType, state->lexeme);
      }
    case '#': // Objective-C class
      { return makeToken(OBJCTKTokenNameObjCClassPointerType, state->lexeme); }
    case ':': // Objective-C selector
      { return makeToken(OBJCTKTokenNameObjCSelectorType, state->lexeme);}
    case '[': // Array type start
      {
        lexer_consumeNumber(state);
        return makeToken(OBJCTKTokenNameArrayDeclarationStart, state->lexeme);
      }
    case ']': // Array type end
      {
        return makeToken(OBJCTKTokenNameArrayDeclarationEnd, state->lexeme);
      }
    case '{': // Struct type start
      {
        lexer_extendLexemeUntilCharacter(state, '=');
        return makeToken(OBJCTKTokenNameStructDeclarationStart, state->lexeme);
      }
    case '}': // Struct type end
      {
        return makeToken(OBJCTKTokenNameStructDeclarationEnd, state->lexeme);
      }
    case '(': // Union type start
      {
        lexer_extendLexemeUntilCharacter(state, '=');
        return makeToken(OBJCTKTokenNameUnionDeclarationStart, state->lexeme);
      }
    case ')': // Union type end
      {
        return makeToken(OBJCTKTokenNameUnionDeclarationEnd, state->lexeme);
      }
    case 'b': // Bitfield type
      {
        lexer_consumeNumber(state);
        return makeToken(OBJCTKTokenNameBitfieldType, state->lexeme);
      }
    case '^': // Pointer type
      {
        return makeToken(OBJCTKTokenNamePointerType, state->lexeme);
      }
    case '?': // Unknown type
      { return makeToken(OBJCTKTokenNameUnknownType, state->lexeme); }
    default:
      break;
  }
  return makeToken(OBJCTKTokenNameInvalid, state->lexeme);
}

namespace objctk {

objctk_token lexer_nextToken(objctk_lexerstate *state) {
  objctk_lexeme lastLexeme = state->lexeme;
  state->lexeme = makeRange(lastLexeme.offset + lastLexeme.length, 0);
  lexer_nextChar(state);

  if (state->lastChar == EOF) return makeToken(OBJCTKTokenNameEOF, state->lexeme);
  if (state->lastChar == '\0') return makeToken(OBJCTKTokenNameEOF, state->lexeme);

  return lexer_consumeType(state);
}

}
