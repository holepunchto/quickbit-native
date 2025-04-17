#include <cstdint>
#include <js.h>
#include <jstl.h>
#include <bare.h>
#include <quickbit.h>
#include <stdlib.h>

static inline void
quickbit_napi_argv_chunks (js_env_t *env, std::vector<js_value_t*> &array, quickbit_chunk_t **chunks, uint32_t *len) {
  int err;
  // Question: Can I subclass js_handle_t with a quickbit_chunk_t prop
  // to avoid this second calloc?
  *len = array.size();
  *chunks = (quickbit_chunk_t *) calloc(*len, sizeof(quickbit_chunk_t));

  size_t i = 0;
  for (js_value_t *&napi_chunk : array) {
    js_typedarray_t<uint8_t> napi_field;
    err = js_get_named_property(env, napi_chunk, "field", napi_field);
    assert(err == 0);

    js_value_t *napi_offset;
    err = js_get_named_property(env, napi_chunk, "offset", &napi_offset);
    assert(err == 0);

    std::span<uint8_t> view;
    err = js_get_typedarray_info(env, napi_field, view);
    assert(err == 0);

    uint32_t offset;
    err = js_get_value_uint32(env, napi_offset, &offset);
    assert(err == 0);

    (*chunks)[i++] = (quickbit_chunk_t){
      .field = view.data(),
      .len = view.size_bytes(),
      .offset = offset,
    };
  }
}

uint32_t quickbit_napi_get (js_env_t *env, js_receiver_t, js_typedarray_t<uint8_t> field_buf, int32_t bit) {
  int err;

  std::span<uint8_t> field;

  err = js_get_typedarray_info(env, field_buf, field);
  assert(err == 0);

  return quickbit_get(field.data(), field.size_bytes(), bit);
}

uint32_t quickbit_napi_set (js_env_t *env, js_receiver_t, js_typedarray_t<uint8_t> field_buf, int32_t bit, uint32_t value) {
  int err;

  std::span<uint8_t> field;

  err = js_get_typedarray_info(env, field_buf, field);
  assert(err == 0);

  return quickbit_set(field.data(), field.size_bytes(), bit, value);
}

void quickbit_napi_fill (js_env_t *env, js_receiver_t, js_typedarray_t<uint8_t> field_buf, uint32_t value, int32_t start, int32_t end) {
  int err;

  std::span<uint8_t> field;

  err = js_get_typedarray_info(env, field_buf, field);
  assert(err == 0);

  quickbit_fill(field.data(), field.size_bytes(), value, start, end);
}

void quickbit_napi_clear (
    js_env_t *env,
    js_receiver_t,
    js_typedarray_t<uint8_t> field_buf,
    std::vector<js_value_t *> chunks_array
) {
  int err;

  std::span<uint8_t> field;
  err = js_get_typedarray_info(env, field_buf, field);
  assert(err == 0);

  quickbit_chunk_t *chunks;
  uint32_t len;
  quickbit_napi_argv_chunks(env, chunks_array, &chunks, &len);

  for (size_t i = 0; i < len; i++) {
    quickbit_clear(field.data(), field.size_bytes(), &chunks[i]);
  }

  free(chunks);
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

  std::span<uint8_t> index;
  err = js_get_typedarray_info(env, index_buf, index);
  assert(err == 0);

  std::span<uint8_t> field;
  err = js_get_typedarray_info(env, field_buf, field);
  assert(err == 0);

  quickbit_index_init(index.data(), field.data(), field.size_bytes());
}

void
quickbit_napi_index_init_sparse (
    js_env_t *env,
    js_receiver_t,
    js_typedarray_t<uint8_t> index_buf,
    std::vector<js_value_t *> chunks_array
) {
  int err;

  std::span<uint8_t> index;
  err = js_get_typedarray_info(env, index_buf, index);
  assert(err == 0);

  quickbit_chunk_t *chunks;
  uint32_t len;
  quickbit_napi_argv_chunks(env, chunks_array, &chunks, &len);

  quickbit_index_init_sparse(index.data(), chunks, len);

  free(chunks);
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

  std::span<uint8_t> index;
  err = js_get_typedarray_info(env, index_buf, index);
  assert(err == 0);

  std::span<uint8_t> field;
  err = js_get_typedarray_info(env, field_buf, field);
  assert(err == 0);

  return quickbit_index_update(index.data(), field.data(), field.size_bytes(), bit);
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

  std::span<uint8_t> index;
  err = js_get_typedarray_info(env, index_buf, index);
  assert(err == 0);

  std::span<uint8_t> field;
  err = js_get_typedarray_info(env, field_buf, field);
  assert(err == 0);

  quickbit_chunk_t chunk = {
    .field = field.data(),
    .len = field.size_bytes(),
    .offset = offset,
  };

  return quickbit_index_update_sparse(index.data(), &chunk, 1, bit);
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

  std::span<uint8_t> index;
  err = js_get_typedarray_info(env, index_buf, index);
  assert(err == 0);

  return quickbit_skip_first(index.data(), len, value, position);
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

  std::span<uint8_t> index;
  err = js_get_typedarray_info(env, index_buf, index);
  assert(err == 0);

  return quickbit_skip_last(index.data(), len, value, position);
}

static js_value_t *
bare_addon_exports(js_env_t *env, js_value_t *exports) {
  // NAPI_EXPORT_SIZEOF(quickbit_index_t)
  int err;

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
}

BARE_MODULE(bare_addon, bare_addon_exports)
