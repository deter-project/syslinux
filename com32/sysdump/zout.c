/*
 * Compress input and feed it to a block-oriented back end.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <zlib.h>
#include "backend.h"

#define ALLOC_CHUNK	65536

int init_data(struct backend *be, const char *argv[])
{
    be->argv = argv;

    be->alloc  = ALLOC_CHUNK;
    be->outbuf = malloc(ALLOC_CHUNK);
    if (!be->outbuf)
	return -1;

    memset(&be->zstream, 0, sizeof be->zstream);

    be->zstream.next_out  = (void *)be->outbuf;
    be->zstream.avail_out = ALLOC_CHUNK;

    be->dbytes = be->zbytes = 0;

    /* Initialize a gzip data stream */
    if (deflateInit2(&be->zstream, 9, Z_DEFLATED,
		     16+15, 9, Z_DEFAULT_STRATEGY) < 0)
	return -1;

    return 0;
}

static int do_deflate(struct backend *be, int flush)
{
    int rv;

    rv = deflate(&be->zstream, flush);
    be->zbytes = be->alloc - be->zstream.avail_out;
    if (be->zstream.avail_out == 0) {
	char *buf;
	buf = realloc(be->outbuf, be->alloc + ALLOC_CHUNK);
	if (!buf)
	    return Z_MEM_ERROR;
	be->outbuf = buf;
	be->zstream.next_out = (void *)(buf + be->zbytes);
	be->zstream.avail_out = be->alloc - be->zbytes;
    }

    return rv;
}


int write_data(struct backend *be, const void *buf, size_t len)
{
    int rv = Z_OK;

    be->zstream.next_in = (void *)buf;
    be->zstream.avail_in = len;

    be->dbytes += len;

    while (be->zstream.avail_in) {
	rv = do_deflate(be, Z_NO_FLUSH);
	if (rv < 0)
	    return -1;
    }
    return 0;
}

/* Output the data and shut down the stream */
int flush_data(struct backend *be)
{
    int rv = Z_OK;

    while (rv == Z_OK) {
	rv = do_deflate(be, Z_FINISH);
	if (rv < 0)
	    return -1;
    }

    if (be->write(be))
	return -1;

    free(be->outbuf);
    be->outbuf = NULL;
    be->dbytes = be->zbytes = be->alloc = 0;
    return 0;
}
