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

#ifndef OBJCTK_MACRO__
#define OBJCTK_MACRO__

#if !defined(OBJCTK_EXTERN)
#  if defined(__cplusplus)
#    define OBJCTK_EXTERN extern "C"
#  else
#    define OBJCTK_EXTERN extern
#  endif
#endif

#if defined(__has_feature) && defined(__has_extension)
#  if __has_feature(objc_fixed_enum) || __has_extension(cxx_strong_enums)
#    define OBJCTK_ENUM(name, type, ...) typedef enum : type { __VA_ARGS__ } name
#  else
#    define OBJCTK_ENUM(name, type, ...) enum { __VA_ARGS__ }; typedef type name
#  endif
#endif

#endif
