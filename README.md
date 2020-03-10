SQLite3 nodejs binding. It's simple to use and supports the both asynchronous and synchronous manners.

![Node.js CI](https://github.com/gurumian/simpledb/workflows/Node.js%20CI/badge.svg)
[![Dependencies](https://david-dm.org/gurumian/simpledb.svg)](https://david-dm.org/gurumian/simpledb)
[![FOSSA Status](https://app.fossa.io/api/projects/git%2Bgithub.com%2Fgurumian%2Fsimpledb.svg?type=shield)](https://app.fossa.io/projects/git%2Bgithub.com%2Fgurumian%2Fsimpledb?ref=badge_shield)

## Prerequisites
```bash
apt install libsqlite3-dev

(or brew)
```

You may need to install `cmake-js` and `typescript`
```
npm i -g cmake-js
npm i -g typescript
```


## Install
```bash
npm i simpledbc
```

## Example
See `example/`

### CREATE
```js
const {Connection} = require('simpledbc');

let conn = new Connection(db)
let stmt = conn.createStatement()
let query = `CREATE TABLE ${table}(idx INTEGER PRIMARY KEY AUTOINCREMENT, passwd TEXT, date DATETIME);`
stmt.execute(query)
.then(res => {
  console.log(res);
})
```

### INSERT
```js
let stmt = conn.createStatement();
let password = Math.round(Math.random() * 1000);
let query =  `INSERT INTO ${table} (passwd, date) VALUES(${password},datetime(\'now\',\'localtime\'));`;
stmt.execute(query)
.then(res => {
  console.log(res);
})
```

### SELECT
```js
let stmt = conn.createStatement();
let query =`SELECT idx, passwd, date FROM ${table}`;
stmt.execute(query)
.then(res => {
  while(res.next()) {
    console.log(res.data);
    // console.log(res.obj);
  }
})
```

### UPDATE
```js
let stmt = conn.createStatement();
let password = 'new password';
let query = `UPDATE ${table} set passwd=\'${password}\', date=datetime(\'now\',\'localtime\') WHERE idx=1;`;
stmt.execute(query)
.then(res => {
  if(res) {
    console.log(`successfully updated to ${password}`);
  }
})
```

### DELETE
```js
let id = 1;
let stmt = conn.createStatement();
let query = `DELETE FROM admin WHERE idx=${id};`;
stmt.execute(query)
.then(res => {
  // 
})
```

### async / await
You can use it like this as well.
```js
async function removeAsync(conn) {
  let id = 2;
  let stmt = conn.createStatement();
  let query = `DELETE FROM admin WHERE idx=${id};`;
  let res = await stmt.execute(query)
  console.log(`removeAsync ${res}`);
}
```


## (Optional) Native only build
```bash
cd native;
mkdir build; cd build
cmake .. && make
```


## License
[![FOSSA Status](https://app.fossa.io/api/projects/git%2Bgithub.com%2Fgurumian%2Fsimpledb.svg?type=large)](https://app.fossa.io/projects/git%2Bgithub.com%2Fgurumian%2Fsimpledb?ref=badge_large)
