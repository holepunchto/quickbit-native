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
  }]
}
