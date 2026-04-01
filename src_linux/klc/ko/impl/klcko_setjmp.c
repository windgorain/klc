/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#include "klcko_impl.h"
#include "ko/ko_errcode.h"
#include "klc/klc_kv_def.h"
#include "klc/klc_nl_def.h"

#ifdef __x86_64__
asm (
"   .text\n"
"   .type		klcko_setjmp, @function\n"
"   .globl		klcko_setjmp\n"
" klcko_setjmp:"
"   pop  %rsi\n"            
"   xorl %eax,%eax\n"       
"   movq %rbx,(%rdi)\n"
"   movq %rsp,8(%rdi)\n"    
"   push %rsi\n"            
"   movq %rbp,16(%rdi)\n"
"   movq %r12,24(%rdi)\n"
"   movq %r13,32(%rdi)\n"
"   movq %r14,40(%rdi)\n"
"   movq %r15,48(%rdi)\n"
"   movq %rsi,56(%rdi)\n"   
"   ret\n"
"	.size		klcko_setjmp, .-klcko_setjmp\n"
);

asm (
"   .text\n"
"   .type klcko_longjmp, @function\n"
"   .globl klcko_longjmp\n"
" klcko_longjmp:"
"   movl %esi,%eax\n"   
"   movq (%rdi),%rbx\n"
"   movq 8(%rdi),%rsp\n"
"   movq 16(%rdi),%rbp\n"
"   movq 24(%rdi),%r12\n"
"   movq 32(%rdi),%r13\n"
"   movq 40(%rdi),%r14\n"
"   movq 48(%rdi),%r15\n"
"   jmp *56(%rdi)\n"
"	.size		klcko_longjmp, .-klcko_longjmp\n"
);
#endif

#ifdef __aarch64__
asm (
"   .text\n"
"   .balign 8\n"
"   .type   klcko_setjmp, @function\n"
"   .globl  klcko_setjmp\n"
" klcko_setjmp:"
"   mov	x1, sp\n"
"   stp	x18, x19, [x0, #0]\n"
"   stp	x20, x21, [x0, #16]\n"
"   stp	x22, x23, [x0, #32]\n"
"   stp	x24, x25, [x0, #48]\n"
"   stp	x26, x27, [x0, #64]\n"
"   stp	x28, x29, [x0, #80]\n"
"   stp	x30, x1,  [x0, #96]\n"
"   mov	x0, #0\n" 			
"   br	x30\n"
"	.size		klcko_setjmp, .-klcko_setjmp\n"
);

asm (
"   .text\n"
"   .balign 8\n"
"   .type klcko_longjmp, @function\n"
"   .globl klcko_longjmp\n"
" klcko_longjmp:"
"   ldp	x18, x19, [x0, #0]\n"
"   ldp	x20, x21, [x0, #16]\n"
"   ldp	x22, x23, [x0, #32]\n"
"   ldp	x24, x25, [x0, #48]\n"
"   ldp	x26, x27, [x0, #64]\n"
"   ldp	x28, x29, [x0, #80]\n"
"   ldp	x30, x2,  [x0, #96]\n"
"   mov	sp, x2\n"
"   mov	x0, x1\n"
"   cbnz	x1, 1f\n"
"   mov	x0, #1\n"
" 1:"
"   br	x30\n"
"	.size		klcko_longjmp, .-klcko_longjmp\n"
);
#endif

