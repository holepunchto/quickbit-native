#include <field.h>
#include <napi-macros.h>
#include <node_api.h>

NAPI_METHOD(field_napi_get) {
  NAPI_ARGV(2);
  NAPI_ARGV_BUFFER_CAST(uint8_t *, field, 0);
  NAPI_ARGV_UINT32(bit, 1);

  NAPI_RETURN_UINT32(field_get(field, bit))
}

NAPI_METHOD(field_napi_set) {
  NAPI_ARGV(3);
  NAPI_ARGV_BUFFER_CAST(uint8_t *, field, 0);
  NAPI_ARGV_UINT32(bit, 1);
  NAPI_ARGV_UINT32(value, 2);

  NAPI_RETURN_UINT32(field_set(field, bit, value))
}

NAPI_METHOD(field_napi_index_of) {
  NAPI_ARGV(5);
  NAPI_ARGV_BUFFER_CAST(uint8_t *, field, 0);
  NAPI_ARGV_UINT32(value, 1);
  NAPI_ARGV_UINT32(position, 2);
  NAPI_ARGV_UINT32(indexed, 3);

  int32_t i;

  if (indexed) {
    NAPI_ARGV_BUFFER_CAST(uint8_t *, index, 4);

    i = field_index_of(field, field_len, value, position, index);
  } else {
    i = field_index_of(field, field_len, value, position, NULL);
  }

  NAPI_RETURN_INT32(i)
}

NAPI_METHOD(field_napi_last_index_of) {
  NAPI_ARGV(5);
  NAPI_ARGV_BUFFER_CAST(uint8_t *, field, 0);
  NAPI_ARGV_UINT32(value, 1);
  NAPI_ARGV_UINT32(position, 2);
  NAPI_ARGV_UINT32(indexed, 3);

  int32_t i;

  if (indexed) {
    NAPI_ARGV_BUFFER_CAST(uint8_t *, index, 4);

    i = field_last_index_of(field, field_len, value, position, index);
  } else {
    i = field_last_index_of(field, field_len, value, position, NULL);
  }

  NAPI_RETURN_INT32(i)
}

NAPI_METHOD(field_napi_index_init) {
  NAPI_ARGV(3);
  NAPI_ARGV_BUFFER_CAST(uint8_t *, index, 0);
  NAPI_ARGV_BUFFER_CAST(uint8_t *, field, 1);
  NAPI_ARGV_UINT32(value, 2);

  field_index_init(index, field, field_len, value);

  return NULL;
}

NAPI_INIT() {
  NAPI_EXPORT_SIZEOF(field_index_t)

  NAPI_EXPORT_FUNCTION(field_napi_get)
  NAPI_EXPORT_FUNCTION(field_napi_set)
  NAPI_EXPORT_FUNCTION(field_napi_index_of)
  NAPI_EXPORT_FUNCTION(field_napi_last_index_of)
  NAPI_EXPORT_FUNCTION(field_napi_index_init)
}
