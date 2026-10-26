#ifdef __x86_64__

	.text
#include <asm/nospec-branch.h>

	.align 4
	.globl klcko_setjmp
	.type klcko_setjmp, @function
klcko_setjmp:
	pop  %rsi			# Return address, and adjust the stack
	xorl %eax,%eax			# Return value
	movq %rbx,(%rdi)
	movq %rsp,8(%rdi)		# Post-return %rsp!
	push %rsi			# Make the call/return stack happy
	movq %rbp,16(%rdi)
	movq %r12,24(%rdi)
	movq %r13,32(%rdi)
	movq %r14,40(%rdi)
	movq %r15,48(%rdi)
	movq %rsi,56(%rdi)		# Return address
	RET

	.size klcko_setjmp,.-klcko_setjmp

	.text
	.align 4
	.globl klcko_longjmp
	.type klcko_longjmp, @function
klcko_longjmp:
	movl %esi,%eax			# Return value (int)
	movq (%rdi),%rbx
	movq 8(%rdi),%rsp
	movq 16(%rdi),%rbp
	movq 24(%rdi),%r12
	movq 32(%rdi),%r13
	movq 40(%rdi),%r14
	movq 48(%rdi),%r15
	ANNOTATE_RETPOLINE_SAFE
	jmp *56(%rdi)

	.size klcko_longjmp,.-klcko_longjmp

#endif

#ifdef __aarch64__

	.text
	.balign 8
	.globl klcko_setjmp
	.type klcko_setjmp, #function
klcko_setjmp:
	mov	x1, sp
	stp	x18, x19, [x0, #0]
	stp	x20, x21, [x0, #16]
	stp	x22, x23, [x0, #32]
	stp	x24, x25, [x0, #48]
	stp	x26, x27, [x0, #64]
	stp	x28, x29, [x0, #80]
	stp	x30, x1,  [x0, #96]
	mov	x0, #0 			/* set the return value of klcko_setjmp */
	br	x30
	.size klcko_setjmp,.-klcko_setjmp

	.text
	.balign 8
	.globl klcko_longjmp
	.type klcko_longjmp, #function
klcko_longjmp:
	ldp	x18, x19, [x0, #0]
	ldp	x20, x21, [x0, #16]
	ldp	x22, x23, [x0, #32]
	ldp	x24, x25, [x0, #48]
	ldp	x26, x27, [x0, #64]
	ldp	x28, x29, [x0, #80]
	ldp	x30, x2,  [x0, #96]
	mov	sp, x2
	mov	x0, x1
	cbnz	x1, 1f
	mov	x0, #1
1:
	br	x30
	.size klcko_longjmp,.-klcko_longjmp

#endif

#ifdef __arm__

	.text
	.balign 4
	.globl klcko_setjmp
	.type klcko_setjmp, #function
klcko_setjmp:
	stmia	r0, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	mov	r0, #0
	mov	r1, #0
	bx  lr
	.size klcko_setjmp,.-klcko_setjmp

	.text
	.balign 4
	.globl klcko_longjmp
	.type klcko_longjmp, #function
klcko_longjmp:
	ldmia	r0, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	movs	r0, r1
	moveq	r0, #1
	mov	    r1, #0
	bx      lr
	.size klcko_longjmp,.-klcko_longjmp

#endif
