/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.9.3 at Thu Jul 25 21:38:42 2019. */

#include "EventBase.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t EventBase_fields[8] = {
    PB_FIELD(  1, STRING  , SINGULAR, CALLBACK, FIRST, EventBase, id, id, 0),
    PB_FIELD(  2, STRING  , SINGULAR, CALLBACK, OTHER, EventBase, correlationId, id, 0),
    PB_FIELD(  3, INT32   , SINGULAR, STATIC  , OTHER, EventBase, deviceTime, correlationId, 0),
    PB_FIELD(  4, STRING  , SINGULAR, CALLBACK, OTHER, EventBase, deviceId, deviceTime, 0),
    PB_FIELD(  5, INT32   , SINGULAR, STATIC  , OTHER, EventBase, version, deviceId, 0),
    PB_FIELD(  6, STRING  , SINGULAR, CALLBACK, OTHER, EventBase, commandId, version, 0),
    PB_FIELD(  7, BYTES   , SINGULAR, CALLBACK, OTHER, EventBase, payload, commandId, 0),
    PB_LAST_FIELD
};


/* @@protoc_insertion_point(eof) */