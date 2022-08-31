import test from 'brittle'
import b4a from 'b4a'

import { indexOf, Index } from './index.js'

test('indexOf', (t) => {
  const field = b4a.alloc(1 << 18)
  field[100000] = 1

  t.is(indexOf(field, 1), 800000)
})

test('indexOf + index', (t) => {
  const field = b4a.alloc(1 << 18)
  field[100000] = 1

  const index = new Index(field)

  t.is(indexOf(field, 1, index), 800000)
})
