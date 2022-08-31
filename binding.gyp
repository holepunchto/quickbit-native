{
  'targets': [{
    'target_name': 'field',
    'include_dirs': [
      '<!(node -e "require(\'napi-macros\')")',
      './vendor/libfield/include',
    ],
    'dependencies': [
      './vendor/libfield.gyp:field',
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
