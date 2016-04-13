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

#if __arm64__

.macro OBJCTK_STORE_PARAMETERS
	stp	x0, x1, [sp, #(0*8)]
	stp	x2, x3, [sp, #(2*8)]
	stp	x4, x5, [sp, #(4*8)]
	stp	x6, x7, [sp, #(6*8)]
.endmacro

.macro OBJCTK_LOAD_PARAMETERS
	ldp	x0, x1, [sp, #(0*8)]
	ldp	x2, x3, [sp, #(2*8)]
	ldp	x4, x5, [sp, #(4*8)]
	ldp	x6, x7, [sp, #(6*8)]
.endmacro

/*
 * Macro arguments:
 *   arg1: The name of the interposing function.
 */
.macro OBJCTK_INTERPOSITION_PROLOGUE
  .text
  .globl $0
  .align 14
$0:
	stp	fp, lr, [sp, #-16]!
	mov	fp, sp
  sub sp, sp, #(16 + 8*8)
  OBJCTK_STORE_PARAMETERS
.endmacro

.macro OBJCTK_INTERPOSITION_INTERLUDE
  OBJCTK_LOAD_PARAMETERS
  blr x10
  OBJCTK_STORE_PARAMETERS
.endmacro

.macro OBJCTK_INTERPOSITION_EPILOGUE
  OBJCTK_LOAD_PARAMETERS
	mov	sp, fp
	ldp	fp, lr, [sp], #16
  ret
.endmacro

/**
 * Macro arguments:
 *   arg1: The name of the interposing function.
 *   arg2: The name of the prologue function to invoke.
 *   arg3: The name of the epilogue function to invoke.
 */
.macro OBJCTK_INTERPOSITION_FUNCTION
  OBJCTK_INTERPOSITION_PROLOGUE $0
  bl $1
  mov x10, x0
  OBJCTK_INTERPOSITION_INTERLUDE
  bl $2
  OBJCTK_INTERPOSITION_EPILOGUE $0
.endmacro

#endif // __arm64__
