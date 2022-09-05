#include <napi-macros.h>
#include <node_api.h>
#include <quickbit.h>

NAPI_METHOD(quickbit_napi_get) {
  NAPI_ARGV(2);
  NAPI_ARGV_BUFFER_CAST(uint8_t *, field, 0);
  NAPI_ARGV_UINT32(bit, 1);

  NAPI_RETURN_UINT32(quickbit_get(field, bit))
}

NAPI_METHOD(quickbit_napi_set) {
  NAPI_ARGV(3);
  NAPI_ARGV_BUFFER_CAST(uint8_t *, field, 0);
  NAPI_ARGV_UINT32(bit, 1);
  NAPI_ARGV_UINT32(value, 2);

  NAPI_RETURN_UINT32(quickbit_set(field, bit, value))
}

NAPI_METHOD(quickbit_napi_index_of) {
  NAPI_ARGV(5);
  NAPI_ARGV_BUFFER_CAST(uint8_t *, field, 0);
  NAPI_ARGV_UINT32(value, 1);
  NAPI_ARGV_UINT32(position, 2);
  NAPI_ARGV_UINT32(indexed, 3);

  int32_t i;

  if (indexed) {
    NAPI_ARGV_BUFFER_CAST(uint8_t *, index, 4);

    i = quickbit_index_of(field, field_len, value, position, index);
  } else {
    i = quickbit_index_of(field, field_len, value, position, NULL);
  }

  NAPI_RETURN_INT32(i)
}

NAPI_METHOD(quickbit_napi_last_index_of) {
  NAPI_ARGV(5);
  NAPI_ARGV_BUFFER_CAST(uint8_t *, field, 0);
  NAPI_ARGV_UINT32(value, 1);
  NAPI_ARGV_UINT32(position, 2);
  NAPI_ARGV_UINT32(indexed, 3);

  int32_t i;

  if (indexed) {
    NAPI_ARGV_BUFFER_CAST(uint8_t *, index, 4);

    i = quickbit_last_index_of(field, field_len, value, position, index);
  } else {
    i = quickbit_last_index_of(field, field_len, value, position, NULL);
  }

  NAPI_RETURN_INT32(i)
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
  NAPI_ARGV_UINT32(bit, 2);

  NAPI_RETURN_UINT32(quickbit_index_update(index, field, bit))
}

NAPI_INIT() {
  NAPI_EXPORT_SIZEOF(quickbit_index_t)

  NAPI_EXPORT_FUNCTION(quickbit_napi_get)
  NAPI_EXPORT_FUNCTION(quickbit_napi_set)
  NAPI_EXPORT_FUNCTION(quickbit_napi_index_of)
  NAPI_EXPORT_FUNCTION(quickbit_napi_last_index_of)
  NAPI_EXPORT_FUNCTION(quickbit_napi_index_init)
  NAPI_EXPORT_FUNCTION(quickbit_napi_index_update)
}
