/*	$OpenBSD: malloc_ulimit1.c,v 1.4 2018/11/28 13:34:06 bluhm Exp $	*/

/* Public Domain, 2006, Otto Moerbeek <otto@drijf.net> */

#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <err.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * This code tries to trigger the case present in -current as of April
 * 2006) where the allocation of the region itself succeeds, but the
 * page dir entry pages fails.
 * This in turn trips a "hole in directories" error.
 * Having a large (512M) ulimit -m helps a lot in triggering the
 * problem. Note that you may need to run this test multiple times to
 * see the error.
*/

#define STARTI	1300
#define FACTOR	1024

int
main()
{
	struct rlimit lim;
	size_t sz;
	int i;
	void *p;
	extern char *malloc_options;

	/* This test takes forever with junking turned on. */
	malloc_options = "jj";

	if (getrlimit(RLIMIT_DATA, &lim) == -1)
		err(1, "getrlimit");

	sz = lim.rlim_cur / FACTOR;

	for (i = STARTI; i >= 0; i--) {
		size_t len = (sz-i) * FACTOR;
		p = malloc(len);
		free(p);
		free(malloc(4096));
	}
	return (0);
}
