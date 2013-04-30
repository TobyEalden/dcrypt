#include <v8.h>
#include <node.h>
#include <node_object_wrap.h>

using namespace v8;
using namespace node;

enum encoding ParseEncoding(v8::Handle<v8::Value> encoding_v, enum encoding _default = BINARY);
