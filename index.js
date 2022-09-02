const binding = require('node-gyp-build')(__dirname)
const b4a = require('b4a')

exports.get = function get (field, bit) {
  if (bit < 0 || bit >= field.byteLength * 8) throw new RangeError('Out of bounds')

  return binding.field_napi_get(field, bit) !== 0
}

exports.set = function set (field, bit, value = true) {
  if (bit < 0 || bit >= field.byteLength * 8) throw new RangeError('Out of bounds')

  return binding.field_napi_set(field, bit, value ? 1 : 0) !== 0
}

exports.indexOf = function indexOf (field, value, position = 0, index = null) {
  if (typeof position === 'object') {
    index = position
    position = 0
  }

  if (position < 0 || position >= field.byteLength * 8) throw new RangeError('Out of bounds')

  return binding.field_napi_index_of(
    field,
    value ? 1 : 0,
    position,
    index ? 1 : 0,
    index ? index.handle : null
  )
}

exports.lastIndexOf = function lastIndexOf (field, value, position = field.byteLength * 8 - 1, index = null) {
  if (typeof position === 'object') {
    index = position
    position = field.byteLength * 8 - 1
  }

  if (position < 0 || position >= field.byteLength * 8) throw new RangeError('Out of bounds')

  return binding.field_napi_last_index_of(
    field,
    value ? 1 : 0,
    position,
    index ? 1 : 0,
    index ? index.handle : null
  )
}

exports.Index = class Index {
  constructor (field) {
    if (field.byteLength > 1 << 18) throw new RangeError('Field is too large to index')

    this.field = field
    this.handle = b4a.alloc(binding.sizeof_field_index_t)

    binding.field_napi_index_init(this.handle, this.field)
  }

  update (bit) {
    if (bit < 0 || bit >= this.field.byteLength * 8) throw new RangeError('Out of bounds')

    return binding.field_napi_index_update(this.handle, this.field, bit) !== 0
  }
}
