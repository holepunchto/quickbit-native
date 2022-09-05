{
  'targets': [{
    'target_name': 'quickbit',
    'include_dirs': [
      '<!(node -e "require(\'napi-macros\')")',
      './vendor/libquickbit/include',
    ],
    'dependencies': [
      './vendor/libquickbit.gyp:libquickbit',
    ],
    'sources': [
      './binding.c',
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
