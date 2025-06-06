import test from 'brittle'
import b4a from 'b4a'

import { findFirst, Index } from './index.js'

test('findFirst', async (t) => {
  const field = b4a.alloc(1 << 18)
  field[100000] = 1

  await t.test('with index', async (t) => {
    const ops = 10000000
    const index = Index.from(field)

    let r

    const elapsed = await t.execution(() => {
      for (let i = 0; i < ops; i++) {
        r = findFirst(field, true, index.skipFirst(false))
      }
    })

    t.is(r, 800000)

    t.comment(Math.round((ops / elapsed) * 1e3) + ' ops/s')
  })

  await t.test('without index', async (t) => {
    const ops = 1000

    let r

    const elapsed = await t.execution(() => {
      for (let i = 0; i < ops; i++) {
        r = findFirst(field, true)
      }
    })

    t.is(r, 800000)

    t.comment(Math.round((ops / elapsed) * 1e3) + ' ops/s')
  })
})
