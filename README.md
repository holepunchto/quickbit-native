# quickbit-native

https://github.com/holepunchto/libquickbit JavaScript bindings for Node.js.

```sh
npm install quickbit-native
```

## Usage

```js
const { get, set, indexOf } = require('quickbit-native')

const field = Buffer.alloc(256) // 2048 bits

const changed = set(field, 1000)
// true

get(field, 1000)
// true

indexOf(field, true)
// 1000
```

## License

ISC
