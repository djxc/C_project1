
/* geos_hello_world.c */

#include <stdio.h>  /* for printf */
#include <stdarg.h> /* for va_list */

/* Only the CAPI header is required */
#include <geos_c.h>

#include "geos_demo.h"

/*
* GEOS requires two message handlers to return
* error and notice message to the calling program.
*
*   typedef void(* GEOSMessageHandler) (const char *fmt,...)
*
* Here we stub out an example that just prints the
* messages to stdout.
*/
static void
geos_msg_handler(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vprintf (fmt, ap);
    va_end(ap);
}

int demo_geos()
{
    /* Send notice and error messages to the terminal */
    initGEOS(geos_msg_handler, geos_msg_handler);

    /* Read WKT into geometry object */
    GEOSWKTReader* reader = GEOSWKTReader_create();
    GEOSGeometry* geom_a = GEOSWKTReader_read(reader, "POINT(1 1)");

    /* Convert result to WKT */
    GEOSWKTWriter* writer = GEOSWKTWriter_create();
    char* wkt = GEOSWKTWriter_write(writer, geom_a);
    printf("Geometry: %s\n", wkt);
  
    GEOSGeometry *geom1_, *geom2_;
    geom1_ = GEOSGeomFromWKT("LINESTRING(0 0, 10 10, 10 0, 0 10)");
    geom2_ = GEOSNode(geom1_);
    // ensure(nullptr != geom2_);

    GEOSNormalize(geom2_);
    char* wkt_ = GEOSWKTWriter_write(writer, geom2_);
    printf("Geometry: %s\n", wkt_);
    // ensure_equals(out,
    //     "MULTILINESTRING ((5 5, 10 10, 10 0, 5 5), (0 10, 5 5), (0 0, 5 5))"
    //     );

    /* Clean up allocated objects */
    GEOSWKTReader_destroy(reader);
    GEOSWKTWriter_destroy(writer);
    GEOSGeom_destroy(geom_a);
    GEOSFree(wkt);
    /* Clean up the global context */
    finishGEOS();
    return 0;
}