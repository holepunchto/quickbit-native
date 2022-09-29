const binding = require('node-gyp-build')(__dirname)
const b4a = require('b4a')

exports.get = function get (field, bit) {
  return binding.quickbit_napi_get(field, bit) !== 0
}

exports.set = function set (field, bit, value = true) {
  return binding.quickbit_napi_set(field, bit, value ? 1 : 0) !== 0
}

exports.fill = function fill (field, value, start = 0, end = field.byteLength * 8) {
  binding.quickbit_napi_fill(field, value ? 1 : 0, start, end)
  return field
}

exports.findFirst = function findFirst (field, value, position = 0) {
  return binding.quickbit_napi_find_first(field, value ? 1 : 0, position)
}

exports.findLast = function findLast (field, value, position = field.byteLength * 8 - 1) {
  return binding.quickbit_napi_find_last(field, value ? 1 : 0, position)
}

exports.Index = class Index {
  constructor (field) {
    if (field.byteLength > 1 << 18) throw new RangeError('Field is too large to index')

    this.field = field
    this.handle = b4a.alloc(binding.sizeof_quickbit_index_t)

    binding.quickbit_napi_index_init(this.handle, this.field)
  }

  update (bit) {
    return binding.quickbit_napi_index_update(this.handle, this.field, bit) !== 0
  }

  skipFirst (value, position = 0) {
    return binding.quickbit_napi_skip_first(this.handle, this.field.byteLength, value ? 1 : 0, position)
  }

  skipLast (value, position = this.field.byteLength * 8 - 1) {
    return binding.quickbit_napi_skip_last(this.handle, this.field.byteLength, value ? 1 : 0, position)
  }
}
