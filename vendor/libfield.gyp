{
  'targets': [{
    'target_name': 'field',
    'type': 'static_library',
    'include_dirs': [
      './libfield/vendor/libsimd/include',
    ],
    'sources': [
      './libfield/src/field.c',
    ],
    'configurations': {
      'Debug': {
        'defines': ['DEBUG'],
      },
      'Release': {
        'defines': ['NDEBUG'],
      },
    },
    'xcode_settings': {
      'OTHER_CFLAGS': [
        '-O3',
      ]
    },
    'cflags': [
      '-O3',
    ],
  }]
}
