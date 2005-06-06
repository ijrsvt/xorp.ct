/* -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*- */

/*
 * Copyright (c) 2001-2005 International Computer Science Institute
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software")
 * to deal in the Software without restriction, subject to the conditions
 * listed in the XORP LICENSE file. These conditions include: you must
 * preserve this copyright notice, and you cannot mention the copyright
 * holders in advertising related to the Software without their permission.
 * The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 * notice is a summary of the XORP LICENSE file; the license in that file is
 * legally binding.
 */

#ident "$XORP: xorp/mrt/random.c,v 1.2 2005/06/01 23:45:45 pavlin Exp $"


/*
 * Random-number generator implementation.
 */
/*
 * XXX: the particular implementation is taken from NetBSD-current
 * (as of 2005/06/01).
 * The rest is a front-end for it.
 */


#include "libxorp/xorp.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>

#include "mrt_module.h"
#include "libxorp/xorp.h"
#include "random.h"


/*
 * Exported variables
 */

/*
 * Local constants definitions
 */

/*
 * Local structures, typedefs and macros
 */
#if defined(HAVE_RANDOM) && !defined(HOST_OS_SOLARIS)
#define LOCAL_SRANDOM(seed)		srandom(seed)
#define LOCAL_RANDOM()			random()
#define LOCAL_INITSTATE(seed, state, n)	initstate(seed, state, n)
#define LOCAL_SETSTATE(arg_state)	setstate(arg_state)
#else
#define LOCAL_SRANDOM(seed)		srandom_from_netbsd(seed)
#define LOCAL_RANDOM()			random_from_netbsd()
#define LOCAL_INITSTATE(seed, state, n)	initstate_from_netbsd(seed, state, n)
#define LOCAL_SETSTATE(arg_state)	setstate_from_netbsd(arg_state)
#endif /* ! (HAVE_RANDOM && !HOST_OS_SOLARIS) */

/*
 * Local variables
 */
static bool_t srandom_called = false;

/*
 * Local functions prototypes
 */
#if !defined(HAVE_RANDOM) || defined(HOST_OS_SOLARIS)
static void	srandom_from_netbsd(unsigned long x);
static long	random_from_netbsd(void);
static char *	initstate_from_netbsd(unsigned long seed, char *arg_state,
				      size_t n);
static char *	setstate_from_netbsd(char *arg_state);
#endif /* ! (HAVE_RANDOM && !HOST_OS_SOLARIS) */


/**
 * my_srandom:
 * @seed: The random generator seed. If '0', then the seed will use the
 * sum of the current seconds and milliseconds.
 * 
 * Init the random generator.
 **/
void
my_srandom(unsigned long seed)
{
    struct timeval curtime;
    
    if (seed == 0) {
	gettimeofday(&curtime, NULL);
	seed = curtime.tv_sec + curtime.tv_usec;
    }
    
    LOCAL_SRANDOM(seed);
    srandom_called = true;
}

/**
 * my_random:
 * @max_value: The upper bound (excluding) for the returned random value.
 * 
 * Generate a random number in the interval [0, @max_value - 1].
 * 
 * Return value: A random number in the interval [0, @max_value - 1].
 **/
unsigned long
my_random(unsigned long max_value)
{
    unsigned long result;
    
    if (max_value == 0)
	return (0);
    
    if (!srandom_called)
	SRANDOM(0);
    
    result = LOCAL_RANDOM() % max_value;
    
    return (result);
}

/*
 * A wrap-up for initstate(3). If the system does not implement initstate(3),
 * it will use a locally defined implementation (copied from NetBSD).
 */
char *
my_initstate(unsigned long seed, char *state, size_t n)
{
    return (LOCAL_INITSTATE(seed, state, n));
}

/*
 * A wrap-up for setstate(3). If the system does not implement initstate(3),
 * it will use a locally defined implementation (copied from NetBSD).
 */
char *
my_setstate(char *state)
{
    return (LOCAL_SETSTATE(state));
}

#if !defined(HAVE_RANDOM) || defined(HOST_OS_SOLARIS)
/*
 * The system does not seem to have a good random generator, hence
 * we use our own functions.
 */
/*
 * Everything below is taken from NetBSD
 * /usr/src/lib/libc/stdlib/random.c
 *
 * The changes are:
 *	- The global function names have suffix '_from_netbsd', and are
 *	  redefined as static.
 *	- The inclusion of some header files is removed.
 *	- The function parameters are ANSI.
 *	- The mutex locking is removed (because it may not be available
 *	  on all systems, and we don't lock to replicate the mutex code
 *	  as well).
 *	- Use only the better random generator that is #ifdef USE_BETTER_RANDOM
 *	- Some unused stuff is deleted.
 *	- Minor cleanup.
 */

/*	$NetBSD: random.c,v 1.23 2003/11/26 20:44:40 jdolecek Exp $	*/

/*
 * Copyright (c) 1983, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "config.h"

#ifdef HAVE_SYS_CDEFS_H
#include <sys/cdefs.h>
#endif
#if defined(LIBC_SCCS) && !defined(lint)
#if 0
static char sccsid[] = "@(#)random.c	8.2 (Berkeley) 5/19/95";
#else
__RCSID("$NetBSD: random.c,v 1.23 2003/11/26 20:44:40 jdolecek Exp $");
#endif
#endif /* LIBC_SCCS and not lint */

#include <assert.h>
#include <errno.h>
#include <stdlib.h>

static void srandom_unlocked(unsigned int);
static long random_unlocked(void);

/*
 * random.c:
 *
 * An improved random number generation package.  In addition to the standard
 * rand()/srand() like interface, this package also has a special state info
 * interface.  The initstate() routine is called with a seed, an array of
 * bytes, and a count of how many bytes are being passed in; this array is
 * then initialized to contain information for random number generation with
 * that much state information.  Good sizes for the amount of state
 * information are 32, 64, 128, and 256 bytes.  The state can be switched by
 * calling the setstate() routine with the same array as was initiallized
 * with initstate().  By default, the package runs with 128 bytes of state
 * information and generates far better random numbers than a linear
 * congruential generator.  If the amount of state information is less than
 * 32 bytes, a simple linear congruential R.N.G. is used.
 *
 * Internally, the state information is treated as an array of ints; the
 * zeroeth element of the array is the type of R.N.G. being used (small
 * integer); the remainder of the array is the state information for the
 * R.N.G.  Thus, 32 bytes of state information will give 7 ints worth of
 * state information, which will allow a degree seven polynomial.  (Note:
 * the zeroeth word of state information also has some other information
 * stored in it -- see setstate() for details).
 * 
 * The random number generation technique is a linear feedback shift register
 * approach, employing trinomials (since there are fewer terms to sum up that
 * way).  In this approach, the least significant bit of all the numbers in
 * the state table will act as a linear feedback shift register, and will
 * have period 2^deg - 1 (where deg is the degree of the polynomial being
 * used, assuming that the polynomial is irreducible and primitive).  The
 * higher order bits will have longer periods, since their values are also
 * influenced by pseudo-random carries out of the lower bits.  The total
 * period of the generator is approximately deg*(2**deg - 1); thus doubling
 * the amount of state information has a vast influence on the period of the
 * generator.  Note: the deg*(2**deg - 1) is an approximation only good for
 * large deg, when the period of the shift register is the dominant factor.
 * With deg equal to seven, the period is actually much longer than the
 * 7*(2**7 - 1) predicted by this formula.
 *
 * Modified 28 December 1994 by Jacob S. Rosenberg.
 * The following changes have been made:
 * All references to the type u_int have been changed to unsigned long.
 * All references to type int have been changed to type long.  Other
 * cleanups have been made as well.  A warning for both initstate and
 * setstate has been inserted to the effect that on Sparc platforms
 * the 'arg_state' variable must be forced to begin on word boundaries.
 * This can be easily done by casting a long integer array to char *.
 * The overall logic has been left STRICTLY alone.  This software was
 * tested on both a VAX and Sun SpacsStation with exactly the same
 * results.  The new version and the original give IDENTICAL results.
 * The new version is somewhat faster than the original.  As the
 * documentation says:  "By default, the package runs with 128 bytes of
 * state information and generates far better random numbers than a linear
 * congruential generator.  If the amount of state information is less than
 * 32 bytes, a simple linear congruential R.N.G. is used."  For a buffer of
 * 128 bytes, this new version runs about 19 percent faster and for a 16
 * byte buffer it is about 5 percent faster.
 *
 * Modified 07 January 2002 by Jason R. Thorpe.
 * The following changes have been made:
 * All the references to "long" have been changed back to "int".  This
 * fixes memory corruption problems on LP64 platforms.
 */

/*
 * For each of the currently supported random number generators, we have a
 * break value on the amount of state information (you need at least this
 * many bytes of state info to support this random number generator), a degree
 * for the polynomial (actually a trinomial) that the R.N.G. is based on, and
 * the separation between the two lower order coefficients of the trinomial.
 */
#define	TYPE_0		0		/* linear congruential */
#define	BREAK_0		8
#define	DEG_0		0
#define	SEP_0		0

#define	TYPE_1		1		/* x**7 + x**3 + 1 */
#define	BREAK_1		32
#define	DEG_1		7
#define	SEP_1		3

#define	TYPE_2		2		/* x**15 + x + 1 */
#define	BREAK_2		64
#define	DEG_2		15
#define	SEP_2		1

#define	TYPE_3		3		/* x**31 + x**3 + 1 */
#define	BREAK_3		128
#define	DEG_3		31
#define	SEP_3		3

#define	TYPE_4		4		/* x**63 + x + 1 */
#define	BREAK_4		256
#define	DEG_4		63
#define	SEP_4		1

/*
 * Array versions of the above information to make code run faster --
 * relies on fact that TYPE_i == i.
 */
#define	MAX_TYPES	5		/* max number of types above */

static const int degrees[MAX_TYPES] =	{ DEG_0, DEG_1, DEG_2, DEG_3, DEG_4 };
static const int seps[MAX_TYPES] =	{ SEP_0, SEP_1, SEP_2, SEP_3, SEP_4 };

/*
 * Initially, everything is set up as if from:
 *
 *	initstate(1, &randtbl, 128);
 *
 * Note that this initialization takes advantage of the fact that srandom()
 * advances the front and rear pointers 10*rand_deg times, and hence the
 * rear pointer which starts at 0 will also end up at zero; thus the zeroeth
 * element of the state information, which contains info about the current
 * position of the rear pointer is just
 *
 *	MAX_TYPES * (rptr - state) + TYPE_3 == TYPE_3.
 */

/* LINTED */
static int randtbl[DEG_3 + 1] = {
	TYPE_3,
	0x991539b1, 0x16a5bce3, 0x6774a4cd,
	0x3e01511e, 0x4e508aaa, 0x61048c05,
	0xf5500617, 0x846b7115, 0x6a19892c,
	0x896a97af, 0xdb48f936, 0x14898454,
	0x37ffd106, 0xb58bff9c, 0x59e17104,
	0xcf918a49, 0x09378c83, 0x52c7a471,
	0x8d293ea9, 0x1f4fc301, 0xc3db71be,
	0x39b44e1c, 0xf8a44ef9, 0x4c8b80b1,
	0x19edc328, 0x87bf4bdd, 0xc9b240e5,
	0xe9ee4b1b, 0x4382aee7, 0x535b6b41,
	0xf3bec5da,
};

/*
 * fptr and rptr are two pointers into the state info, a front and a rear
 * pointer.  These two pointers are always rand_sep places aparts, as they
 * cycle cyclically through the state information.  (Yes, this does mean we
 * could get away with just one pointer, but the code for random() is more
 * efficient this way).  The pointers are left positioned as they would be
 * from the call
 *
 *	initstate(1, randtbl, 128);
 *
 * (The position of the rear pointer, rptr, is really 0 (as explained above
 * in the initialization of randtbl) because the state table pointer is set
 * to point to randtbl[1] (as explained below).
 */
static int *fptr = &randtbl[SEP_3 + 1];
static int *rptr = &randtbl[1];

/*
 * The following things are the pointer to the state information table, the
 * type of the current generator, the degree of the current polynomial being
 * used, and the separation between the two pointers.  Note that for efficiency
 * of random(), we remember the first location of the state information, not
 * the zeroeth.  Hence it is valid to access state[-1], which is used to
 * store the type of the R.N.G.  Also, we remember the last location, since
 * this is more efficient than indexing every time to find the address of
 * the last element to see if the front and rear pointers have wrapped.
 */
static int *state = &randtbl[1];
static int rand_type = TYPE_3;
static int rand_deg = DEG_3;
static int rand_sep = SEP_3;
static int *end_ptr = &randtbl[DEG_3 + 1];

/*
 * srandom:
 *
 * Initialize the random number generator based on the given seed.  If the
 * type is the trivial no-state-information type, just remember the seed.
 * Otherwise, initializes state[] based on the given "seed" via a linear
 * congruential generator.  Then, the pointers are set to known locations
 * that are exactly rand_sep places apart.  Lastly, it cycles the state
 * information a given number of times to get rid of any initial dependencies
 * introduced by the L.C.R.N.G.  Note that the initialization of randtbl[]
 * for default usage relies on values produced by this routine.
 */
static void
srandom_unlocked(unsigned int x)
{
	int i;

	if (rand_type == TYPE_0)
		state[0] = x;
	else {
		state[0] = x;
		for (i = 1; i < rand_deg; i++) {
			int x1, hi, lo, t;

			/*
			 * Compute x[n + 1] = (7^5 * x[n]) mod (2^31 - 1).
			 * From "Random number generators: good ones are hard
			 * to find", Park and Miller, Communications of the ACM,
			 * vol. 31, no. 10,
			 * October 1988, p. 1195.
			 */
			x1 = state[i - 1];
			hi = x1 / 127773;
			lo = x1 % 127773;
			t = 16807 * lo - 2836 * hi;
			if (t <= 0)
				t += 0x7fffffff;
			state[i] = t;
		}
		fptr = &state[rand_sep];
		rptr = &state[0];
		for (i = 0; i < 10 * rand_deg; i++)
			(void)random_unlocked();
	}
}

static void
srandom_from_netbsd(unsigned long x)
{
	srandom_unlocked((unsigned int) x);
}

/*
 * initstate:
 *
 * Initialize the state information in the given array of n bytes for future
 * random number generation.  Based on the number of bytes we are given, and
 * the break values for the different R.N.G.'s, we choose the best (largest)
 * one we can and set things up for it.  srandom() is then called to
 * initialize the state information.
 * 
 * Note that on return from srandom(), we set state[-1] to be the type
 * multiplexed with the current value of the rear pointer; this is so
 * successive calls to initstate() won't lose this information and will be
 * able to restart with setstate().
 * 
 * Note: the first thing we do is save the current state, if any, just like
 * setstate() so that it doesn't matter when initstate is called.
 *
 * Returns a pointer to the old state.
 *
 * Note: The Sparc platform requires that arg_state begin on an int
 * word boundary; otherwise a bus error will occur. Even so, lint will
 * complain about mis-alignment, but you should disregard these messages.
 */
static char *
initstate_from_netbsd(
	unsigned long seed,		/* seed for R.N.G. */
	char *arg_state,		/* pointer to state array */
	size_t n)			/* # bytes of state info */
{
	void *ostate = (void *)(&state[-1]);
	int *int_arg_state;

	int_arg_state = (int *)(void *)arg_state;

	if (rand_type == TYPE_0)
		state[-1] = rand_type;
	else
		state[-1] = MAX_TYPES * (int)(rptr - state) + rand_type;
	if (n < BREAK_0) {
		return (NULL);
	} else if (n < BREAK_1) {
		rand_type = TYPE_0;
		rand_deg = DEG_0;
		rand_sep = SEP_0;
	} else if (n < BREAK_2) {
		rand_type = TYPE_1;
		rand_deg = DEG_1;
		rand_sep = SEP_1;
	} else if (n < BREAK_3) {
		rand_type = TYPE_2;
		rand_deg = DEG_2;
		rand_sep = SEP_2;
	} else if (n < BREAK_4) {
		rand_type = TYPE_3;
		rand_deg = DEG_3;
		rand_sep = SEP_3;
	} else {
		rand_type = TYPE_4;
		rand_deg = DEG_4;
		rand_sep = SEP_4;
	}
	state = (int *) (int_arg_state + 1); /* first location */
	end_ptr = &state[rand_deg];	/* must set end_ptr before srandom */
	srandom_unlocked((unsigned int) seed);
	if (rand_type == TYPE_0)
		int_arg_state[0] = rand_type;
	else
		int_arg_state[0] = MAX_TYPES * (int)(rptr - state) + rand_type;
	return((char *)ostate);
}

/*
 * setstate:
 *
 * Restore the state from the given state array.
 *
 * Note: it is important that we also remember the locations of the pointers
 * in the current state information, and restore the locations of the pointers
 * from the old state information.  This is done by multiplexing the pointer
 * location into the zeroeth word of the state information.
 *
 * Note that due to the order in which things are done, it is OK to call
 * setstate() with the same state as the current state.
 *
 * Returns a pointer to the old state information.
 *
 * Note: The Sparc platform requires that arg_state begin on a long
 * word boundary; otherwise a bus error will occur. Even so, lint will
 * complain about mis-alignment, but you should disregard these messages.
 */
static char *
setstate_from_netbsd(
	char *arg_state)		/* pointer to state array */
{
	int *new_state;
	int type;
	int rear;
	void *ostate = (void *)(&state[-1]);

	new_state = (int *)(void *)arg_state;
	type = (int)(new_state[0] % MAX_TYPES);
	rear = (int)(new_state[0] / MAX_TYPES);

	if (rand_type == TYPE_0)
		state[-1] = rand_type;
	else
		state[-1] = MAX_TYPES * (int)(rptr - state) + rand_type;
	switch(type) {
	case TYPE_0:
	case TYPE_1:
	case TYPE_2:
	case TYPE_3:
	case TYPE_4:
		rand_type = type;
		rand_deg = degrees[type];
		rand_sep = seps[type];
		break;
	default:
		return (NULL);
	}
	state = (int *) (new_state + 1);
	if (rand_type != TYPE_0) {
		rptr = &state[rear];
		fptr = &state[(rear + rand_sep) % rand_deg];
	}
	end_ptr = &state[rand_deg];		/* set end_ptr too */
	return((char *)ostate);
}

/*
 * random:
 *
 * If we are using the trivial TYPE_0 R.N.G., just do the old linear
 * congruential bit.  Otherwise, we do our fancy trinomial stuff, which is
 * the same in all the other cases due to all the global variables that have
 * been set up.  The basic operation is to add the number at the rear pointer
 * into the one at the front pointer.  Then both pointers are advanced to
 * the next location cyclically in the table.  The value returned is the sum
 * generated, reduced to 31 bits by throwing away the "least random" low bit.
 *
 * Note: the code takes advantage of the fact that both the front and
 * rear pointers can't wrap on the same call by not testing the rear
 * pointer if the front one has wrapped.
 *
 * Returns a 31-bit random number.
 */
static long
random_unlocked()
{
	int i;
	int *f, *r;

	if (rand_type == TYPE_0) {
		i = state[0];
		state[0] = i = (i * 1103515245 + 12345) & 0x7fffffff;
	} else {
		/*
		 * Use local variables rather than static variables for speed.
		 */
		f = fptr; r = rptr;
		*f += *r;
		/* chucking least random bit */
		i = ((unsigned int)*f >> 1) & 0x7fffffff;
		if (++f >= end_ptr) {
			f = state;
			++r;
		}
		else if (++r >= end_ptr) {
			r = state;
		}

		fptr = f; rptr = r;
	}
	return(i);
}

static long
random_from_netbsd()
{
	long r;

	r = random_unlocked();
	return (r);
}

#endif /* ! (HAVE_RANDOM && !HOST_OS_SOLARIS) */
