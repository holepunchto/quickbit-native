import test from 'brittle'
import b4a from 'b4a'

import { get, set, fill, clear, findFirst, findLast, Index } from './index.js'

test('get', (t) => {
  const field = b4a.alloc(1)

  field[0] = 0xff
  t.is(get(field, 0), true)
  t.is(get(field, 7), true)

  field[0] = 0xfe
  t.is(get(field, 0), false)
  t.is(get(field, 7), true)
})

test('set', (t) => {
  const field = b4a.alloc(1)
  field[0] = 0xfe

  t.is(set(field, 0), true)
  t.is(field[0], 0xff)

  t.is(set(field, 0), false)

  t.is(set(field, 0, false), true)
  t.is(field[0], 0xfe)
})

test('fill', (t) => {
  const field = b4a.alloc(4)

  fill(field, true, 4, 28)
  t.alike([...field], [0xf0, 0xff, 0xff, 0x0f])
})

test('clear aligned', (t) => {
  const field = b4a.alloc(32, 0xff)

  clear(field, { field: b4a.alloc(24, 0xff), offset: 0 })
  t.alike([...field], [0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff])
})

test('clear unaligned', (t) => {
  const field = b4a.alloc(32, 0xff)

  clear(field, { field: b4a.alloc(24, 0xff), offset: 2 })
  t.alike([...field], [0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff])
})

test('findFirst', (t) => {
  const field = b4a.alloc(1 << 18)
  field[100000] = 1

  t.is(findFirst(field, true), 800000)
  t.is(findFirst(field, true, 800000), 800000)
  t.is(findFirst(field, true, 800001), -1)
  t.is(findFirst(field, true, -1), -1)
})

test('findFirst + index', (t) => {
  const field = b4a.alloc(1 << 18)
  field[100000] = 1

  const index = Index.from(field)

  t.is(findFirst(field, true, index.skipFirst(false, 0)), 800000)
  t.is(findFirst(field, true, index.skipFirst(false, 800000)), 800000)
  t.is(findFirst(field, true, index.skipFirst(false, 800001)), -1)
  t.is(findFirst(field, true, index.skipFirst(false, -1)), -1)
})

test('index dense', (t) => {
  const field = b4a.alloc(1 << 18)
  field[0] = 1
  field[2048] = 1

  const index = Index.from(field)

  const expected = b4a.alloc(index.handle.byteLength / 2, 0xff)
  expected[0] = 0xfc
  expected[16] = expected[32] = 0xfe

  t.alike(index.handle.subarray(0, expected.byteLength), expected)

  field[4096] = 1
  index.update(32768)

  expected[0] = 0xf8
  expected[48] = 0xfe

  t.alike(index.handle.subarray(0, expected.byteLength), expected)
})

test('index sparse', (t) => {
  const chunks = [
    { field: b4a.alloc(32), offset: 0 },
    { field: b4a.alloc(32), offset: 2048 }
  ]

  chunks[0].field[0] = 1
  chunks[1].field[0] = 1

  const index = Index.from(chunks)

  const expected = b4a.alloc(index.handle.byteLength / 2, 0xff)
  expected[0] = 0xfc
  expected[16] = expected[32] = 0xfe

  t.alike(index.handle.subarray(0, expected.byteLength), expected)

  chunks.push({
    field: b4a.alloc(32), offset: 4096
  })

  chunks[2].field[0] = 1
  index.update(32768)

  expected[0] = 0xf8
  expected[48] = 0xfe

  t.alike(index.handle.subarray(0, expected.byteLength), expected)
})

test('skipLast at index boundary', (t) => {
  const field = b4a.alloc(1 << 18)

  set(field, 12800, true)

  const index = Index.from(field)

  t.is(index.skipLast(false, 12808), 12808)
})

test('large index', (t) => {
  const field = b4a.alloc(1 << 18)

  set(field, 1000000, true)

  const index = Index.from(field)

  t.is(index.skipFirst(false, 0), 999936)
})

test('findFirst various typedarrays', (t) => {
  const u32field = Uint32Array.of(0, 0, 0, 1 << 30)
  let idx = findFirst(u32field, true)
  t.is(idx, 32 * 3 + 30, 'Uint32Array')

  const u16field = Uint16Array.of(0, 0, 0, 1 << 15)
  idx = findFirst(u16field, true)
  t.is(idx, 16 * 3 + 15, 'Uint16Array')

  const u8field = Uint8Array.of(0, 0, 0, 1 << 7)
  idx = findFirst(u8field, true)
  t.is(idx, 8 * 3 + 7, 'Uint8Array')

  const bField = Buffer.from([0, 0, 0, 1 << 7])
  idx = findFirst(bField, true)
  t.is(idx, 8 * 3 + 7, 'Buffer')

  /* TODO
  const abField = u32field.buffer
  t.ok(abField instanceof ArrayBuffer)
  idx = findFirst(abField, true)
  t.is(idx, 32 * 3 + 30, 'ArrayBuffer')
  */
})

test('findLast various typedarrays', (t) => {
  let bit = 32 * 3 + 25
  const u32field = new Uint32Array(4)
  set(u32field, bit, true)
  set(u32field, bit + 1, true)
  set(u32field, bit + 2, true)
  let idx = findLast(u32field, false, bit + 2)
  t.is(idx, bit - 1, 'Uint32Array')

  bit = 16 * 3 + 9
  const u16field = new Uint16Array(4)
  set(u16field, bit, true)
  set(u16field, bit + 1, true)
  idx = findLast(u16field, false, bit + 1)
  t.is(idx, bit - 1, 'Uint16Array')

  const u8field = new Uint8Array(4)
  bit = 8 * 3 + 4
  set(u8field, bit, true)
  set(u8field, bit + 1, true)
  idx = findLast(u8field, false, bit + 1)
  t.is(idx, bit - 1, 'Uint8Array')

  const bField = Buffer.alloc(4)
  set(bField, bit, true)
  set(bField, bit + 1, true)
  idx = findLast(bField, false, bit + 1)
  t.is(idx, bit - 1, 'Buffer')

  const abField = u32field.buffer
  bit = 32 * 3 + 25
  t.ok(abField instanceof ArrayBuffer)
  idx = findLast(u32field, false, bit + 1)
  t.is(idx, bit - 1, 'ArrayBuffer')
})
