/*
 * mips/netbsd1/md.c
 * NetBSD1 MIPS specific functions.
 *
 * Copyright (c) 1996, 1997
 *	Transvirtual Technologies, Inc.  All rights reserved.
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file.
 */

#include "config.h"
#include "config-std.h"
#include "config-mem.h"
#include "kaffe/jtypes.h"
#include "object.h"
#include "support.h"

void
mips32CallMethod (callMethodInfo *CALL)
{
    register int	iret	__asm__("$2");
    register int64	jret	__asm__("$2");
    register float	fret	__asm__("$f0");
    register double	dret	__asm__("$f0");

    register int	i0	__asm__("$4");
    register int	i1	__asm__("$5");
    register int	i2	__asm__("$6");
    register int	i3	__asm__("$7");

    register int*	sp	__asm__("sp");
    register int*	gp	__asm__("gp");
    register int*	ra	__asm__("ra");
    register int64	(*t9)(void) __asm__("$25");
    int* curr_sp = sp;
    int* curr_gp = gp;
    int* curr_ra = ra;

    {
      int nrargs = CALL->nrargs;
      uintp *argframe = __builtin_alloca((nrargs > 4 ? nrargs : 4) * sizeof(int64));
      int argidx;
      int idx = 0;

      for (argidx = 0; argidx < nrargs; ++argidx) {
	switch (CALL->callsize[argidx]) {
	case 2:
	  if ((idx & 1) != 0) {	/* Align */
	    idx += 1;
	  }
	  *(jlong*)&argframe[idx] = CALL->args[argidx].j;
	  idx += 2;
	  break;
	case 1:
	  *(jint*)&argframe[idx] = CALL->args[argidx].i;
	  idx += 1;
	  break;
	default:
	  break;
	}
      }

      /* Load the register up - alot of this is done speculatively
       * since sometime we need values in float registers, sometimes
       * in integers.
       */
      if (CALL->callsize[0] == 1) {
	asm volatile ("l.s $f12,%0" : : "m" (*(jfloat*)&argframe[0]));
	if (CALL->callsize[1] == 1) {
	  asm volatile ("l.s $f14,%0" : : "m" (*(jfloat*)&argframe[1]));
	}
	else if (CALL->callsize[1] == 2) {
	  asm volatile ("l.d $f14,%0" : : "m" (*(jdouble*)&argframe[2]));
	}
      }
      else if (CALL->callsize[0] == 2) {
	asm volatile ("l.d $f12,%0" : : "m" (*(jdouble*)&argframe[0]));
	if (CALL->callsize[2] == 1) {
	  asm volatile ("l.s $f14,%0" : : "m" (*(jfloat*)&argframe[2]));
	}
	else if (CALL->callsize[2] == 2) {
	  asm volatile ("l.d $f14,%0" : : "m" (*(jdouble*)&argframe[2]));
	}
      }
      i0 = argframe[0];
      i1 = argframe[1];
      i2 = argframe[2];
      i3 = argframe[3];

      sp = argframe;

      asm ("" : : "r" (i0), "r" (i1), "r" (i2), "r" (i3));

      (*(t9 = CALL->function))();

      asm ("lw $31,%0" : : "m" (curr_ra));
      asm ("lw $gp,%0" : : "m" (curr_gp));
      asm ("lw $sp,%0" : : "m" (curr_sp));

      if (CALL->retsize != 0) {
	switch(CALL->retsize) {
	case 2:
	  if (CALL->rettype == 'D')
	    CALL->ret->d = dret;
	  else {
	    CALL->ret->j = jret;
	  }
	  break;
	case 1:
	  if (CALL->rettype == 'F')
	    CALL->ret->f = fret;
	  else
	    CALL->ret->i = iret;
	  break;
	default:
	  ABORT();
	  break;
	}
      }
    }
}


#include "mips/mips.c"
