const binding = require('node-gyp-build')(__dirname)
const b4a = require('b4a')

exports.indexOf = function indexOf (field, value, position = 0, index = null) {
  if (typeof position === 'object') {
    index = position
    position = 0
  }

  return binding.field_napi_index_of(
    field,
    value,
    position,
    index ? 1 : 0,
    index ? index.handle : null
  )
}

exports.Index = class Index {
  constructor (field, value = 0) {
    this.value = value ? 1 : 0
    this.handle = b4a.alloc(binding.sizeof_field_index_t)

    binding.field_napi_index_init(this.handle, field, value)
  }
}
