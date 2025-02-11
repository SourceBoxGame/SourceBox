//===---------------------------- libunwind.h -----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//
// Compatible with libuwind API documented at:
//   http://www.nongnu.org/libunwind/man/libunwind(3).html
//
//===----------------------------------------------------------------------===//

#ifndef __LIBUNWIND__
#define __LIBUNWIND__

#include <__libunwind_config.h>

#include <stdint.h>
#include <stddef.h>

#ifdef __APPLE__
  #include <Availability.h>
    #ifdef __arm__
       #define LIBUNWIND_AVAIL __attribute__((unavailable))
    #else
      #define LIBUNWIND_AVAIL __OSX_AVAILABLE_STARTING(__MAC_10_6, __IPHONE_5_0)
    #endif
#else
  #define LIBUNWIND_AVAIL
#endif

/* error codes */
enum {
  UNW_ESUCCESS      = 0,     /* no error */
  UNW_EUNSPEC       = -6540, /* unspecified (general) error */
  UNW_ENOMEM        = -6541, /* out of memory */
  UNW_EBADREG       = -6542, /* bad register number */
  UNW_EREADONLYREG  = -6543, /* attempt to write read-only register */
  UNW_ESTOPUNWIND   = -6544, /* stop unwinding */
  UNW_EINVALIDIP    = -6545, /* invalid IP */
  UNW_EBADFRAME     = -6546, /* bad frame */
  UNW_EINVAL        = -6547, /* unsupported operation or bad value */
  UNW_EBADVERSION   = -6548, /* unwind info has unsupported version */
  UNW_ENOINFO       = -6549  /* no unwind info found */
};

struct unw_context_t {
  uint64_t data[_LIBUNWIND_CONTEXT_SIZE];
};
typedef struct unw_context_t unw_context_t;

struct unw_cursor_t {
  uint64_t data[_LIBUNWIND_CURSOR_SIZE];
};
typedef struct unw_cursor_t unw_cursor_t;

typedef struct unw_addr_space *unw_addr_space_t;

typedef int unw_regnum_t;
#if _LIBUNWIND_ARM_EHABI
typedef uint32_t unw_word_t;
typedef uint64_t unw_fpreg_t;
#else
typedef uint64_t unw_word_t;
typedef double unw_fpreg_t;
#endif

struct unw_proc_info_t {
  unw_word_t  start_ip;         /* start address of function */
  unw_word_t  end_ip;           /* address after end of function */
  unw_word_t  lsda;             /* address of language specific data area, */
                                /*  or zero if not used */
  unw_word_t  handler;          /* personality routine, or zero if not used */
  unw_word_t  gp;               /* not used */
  unw_word_t  flags;            /* not used */
  uint32_t    format;           /* compact unwind encoding, or zero if none */
  uint32_t    unwind_info_size; /* size of dwarf unwind info, or zero if none */
  unw_word_t  unwind_info;      /* address of dwarf unwind info, or zero */
  unw_word_t  extra;            /* mach_header of mach-o image containing func */
};
typedef struct unw_proc_info_t unw_proc_info_t;

#ifdef __cplusplus
extern "C" {
#endif

extern int unw_getcontext(unw_context_t *) LIBUNWIND_AVAIL;
extern int unw_init_local(unw_cursor_t *, unw_context_t *) LIBUNWIND_AVAIL;
extern int unw_step(unw_cursor_t *) LIBUNWIND_AVAIL;
extern int unw_get_reg(unw_cursor_t *, unw_regnum_t, unw_word_t *) LIBUNWIND_AVAIL;
extern int unw_get_fpreg(unw_cursor_t *, unw_regnum_t, unw_fpreg_t *) LIBUNWIND_AVAIL;
extern int unw_set_reg(unw_cursor_t *, unw_regnum_t, unw_word_t) LIBUNWIND_AVAIL;
extern int unw_set_fpreg(unw_cursor_t *, unw_regnum_t, unw_fpreg_t)  LIBUNWIND_AVAIL;
extern int unw_resume(unw_cursor_t *) LIBUNWIND_AVAIL;

#ifdef __arm__
/* Save VFP registers in FSTMX format (instead of FSTMD). */
extern void unw_save_vfp_as_X(unw_cursor_t *) LIBUNWIND_AVAIL;
#endif


extern const char *unw_regname(unw_cursor_t *, unw_regnum_t) LIBUNWIND_AVAIL;
extern int unw_get_proc_info(unw_cursor_t *, unw_proc_info_t *) LIBUNWIND_AVAIL;
extern int unw_is_fpreg(unw_cursor_t *, unw_regnum_t) LIBUNWIND_AVAIL;
extern int unw_is_signal_frame(unw_cursor_t *) LIBUNWIND_AVAIL;
extern int unw_get_proc_name(unw_cursor_t *, char *, size_t, unw_word_t *) LIBUNWIND_AVAIL;
//extern int       unw_get_save_loc(unw_cursor_t*, int, unw_save_loc_t*);

extern unw_addr_space_t unw_local_addr_space;

#ifdef UNW_REMOTE
/*
 * Mac OS X "remote" API for unwinding other processes on same machine
 *
 */
extern unw_addr_space_t unw_create_addr_space_for_task(task_t);
extern void unw_destroy_addr_space(unw_addr_space_t);
extern int unw_init_remote_thread(unw_cursor_t *, unw_addr_space_t, thread_t *);
#endif /* UNW_REMOTE */

/*
 * traditional libuwind "remote" API
 *   NOT IMPLEMENTED on Mac OS X
 *
 * extern int               unw_init_remote(unw_cursor_t*, unw_addr_space_t,
 *                                          thread_t*);
 * extern unw_accessors_t   unw_get_accessors(unw_addr_space_t);
 * extern unw_addr_space_t  unw_create_addr_space(unw_accessors_t, int);
 * extern void              unw_flush_cache(unw_addr_space_t, unw_word_t,
 *                                          unw_word_t);
 * extern int               unw_set_caching_policy(unw_addr_space_t,
 *                                                 unw_caching_policy_t);
 * extern void              _U_dyn_register(unw_dyn_info_t*);
 * extern void              _U_dyn_cancel(unw_dyn_info_t*);
 */

#ifdef __cplusplus
}
#endif

// architecture independent register numbers
enum {
  UNW_REG_IP = -1, // instruction pointer
  UNW_REG_SP = -2, // stack pointer
};

// 32-bit x86 registers
enum {
  UNW_X86_EAX = 0,
  UNW_X86_ECX = 1,
  UNW_X86_EDX = 2,
  UNW_X86_EBX = 3,
  UNW_X86_EBP = 4,
  UNW_X86_ESP = 5,
  UNW_X86_ESI = 6,
  UNW_X86_EDI = 7
};

// 64-bit x86_64 registers
enum {
  UNW_X86_64_RAX = 0,
  UNW_X86_64_RDX = 1,
  UNW_X86_64_RCX = 2,
  UNW_X86_64_RBX = 3,
  UNW_X86_64_RSI = 4,
  UNW_X86_64_RDI = 5,
  UNW_X86_64_RBP = 6,
  UNW_X86_64_RSP = 7,
  UNW_X86_64_R8  = 8,
  UNW_X86_64_R9  = 9,
  UNW_X86_64_R10 = 10,
  UNW_X86_64_R11 = 11,
  UNW_X86_64_R12 = 12,
  UNW_X86_64_R13 = 13,
  UNW_X86_64_R14 = 14,
  UNW_X86_64_R15 = 15
};


// 32-bit ppc register numbers
enum {
  UNW_PPC_R0  = 0,
  UNW_PPC_R1  = 1,
  UNW_PPC_R2  = 2,
  UNW_PPC_R3  = 3,
  UNW_PPC_R4  = 4,
  UNW_PPC_R5  = 5,
  UNW_PPC_R6  = 6,
  UNW_PPC_R7  = 7,
  UNW_PPC_R8  = 8,
  UNW_PPC_R9  = 9,
  UNW_PPC_R10 = 10,
  UNW_PPC_R11 = 11,
  UNW_PPC_R12 = 12,
  UNW_PPC_R13 = 13,
  UNW_PPC_R14 = 14,
  UNW_PPC_R15 = 15,
  UNW_PPC_R16 = 16,
  UNW_PPC_R17 = 17,
  UNW_PPC_R18 = 18,
  UNW_PPC_R19 = 19,
  UNW_PPC_R20 = 20,
  UNW_PPC_R21 = 21,
  UNW_PPC_R22 = 22,
  UNW_PPC_R23 = 23,
  UNW_PPC_R24 = 24,
  UNW_PPC_R25 = 25,
  UNW_PPC_R26 = 26,
  UNW_PPC_R27 = 27,
  UNW_PPC_R28 = 28,
  UNW_PPC_R29 = 29,
  UNW_PPC_R30 = 30,
  UNW_PPC_R31 = 31,
  UNW_PPC_F0  = 32,
  UNW_PPC_F1  = 33,
  UNW_PPC_F2  = 34,
  UNW_PPC_F3  = 35,
  UNW_PPC_F4  = 36,
  UNW_PPC_F5  = 37,
  UNW_PPC_F6  = 38,
  UNW_PPC_F7  = 39,
  UNW_PPC_F8  = 40,
  UNW_PPC_F9  = 41,
  UNW_PPC_F10 = 42,
  UNW_PPC_F11 = 43,
  UNW_PPC_F12 = 44,
  UNW_PPC_F13 = 45,
  UNW_PPC_F14 = 46,
  UNW_PPC_F15 = 47,
  UNW_PPC_F16 = 48,
  UNW_PPC_F17 = 49,
  UNW_PPC_F18 = 50,
  UNW_PPC_F19 = 51,
  UNW_PPC_F20 = 52,
  UNW_PPC_F21 = 53,
  UNW_PPC_F22 = 54,
  UNW_PPC_F23 = 55,
  UNW_PPC_F24 = 56,
  UNW_PPC_F25 = 57,
  UNW_PPC_F26 = 58,
  UNW_PPC_F27 = 59,
  UNW_PPC_F28 = 60,
  UNW_PPC_F29 = 61,
  UNW_PPC_F30 = 62,
  UNW_PPC_F31 = 63,
  UNW_PPC_MQ  = 64,
  UNW_PPC_LR  = 65,
  UNW_PPC_CTR = 66,
  UNW_PPC_AP  = 67,
  UNW_PPC_CR0 = 68,
  UNW_PPC_CR1 = 69,
  UNW_PPC_CR2 = 70,
  UNW_PPC_CR3 = 71,
  UNW_PPC_CR4 = 72,
  UNW_PPC_CR5 = 73,
  UNW_PPC_CR6 = 74,
  UNW_PPC_CR7 = 75,
  UNW_PPC_XER = 76,
  UNW_PPC_V0  = 77,
  UNW_PPC_V1  = 78,
  UNW_PPC_V2  = 79,
  UNW_PPC_V3  = 80,
  UNW_PPC_V4  = 81,
  UNW_PPC_V5  = 82,
  UNW_PPC_V6  = 83,
  UNW_PPC_V7  = 84,
  UNW_PPC_V8  = 85,
  UNW_PPC_V9  = 86,
  UNW_PPC_V10 = 87,
  UNW_PPC_V11 = 88,
  UNW_PPC_V12 = 89,
  UNW_PPC_V13 = 90,
  UNW_PPC_V14 = 91,
  UNW_PPC_V15 = 92,
  UNW_PPC_V16 = 93,
  UNW_PPC_V17 = 94,
  UNW_PPC_V18 = 95,
  UNW_PPC_V19 = 96,
  UNW_PPC_V20 = 97,
  UNW_PPC_V21 = 98,
  UNW_PPC_V22 = 99,
  UNW_PPC_V23 = 100,
  UNW_PPC_V24 = 101,
  UNW_PPC_V25 = 102,
  UNW_PPC_V26 = 103,
  UNW_PPC_V27 = 104,
  UNW_PPC_V28 = 105,
  UNW_PPC_V29 = 106,
  UNW_PPC_V30 = 107,
  UNW_PPC_V31 = 108,
  UNW_PPC_VRSAVE  = 109,
  UNW_PPC_VSCR    = 110,
  UNW_PPC_SPE_ACC = 111,
  UNW_PPC_SPEFSCR = 112
};

// 64-bit ARM64 registers
enum {
  UNW_ARM64_X0  = 0,
  UNW_ARM64_X1  = 1,
  UNW_ARM64_X2  = 2,
  UNW_ARM64_X3  = 3,
  UNW_ARM64_X4  = 4,
  UNW_ARM64_X5  = 5,
  UNW_ARM64_X6  = 6,
  UNW_ARM64_X7  = 7,
  UNW_ARM64_X8  = 8,
  UNW_ARM64_X9  = 9,
  UNW_ARM64_X10 = 10,
  UNW_ARM64_X11 = 11,
  UNW_ARM64_X12 = 12,
  UNW_ARM64_X13 = 13,
  UNW_ARM64_X14 = 14,
  UNW_ARM64_X15 = 15,
  UNW_ARM64_X16 = 16,
  UNW_ARM64_X17 = 17,
  UNW_ARM64_X18 = 18,
  UNW_ARM64_X19 = 19,
  UNW_ARM64_X20 = 20,
  UNW_ARM64_X21 = 21,
  UNW_ARM64_X22 = 22,
  UNW_ARM64_X23 = 23,
  UNW_ARM64_X24 = 24,
  UNW_ARM64_X25 = 25,
  UNW_ARM64_X26 = 26,
  UNW_ARM64_X27 = 27,
  UNW_ARM64_X28 = 28,
  UNW_ARM64_X29 = 29,
  UNW_ARM64_FP  = 29,
  UNW_ARM64_X30 = 30,
  UNW_ARM64_LR  = 30,
  UNW_ARM64_X31 = 31,
  UNW_ARM64_SP  = 31,
  // reserved block
  UNW_ARM64_D0  = 64,
  UNW_ARM64_D1  = 65,
  UNW_ARM64_D2  = 66,
  UNW_ARM64_D3  = 67,
  UNW_ARM64_D4  = 68,
  UNW_ARM64_D5  = 69,
  UNW_ARM64_D6  = 70,
  UNW_ARM64_D7  = 71,
  UNW_ARM64_D8  = 72,
  UNW_ARM64_D9  = 73,
  UNW_ARM64_D10 = 74,
  UNW_ARM64_D11 = 75,
  UNW_ARM64_D12 = 76,
  UNW_ARM64_D13 = 77,
  UNW_ARM64_D14 = 78,
  UNW_ARM64_D15 = 79,
  UNW_ARM64_D16 = 80,
  UNW_ARM64_D17 = 81,
  UNW_ARM64_D18 = 82,
  UNW_ARM64_D19 = 83,
  UNW_ARM64_D20 = 84,
  UNW_ARM64_D21 = 85,
  UNW_ARM64_D22 = 86,
  UNW_ARM64_D23 = 87,
  UNW_ARM64_D24 = 88,
  UNW_ARM64_D25 = 89,
  UNW_ARM64_D26 = 90,
  UNW_ARM64_D27 = 91,
  UNW_ARM64_D28 = 92,
  UNW_ARM64_D29 = 93,
  UNW_ARM64_D30 = 94,
  UNW_ARM64_D31 = 95,
};

// 32-bit ARM registers. Numbers match DWARF for ARM spec #3.1 Table 1.
// Naming scheme uses recommendations given in Note 4 for VFP-v2 and VFP-v3.
// In this scheme, even though the 64-bit floating point registers D0-D31
// overlap physically with the 32-bit floating pointer registers S0-S31,
// they are given a non-overlapping range of register numbers.
//
// Commented out ranges are not preserved during unwinding.
enum {
  UNW_ARM_R0  = 0,
  UNW_ARM_R1  = 1,
  UNW_ARM_R2  = 2,
  UNW_ARM_R3  = 3,
  UNW_ARM_R4  = 4,
  UNW_ARM_R5  = 5,
  UNW_ARM_R6  = 6,
  UNW_ARM_R7  = 7,
  UNW_ARM_R8  = 8,
  UNW_ARM_R9  = 9,
  UNW_ARM_R10 = 10,
  UNW_ARM_R11 = 11,
  UNW_ARM_R12 = 12,
  UNW_ARM_SP  = 13,  // Logical alias for UNW_REG_SP
  UNW_ARM_R13 = 13,
  UNW_ARM_LR  = 14,
  UNW_ARM_R14 = 14,
  UNW_ARM_IP  = 15,  // Logical alias for UNW_REG_IP
  UNW_ARM_R15 = 15,
  // 16-63 -- OBSOLETE. Used in VFP1 to represent both S0-S31 and D0-D31.
  UNW_ARM_S0  = 64,
  UNW_ARM_S1  = 65,
  UNW_ARM_S2  = 66,
  UNW_ARM_S3  = 67,
  UNW_ARM_S4  = 68,
  UNW_ARM_S5  = 69,
  UNW_ARM_S6  = 70,
  UNW_ARM_S7  = 71,
  UNW_ARM_S8  = 72,
  UNW_ARM_S9  = 73,
  UNW_ARM_S10 = 74,
  UNW_ARM_S11 = 75,
  UNW_ARM_S12 = 76,
  UNW_ARM_S13 = 77,
  UNW_ARM_S14 = 78,
  UNW_ARM_S15 = 79,
  UNW_ARM_S16 = 80,
  UNW_ARM_S17 = 81,
  UNW_ARM_S18 = 82,
  UNW_ARM_S19 = 83,
  UNW_ARM_S20 = 84,
  UNW_ARM_S21 = 85,
  UNW_ARM_S22 = 86,
  UNW_ARM_S23 = 87,
  UNW_ARM_S24 = 88,
  UNW_ARM_S25 = 89,
  UNW_ARM_S26 = 90,
  UNW_ARM_S27 = 91,
  UNW_ARM_S28 = 92,
  UNW_ARM_S29 = 93,
  UNW_ARM_S30 = 94,
  UNW_ARM_S31 = 95,
  //  96-103 -- OBSOLETE. F0-F7. Used by the FPA system. Superseded by VFP.
  // 104-111 -- wCGR0-wCGR7, ACC0-ACC7 (Intel wireless MMX)
  UNW_ARM_WR0 = 112,
  UNW_ARM_WR1 = 113,
  UNW_ARM_WR2 = 114,
  UNW_ARM_WR3 = 115,
  UNW_ARM_WR4 = 116,
  UNW_ARM_WR5 = 117,
  UNW_ARM_WR6 = 118,
  UNW_ARM_WR7 = 119,
  UNW_ARM_WR8 = 120,
  UNW_ARM_WR9 = 121,
  UNW_ARM_WR10 = 122,
  UNW_ARM_WR11 = 123,
  UNW_ARM_WR12 = 124,
  UNW_ARM_WR13 = 125,
  UNW_ARM_WR14 = 126,
  UNW_ARM_WR15 = 127,
  // 128-133 -- SPSR, SPSR_{FIQ|IRQ|ABT|UND|SVC}
  // 134-143 -- Reserved
  // 144-150 -- R8_USR-R14_USR
  // 151-157 -- R8_FIQ-R14_FIQ
  // 158-159 -- R13_IRQ-R14_IRQ
  // 160-161 -- R13_ABT-R14_ABT
  // 162-163 -- R13_UND-R14_UND
  // 164-165 -- R13_SVC-R14_SVC
  // 166-191 -- Reserved
  UNW_ARM_WC0 = 192,
  UNW_ARM_WC1 = 193,
  UNW_ARM_WC2 = 194,
  UNW_ARM_WC3 = 195,
  // 196-199 -- wC4-wC7 (Intel wireless MMX control)
  // 200-255 -- Reserved
  UNW_ARM_D0  = 256,
  UNW_ARM_D1  = 257,
  UNW_ARM_D2  = 258,
  UNW_ARM_D3  = 259,
  UNW_ARM_D4  = 260,
  UNW_ARM_D5  = 261,
  UNW_ARM_D6  = 262,
  UNW_ARM_D7  = 263,
  UNW_ARM_D8  = 264,
  UNW_ARM_D9  = 265,
  UNW_ARM_D10 = 266,
  UNW_ARM_D11 = 267,
  UNW_ARM_D12 = 268,
  UNW_ARM_D13 = 269,
  UNW_ARM_D14 = 270,
  UNW_ARM_D15 = 271,
  UNW_ARM_D16 = 272,
  UNW_ARM_D17 = 273,
  UNW_ARM_D18 = 274,
  UNW_ARM_D19 = 275,
  UNW_ARM_D20 = 276,
  UNW_ARM_D21 = 277,
  UNW_ARM_D22 = 278,
  UNW_ARM_D23 = 279,
  UNW_ARM_D24 = 280,
  UNW_ARM_D25 = 281,
  UNW_ARM_D26 = 282,
  UNW_ARM_D27 = 283,
  UNW_ARM_D28 = 284,
  UNW_ARM_D29 = 285,
  UNW_ARM_D30 = 286,
  UNW_ARM_D31 = 287,
  // 288-319 -- Reserved for VFP/Neon
  // 320-8191 -- Reserved
  // 8192-16383 -- Unspecified vendor co-processor register.
};

// OpenRISC1000 register numbers
enum {
  UNW_OR1K_R0  = 0,
  UNW_OR1K_R1  = 1,
  UNW_OR1K_R2  = 2,
  UNW_OR1K_R3  = 3,
  UNW_OR1K_R4  = 4,
  UNW_OR1K_R5  = 5,
  UNW_OR1K_R6  = 6,
  UNW_OR1K_R7  = 7,
  UNW_OR1K_R8  = 8,
  UNW_OR1K_R9  = 9,
  UNW_OR1K_R10 = 10,
  UNW_OR1K_R11 = 11,
  UNW_OR1K_R12 = 12,
  UNW_OR1K_R13 = 13,
  UNW_OR1K_R14 = 14,
  UNW_OR1K_R15 = 15,
  UNW_OR1K_R16 = 16,
  UNW_OR1K_R17 = 17,
  UNW_OR1K_R18 = 18,
  UNW_OR1K_R19 = 19,
  UNW_OR1K_R20 = 20,
  UNW_OR1K_R21 = 21,
  UNW_OR1K_R22 = 22,
  UNW_OR1K_R23 = 23,
  UNW_OR1K_R24 = 24,
  UNW_OR1K_R25 = 25,
  UNW_OR1K_R26 = 26,
  UNW_OR1K_R27 = 27,
  UNW_OR1K_R28 = 28,
  UNW_OR1K_R29 = 29,
  UNW_OR1K_R30 = 30,
  UNW_OR1K_R31 = 31,
};

#endif
