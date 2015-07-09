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

#ifndef OBJCTK_LEXER__
#define OBJCTK_LEXER__

#include "macros.h"
#include "internal-types.h"

#include <limits.h>

namespace objctk {

enum {
  // Invalid token.
  OBJCTKTokenNameInvalid = (UCHAR_MAX + 1),

  // Terminator.
  OBJCTKTokenNameEOF,

  // C99 basic types.
  OBJCTKTokenNameBasicType,

  // Compound types.
  OBJCTKTokenNameArrayDeclarationStart,
  OBJCTKTokenNameArrayDeclarationEnd,
  OBJCTKTokenNameStructDeclarationStart,
  OBJCTKTokenNameStructDeclarationEnd,
  OBJCTKTokenNameUnionDeclarationStart,
  OBJCTKTokenNameUnionDeclarationEnd,

  // Pointer types.
  OBJCTKTokenNamePointerType,
  OBJCTKTokenNameCharacterStringType,
  OBJCTKTokenNameObjCObjectPointerType,
  OBJCTKTokenNameObjCClassPointerType,
  OBJCTKTokenNameObjCSelectorType,

  // Miscellaneous types.
  OBJCTKTokenNameBitfieldType,
  OBJCTKTokenNameVoidType,
  OBJCTKTokenNameUnknownType,
};

typedef struct objctk_token {
  int name;
  objctk_lexeme value;
} objctk_token;

typedef struct objctk_lexerstate {
  const char *input;
  size_t inputLength;
  size_t index;

  objctk_lexeme lexeme;
  char lastChar;
  char peekChar;
} objctk_lexerstate;

static inline objctk_lexerstate makeLexerState(const char *input) {
  size_t inputLength = strlen(input);
  objctk_lexerstate state = {
    .input = input,
    .inputLength = inputLength,
    .index = 0,
    .lexeme = makeRange(0, 0),
    .lastChar = '\0',
    .peekChar = '\0',
  };
  return state;
}

objctk_token lexer_nextToken(objctk_lexerstate *state);

}

#endif
