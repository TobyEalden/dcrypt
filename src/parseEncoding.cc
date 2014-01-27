#include "parseEncoding.h"

enum encoding node::ParseEncoding(Handle<Value> encoding_v, enum encoding _default) {
  HandleScope scope;

  if (!encoding_v->IsString()) return _default;

  String::Utf8Value encoding(encoding_v);

  if (strcmp(*encoding, "utf8") == 0) {
    return UTF8;
  } else if (strcmp(*encoding, "utf-8") == 0) {
    return UTF8;
  } else if (strcmp(*encoding, "ascii") == 0) {
    return ASCII;
  } else if (strcmp(*encoding, "base64") == 0) {
    return BASE64;
  } else if (strcmp(*encoding, "ucs2") == 0) {
    return UCS2;
  } else if (strcmp(*encoding, "ucs-2") == 0) {
    return UCS2;
  } else if (strcmp(*encoding, "utf16le") == 0) {
    return UCS2;
  } else if (strcmp(*encoding, "utf-16le") == 0) {
    return UCS2;
  } else if (strcmp(*encoding, "binary") == 0) {
    return BINARY;
  } else if (strcmp(*encoding, "hex") == 0) {
    return HEX;
  } else if (strcmp(*encoding, "raw") == 0) {
    if (!no_deprecation) {
      fprintf(stderr, "'raw' (array of integers) has been removed. "
                      "Use 'binary'.\n");
    }
    return BINARY;
  } else if (strcmp(*encoding, "raws") == 0) {
    if (!no_deprecation) {
      fprintf(stderr, "'raws' encoding has been renamed to 'binary'. "
                      "Please update your code.\n");
    }
    return BINARY;
  } else {
    return _default;
  }
}
