#include <bare.h>
#include <js.h>
#include <jstl.h>
#include <quickbit.h>

struct js_anybuffer_t {
  uint8_t *data;
  size_t len;
};

template <>
struct js_type_info_t<js_anybuffer_t> {
  using type = js_value_t *;

  static auto
  signature() {
    return js_object;
  }

  static inline const std::array<uint8_t, 12> stride = [] {
    std::array<uint8_t, 12> a{};
    a[js_int8array] = 1;
    a[js_uint8array] = 1;
    a[js_uint8clampedarray] = 1;
    a[js_int16array] = 2;
    a[js_uint16array] = 2;
    a[js_int32array] = 4;
    a[js_uint32array] = 4;
    a[js_float16array] = 2;
    a[js_float32array] = 4;
    a[js_float64array] = 8;
    a[js_bigint64array] = 8;
    a[js_biguint64array] = 8;
    return a;
  }();

  static auto
  unmarshall(js_env_t *env, js_value_t *value, js_anybuffer_t &buffer) {
    int err;
    size_t n_elements;
    js_typedarray_type_t type = js_uint8array;

    err = js_get_typedarray_info(env, value, &type, (void **) &buffer.data, &n_elements, NULL, NULL);
    if (err < 0) return err;

    assert(type < stride.size());
    buffer.len = n_elements * stride[type];

    return 0;
  }
};

template <>
struct js_type_info_t<quickbit_chunk_t> {
  using type = js_value_t *;

  static auto
  signature() {
    return js_object;
  }

  static auto
  unmarshall(js_env_t *env, js_value_t *value, quickbit_chunk_t &chunk) {
    int err;

    js_anybuffer_t field;
    err = js_get_property(env, value, "field", field);
    if (err < 0) return err;

    uint32_t offset;
    err = js_get_property(env, value, "offset", offset);
    if (err < 0) return err;

    chunk = {
      .field = field.data,
      .len = field.len,
      .offset = offset,
    };

    return 0;
  }
};

static inline uint32_t
quickbit_napi_get(
  js_env_t *env,
  js_receiver_t,
  js_anybuffer_t field,
  int32_t bit
) {
  return quickbit_get(field.data, field.len, bit);
}

static inline uint32_t
quickbit_napi_set(
  js_env_t *env,
  js_receiver_t,
  js_anybuffer_t field,
  int32_t bit,
  uint32_t value
) {
  return quickbit_set(field.data, field.len, bit, value);
}

static inline void
quickbit_napi_fill(
  js_env_t *env,
  js_receiver_t,
  js_anybuffer_t field,
  uint32_t value,
  int32_t start,
  int32_t end
) {
  quickbit_fill(field.data, field.len, value, start, end);
}

static inline void
quickbit_napi_clear(
  js_env_t *env,
  js_receiver_t,
  js_anybuffer_t field,
  std::vector<quickbit_chunk_t> chunks
) {
  for (auto chunk : chunks) {
    quickbit_clear(field.data, field.len, &chunk);
  }
}

static inline int32_t
quickbit_napi_find_first(
  js_env_t *env,
  js_receiver_t,
  js_anybuffer_t field,
  uint32_t value,
  int32_t position
) {
  return quickbit_find_first(field.data, field.len, value, position);
}

static inline int32_t
quickbit_napi_find_last(
  js_env_t *env,
  js_receiver_t,
  js_anybuffer_t field,
  uint32_t value,
  int32_t position
) {
  return quickbit_find_last(field.data, field.len, value, position);
}

static inline void
quickbit_napi_index_init(
  js_env_t *env,
  js_receiver_t,
  js_typedarray_t<uint8_t> index_buf,
  js_anybuffer_t field
) {
  int err;

  quickbit_index_t *index;
  err = js_get_typedarray_info<quickbit_index_t>(env, index_buf, index);
  assert(err == 0);

  quickbit_index_init((uint8_t *) index, field.data, field.len);
}

static inline void
quickbit_napi_index_init_sparse(
  js_env_t *env,
  js_receiver_t,
  js_typedarray_t<uint8_t> index_buf,
  std::vector<quickbit_chunk_t> chunks
) {
  int err;

  quickbit_index_t *index;
  err = js_get_typedarray_info<quickbit_index_t>(env, index_buf, index);
  assert(err == 0);

  quickbit_index_init_sparse((uint8_t *) index, chunks.data(), chunks.size());
}

static inline uint32_t
quickbit_napi_index_update(
  js_env_t *env,
  js_receiver_t,
  js_typedarray_t<uint8_t> index_buf,
  js_anybuffer_t field,
  int32_t bit
) {
  int err;

  quickbit_index_t *index;
  err = js_get_typedarray_info<quickbit_index_t>(env, index_buf, index);
  assert(err == 0);

  return quickbit_index_update((uint8_t *) index, field.data, field.len, bit);
}

static inline uint32_t
quickbit_napi_index_update_sparse(
  js_env_t *env,
  js_receiver_t,
  js_typedarray_t<uint8_t> index_buf,
  js_anybuffer_t field,
  uint32_t offset,
  int32_t bit
) {
  int err;

  quickbit_index_t *index;
  err = js_get_typedarray_info<quickbit_index_t>(env, index_buf, index);
  assert(err == 0);

  quickbit_chunk_t chunk = {
    .field = field.data,
    .len = field.len,
    .offset = offset,
  };

  return quickbit_index_update_sparse((uint8_t *) index, &chunk, 1, bit);
}

static inline uint32_t
quickbit_napi_skip_first(
  js_env_t *env,
  js_receiver_t,
  js_typedarray_t<uint8_t> index_buf,
  uint32_t len,
  uint32_t value,
  int32_t position
) {
  int err;

  quickbit_index_t *index;
  err = js_get_typedarray_info<quickbit_index_t>(env, index_buf, index);
  assert(err == 0);

  return quickbit_skip_first((uint8_t *) index, len, value, position);
}

static inline uint32_t
quickbit_napi_skip_last(
  js_env_t *env,
  js_receiver_t,
  js_typedarray_t<uint8_t> index_buf,
  uint32_t len,
  uint32_t value,
  int32_t position
) {
  int err;

  quickbit_index_t *index;
  err = js_get_typedarray_info<quickbit_index_t>(env, index_buf, index);
  assert(err == 0);

  return quickbit_skip_last((uint8_t *) index, len, value, position);
}

static js_value_t *
quickbit_napi_exports(js_env_t *env, js_value_t *exports) {
  int err;

  err = js_set_property<uint32_t>(env, exports, "sizeof_quickbit_index_t", sizeof(quickbit_index_t));
  assert(err == 0);

  err = js_set_property<quickbit_napi_get>(env, exports, "quickbit_napi_get");
  assert(err == 0);

  err = js_set_property<quickbit_napi_set>(env, exports, "quickbit_napi_set");
  assert(err == 0);

  err = js_set_property<quickbit_napi_fill>(env, exports, "quickbit_napi_fill");
  assert(err == 0);

  err = js_set_property<quickbit_napi_clear>(env, exports, "quickbit_napi_clear");
  assert(err == 0);

  err = js_set_property<quickbit_napi_find_first>(env, exports, "quickbit_napi_find_first");
  assert(err == 0);

  err = js_set_property<quickbit_napi_find_last>(env, exports, "quickbit_napi_find_last");
  assert(err == 0);

  err = js_set_property<quickbit_napi_index_init>(env, exports, "quickbit_napi_index_init");
  assert(err == 0);

  err = js_set_property<quickbit_napi_index_init_sparse>(env, exports, "quickbit_napi_index_init_sparse");
  assert(err == 0);

  err = js_set_property<quickbit_napi_index_update>(env, exports, "quickbit_napi_index_update");
  assert(err == 0);

  err = js_set_property<quickbit_napi_index_update_sparse>(env, exports, "quickbit_napi_index_update_sparse");
  assert(err == 0);

  err = js_set_property<quickbit_napi_skip_first>(env, exports, "quickbit_napi_skip_first");
  assert(err == 0);

  err = js_set_property<quickbit_napi_skip_last>(env, exports, "quickbit_napi_skip_last");
  assert(err == 0);

  return exports;
}

BARE_MODULE(quickbit_napi, quickbit_napi_exports)
