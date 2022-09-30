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

const Index = exports.Index = class Index {
  static from (fieldOrChunks) {
    if (Array.isArray(fieldOrChunks)) {
      return new SparseIndex(fieldOrChunks)
    } else {
      return new DenseIndex(fieldOrChunks)
    }
  }

  get byteLength () {
    return 0
  }

  constructor () {
    this.handle = b4a.allocUnsafe(binding.sizeof_quickbit_index_t)
  }

  skipFirst (value, position = 0) {
    return binding.quickbit_napi_skip_first(this.handle, this.byteLength, value ? 1 : 0, position)
  }

  skipLast (value, position = this.byteLength * 8 - 1) {
    return binding.quickbit_napi_skip_last(this.handle, this.byteLength, value ? 1 : 0, position)
  }
}

class DenseIndex extends Index {
  constructor (field) {
    super()
    this.field = field

    binding.quickbit_napi_index_init(this.handle, this.field)
  }

  get byteLength () {
    return this.field.byteLength
  }

  update (bit) {
    return binding.quickbit_napi_index_update(this.handle, this.field, bit) !== 0
  }
}

class SparseIndex extends Index {
  constructor (chunks) {
    super()
    this.chunks = chunks

    binding.quickbit_napi_index_init_sparse(this.handle, this.chunks)
  }

  get byteLength () {
    const last = this.chunks[this.chunks.length - 1]
    return last ? last.offset + last.field.byteLength : 0
  }

  update (bit) {
    return binding.quickbit_napi_index_update_sparse(this.handle, this.chunks, bit) !== 0
  }
}
