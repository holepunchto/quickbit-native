{
  'targets': [{
    'target_name': 'libquickbit',
    'type': 'static_library',
    'include_dirs': [
      './libquickbit/vendor/libsimdle/include',
    ],
    'sources': [
      './libquickbit/src/quickbit.c',
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
