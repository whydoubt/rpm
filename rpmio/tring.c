#include "system.h"
#include "rpmio_internal.h"
#include "misc.h"
#include "popt.h"
#include "debug.h"

static int printing = 0;
static int _debug = 0;

static struct poptOption optionsTable[] = {
 { "print", 'p', POPT_ARG_VAL,	&printing, 1,		NULL, NULL },
 { "noprint", 'n', POPT_ARG_VAL, &printing, 0,		NULL, NULL },
 { "debug", 'd', POPT_ARG_VAL,	&_debug, -1,		NULL, NULL },
  POPT_AUTOHELP
  POPT_TABLEEND
};

int
main (int argc, const char *argv[])
{
    poptContext optCon = poptGetContext(argv[0], argc, argv, optionsTable, 0);
    rpmDigest dig;
    const byte * pkt = NULL;
    ssize_t pktlen;
    const char ** args;
    const char * fn;
    int rc = 0;

    while ((rc = poptGetNextOpt(optCon)) > 0)
	;

    if ((args = poptGetArgs(optCon)) != NULL)
    while ((fn = *args++) != NULL) {
	rc = pgpReadPkts(fn, &pkt, &pktlen);
	if (rc || pkt == NULL || pktlen <= 0)
	    continue;

fprintf(stderr, "===================== %s\n", fn);
	dig = xcalloc(1, sizeof(*dig));
	(void) pgpPrtPkts(pkt, pktlen, dig, printing);
	free((void *)pkt);
	pkt = NULL;
	free((void *)dig);
	dig = NULL;
    }

    return rc;
}
