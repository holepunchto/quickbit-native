#include <napi-macros.h>
#include <node_api.h>
#include <quickbit.h>

NAPI_METHOD(quickbit_napi_get) {
  NAPI_ARGV(2);
  NAPI_ARGV_BUFFER_CAST(uint8_t *, field, 0);
  NAPI_ARGV_INT32(bit, 1);

  NAPI_RETURN_UINT32(quickbit_get(field, field_len, bit))
}

NAPI_METHOD(quickbit_napi_set) {
  NAPI_ARGV(3);
  NAPI_ARGV_BUFFER_CAST(uint8_t *, field, 0);
  NAPI_ARGV_INT32(bit, 1);
  NAPI_ARGV_UINT32(value, 2);

  NAPI_RETURN_UINT32(quickbit_set(field, field_len, bit, value))
}

NAPI_METHOD(quickbit_napi_fill) {
  NAPI_ARGV(4);
  NAPI_ARGV_BUFFER_CAST(uint8_t *, field, 0);
  NAPI_ARGV_UINT32(value, 1);
  NAPI_ARGV_INT32(start, 2);
  NAPI_ARGV_INT32(end, 3);

  quickbit_fill(field, field_len, value, start, end);

  return NULL;
}

NAPI_METHOD(quickbit_napi_find_first) {
  NAPI_ARGV(3);
  NAPI_ARGV_BUFFER_CAST(uint8_t *, field, 0);
  NAPI_ARGV_UINT32(value, 1);
  NAPI_ARGV_INT32(position, 2);

  NAPI_RETURN_INT32(quickbit_find_first(field, field_len, value, position))
}

NAPI_METHOD(quickbit_napi_find_last) {
  NAPI_ARGV(3);
  NAPI_ARGV_BUFFER_CAST(uint8_t *, field, 0);
  NAPI_ARGV_UINT32(value, 1);
  NAPI_ARGV_INT32(position, 2);

  NAPI_RETURN_INT32(quickbit_find_last(field, field_len, value, position))
}

NAPI_METHOD(quickbit_napi_index_init) {
  NAPI_ARGV(2);
  NAPI_ARGV_BUFFER_CAST(uint8_t *, index, 0);
  NAPI_ARGV_BUFFER_CAST(uint8_t *, field, 1);

  quickbit_index_init(index, field, field_len);

  return NULL;
}

NAPI_METHOD(quickbit_napi_index_update) {
  NAPI_ARGV(3);
  NAPI_ARGV_BUFFER_CAST(uint8_t *, index, 0);
  NAPI_ARGV_BUFFER_CAST(uint8_t *, field, 1);
  NAPI_ARGV_INT32(bit, 2);

  NAPI_RETURN_UINT32(quickbit_index_update(index, field, field_len, bit))
}

NAPI_METHOD(quickbit_napi_skip_first) {
  NAPI_ARGV(4);
  NAPI_ARGV_BUFFER_CAST(uint8_t *, index, 0);
  NAPI_ARGV_UINT32(len, 1);
  NAPI_ARGV_UINT32(value, 2);
  NAPI_ARGV_INT32(position, 3);

  NAPI_RETURN_UINT32(quickbit_skip_first(index, len, value, position))
}

NAPI_METHOD(quickbit_napi_skip_last) {
  NAPI_ARGV(4);
  NAPI_ARGV_BUFFER_CAST(uint8_t *, index, 0);
  NAPI_ARGV_UINT32(len, 1);
  NAPI_ARGV_UINT32(value, 2);
  NAPI_ARGV_INT32(position, 3);

  NAPI_RETURN_UINT32(quickbit_skip_last(index, len, value, position))
}

NAPI_INIT() {
  NAPI_EXPORT_SIZEOF(quickbit_index_t)

  NAPI_EXPORT_FUNCTION(quickbit_napi_get)
  NAPI_EXPORT_FUNCTION(quickbit_napi_set)
  NAPI_EXPORT_FUNCTION(quickbit_napi_fill)
  NAPI_EXPORT_FUNCTION(quickbit_napi_find_first)
  NAPI_EXPORT_FUNCTION(quickbit_napi_find_last)
  NAPI_EXPORT_FUNCTION(quickbit_napi_index_init)
  NAPI_EXPORT_FUNCTION(quickbit_napi_index_update)
  NAPI_EXPORT_FUNCTION(quickbit_napi_skip_first)
  NAPI_EXPORT_FUNCTION(quickbit_napi_skip_last)
}
