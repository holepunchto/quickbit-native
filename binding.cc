#include <bare.h>
#include <js.h>
#include <jstl.h>
#include <quickbit.h>
#include <stdlib.h>

// workaround: `js_object_t` missing template
struct js__object_t : js_object_t {};

template <>
struct js_type_info_t<js__object_t> {
  using type = js_value_t *;

  static auto
  signature() {
    return js_object;
  }

  static int unmarshall(js_env_t *env, js_value_t *value, js__object_t &out) {
    out.value = value;
    return 0;
  }
};

static inline auto
quickbit_napi_argv_chunks(js_env_t *env, std::vector<js__object_t> &array) {
  int err;

  std::vector<quickbit_chunk_t> chunks;
  chunks.reserve(array.size());

  for (auto &obj : array) {
    js_typedarray_t<uint8_t> field;
    err = js_get_property(env, obj, "field", field);
    assert(err == 0);

    std::span<uint8_t> view;
    err = js_get_typedarray_info(env, field, view);
    assert(err == 0);

    uint32_t offset;
    err = js_get_property(env, obj, "offset", offset);
    assert(err == 0);

    chunks.emplace_back(quickbit_chunk_t {
      .field = view.data(),
      .len = view.size_bytes(),
      .offset = offset,
    });

  }

  return chunks;
}

uint32_t
quickbit_napi_get (
  js_env_t *env,
  js_receiver_t,
  js_typedarray_t<uint8_t> field_buf,
  int32_t bit
) {
  int err;

  std::span<uint8_t> field;

  err = js_get_typedarray_info(env, field_buf, field);
  assert(err == 0);

  return quickbit_get(field.data(), field.size_bytes(), bit);
}

uint32_t
quickbit_napi_set (
  js_env_t *env,
  js_receiver_t,
  js_typedarray_t<uint8_t> field_buf,
  int32_t bit,
  uint32_t value
) {
  int err;

  std::span<uint8_t> field;

  err = js_get_typedarray_info(env, field_buf, field);
  assert(err == 0);

  return quickbit_set(field.data(), field.size_bytes(), bit, value);
}

void
quickbit_napi_fill (
  js_env_t *env,
  js_receiver_t,
  js_typedarray_t<uint8_t> field_buf,
  uint32_t value,
  int32_t start,
  int32_t end
) {
  int err;

  std::span<uint8_t> field;

  err = js_get_typedarray_info(env, field_buf, field);
  assert(err == 0);

  quickbit_fill(field.data(), field.size_bytes(), value, start, end);
}

void
quickbit_napi_clear (
  js_env_t *env,
  js_receiver_t,
  js_typedarray_t<uint8_t> field_buf,
  std::vector<js__object_t> chunks_array
) {
  int err;

  std::span<uint8_t> field;
  err = js_get_typedarray_info(env, field_buf, field);
  assert(err == 0);

  auto chunks = quickbit_napi_argv_chunks(env, chunks_array);

  for (auto chunk: chunks) {
    quickbit_clear(field.data(), field.size_bytes(), &chunk);
  }
}

int32_t
quickbit_napi_find_first (
  js_env_t *env,
  js_receiver_t,
  js_typedarray_t<uint8_t> field_buf,
  uint32_t value,
  int32_t position
) {
  int err;

  std::span<uint8_t> field;
  err = js_get_typedarray_info(env, field_buf, field);
  assert(err == 0);

  return quickbit_find_first(field.data(), field.size_bytes(), value, position);
}

int32_t
quickbit_napi_find_last (
  js_env_t *env,
  js_receiver_t,
  js_typedarray_t<uint8_t> field_buf,
  uint32_t value,
  int32_t position
) {
  int err;

  std::span<uint8_t> field;
  err = js_get_typedarray_info(env, field_buf, field);
  assert(err == 0);

  return quickbit_find_last(field.data(), field.size_bytes(), value, position);
}

void
quickbit_napi_index_init (
  js_env_t *env,
  js_receiver_t,
  js_typedarray_t<uint8_t> index_buf,
  js_typedarray_t<uint8_t> field_buf
) {
  int err;

  quickbit_index_t *index;
  err = js_get_typedarray_info<quickbit_index_t>(env, index_buf, index);
  assert(err == 0);


  std::span<uint8_t> field;
  err = js_get_typedarray_info(env, field_buf, field);
  assert(err == 0);

  quickbit_index_init((uint8_t *) index, field.data(), field.size_bytes());
}

void
quickbit_napi_index_init_sparse (
  js_env_t *env,
  js_receiver_t,
  js_typedarray_t<uint8_t> index_buf,
  std::vector<js__object_t> chunks_array
) {
  int err;

  quickbit_index_t *index;
  err = js_get_typedarray_info<quickbit_index_t>(env, index_buf, index);
  assert(err == 0);


  auto chunks = quickbit_napi_argv_chunks(env, chunks_array);

  quickbit_index_init_sparse((uint8_t *) index, chunks.data(), chunks.size());
}

uint32_t
quickbit_napi_index_update (
  js_env_t *env,
  js_receiver_t,
  js_typedarray_t<uint8_t> index_buf,
  js_typedarray_t<uint8_t> field_buf,
  int32_t bit
) {
  int err;

  quickbit_index_t *index;
  err = js_get_typedarray_info<quickbit_index_t>(env, index_buf, index);
  assert(err == 0);

  std::span<uint8_t> field;
  err = js_get_typedarray_info(env, field_buf, field);
  assert(err == 0);

  return quickbit_index_update((uint8_t *) index, field.data(), field.size_bytes(), bit);
}

uint32_t
quickbit_napi_index_update_sparse (
  js_env_t *env,
  js_receiver_t,
  js_typedarray_t<uint8_t> index_buf,
  js_typedarray_t<uint8_t> field_buf,
  uint32_t offset,
  int32_t bit
) {
  int err;

  quickbit_index_t *index;
  err = js_get_typedarray_info<quickbit_index_t>(env, index_buf, index);
  assert(err == 0);


  std::span<uint8_t> field;
  err = js_get_typedarray_info(env, field_buf, field);
  assert(err == 0);

  quickbit_chunk_t chunk = {
    .field = field.data(),
    .len = field.size_bytes(),
    .offset = offset,
  };

  return quickbit_index_update_sparse((uint8_t *) index, &chunk, 1, bit);
}

uint32_t
quickbit_napi_skip_first (
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


uint32_t
quickbit_napi_skip_last (
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
bare_addon_exports(js_env_t *env, js_value_t *exports) {
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

BARE_MODULE(bare_addon, bare_addon_exports)
