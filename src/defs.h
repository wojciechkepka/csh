#ifndef CSH_DEFSH
#define CSH_DEFSH

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


#ifndef _ERR
#define _ERR(msg) fprintf (stderr, (msg  "\n"));
#endif // _ERR

#ifndef _ERRF
#define _ERRF(fmt, ...) fprintf (stderr, (fmt  "\n"), __VA_ARGS__);
#endif // _ERRF


#ifndef _ERRNO
#define _ERRNO(msg) fprintf (stderr, (msg  ": %s\n"), strerror(errno));
#endif // _ERRNO

#ifndef _ERRNOF
#define _ERRNOF(fmt, ...) fprintf (stderr, (fmt  ": %s\n"), __VA_ARGS__, strerror(errno));
#endif // _ERRNOF


#endif // CSH_DEFSH
