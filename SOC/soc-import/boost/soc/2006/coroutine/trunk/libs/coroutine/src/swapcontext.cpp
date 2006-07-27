#if !defined(i386)
#error This file is for x86 CPUs only.
#endif

#if !defined(__GNUC__)
#error This file requires gcc.
#endif

/*
 * This file should really be a plain assembler file.
 * Unfortunately Boost.Build v2 doesn't handle asm files (yet).
 * For now resort to inline asm.
 */


/* 
   EAX is &from.sp	
   EDX is to.sp
   This is the simplest version of swapcontext
   It saves registers on the old stack,
   saves the old stack pointer, 
   load the new stack pointer,
   pop registers from the new stack
   and returns to new caller.
   EAX is simpy passed to the function it returns to.
   The first time EAX is the first parameter of the trampoline.
   Otherwise it is simply discarded.
   NOTE: This function should work on any IA32 CPU.
   NOTE: The biggest penality is the last jump that
   will be always mispredicted (~50 cycles on P4).
   We try to make its address available as soon as possible
   to try to reduce the penality. Doing a ret instead of a 
   'add $4, %esp'
   'jmp *%ecx'
   really kills performance.
   NOTE: popl is slightly better than mov+add to pop registers
   so is pushl rather than mov+sub.
   */
#define BOOST_COROUTINE_swapcontext(name) \
asm volatile (                            \
".text \n\t"                              \
".global " #name "\n\t"                   \
".type " #name "@function\n\t"            \
".align 16\n\t"                           \
#name":\n\t"                              \
"movl  16(%edx), %ecx\n\t"                \
"pushl %ebp\n\t"                          \
"pushl %ebx\n\t"                          \
"pushl %esi\n\t"                          \
"pushl %edi\n\t"                          \
"movl  %esp, (%eax)\n\t"                  \
"movl  %edx, %esp\n\r"                    \
"popl  %edi\n\t"                          \
"popl  %esi\n\t"                          \
"popl  %ebx\n\t"                          \
"popl  %ebp\n\t"                          \
"add   $4, %esp\n\t"                      \
"jmp   *%ecx\n\t"                         \
"ud2\n\t")                                \
/**/

BOOST_COROUTINE_swapcontext(swapcontext_stack);
BOOST_COROUTINE_swapcontext(swapcontext_stack2);
BOOST_COROUTINE_swapcontext(swapcontext_stack3);

#undef BOOST_COROUTINE_swapcontext
