## Prerequisites
[![FOSSA Status](https://app.fossa.io/api/projects/git%2Bgithub.com%2Fgurumian%2Fsimpledb.svg?type=shield)](https://app.fossa.io/projects/git%2Bgithub.com%2Fgurumian%2Fsimpledb?ref=badge_shield)

```bash
apt install libsqlite3-dev

(or brew)
```

You may need to install `cmake-js` as well
```
npm i -g cmake-js
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
stmt.execute({
  query: query,
  async: true,
})
.then(res => {
  console.log(res);
})
```

### INSERT
```js
let stmt = conn.createStatement();
let password = Math.round(Math.random() * 1000);
let query =  `INSERT INTO ${table} (passwd, date) VALUES(${password},datetime(\'now\',\'localtime\'));`;
stmt.execute({
  query: query,
  async: false,
})
.then(res => {
  console.log(res);
})
```

### SELECT
```js
let stmt = conn.createStatement();
let query =`SELECT idx, passwd, date FROM ${table}`;
stmt.executeQuery({
  query: query,
  async: true,
})
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
stmt.execute({
  query: query,
  async: true,
})
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
stmt.execute({
  query: query,
  async: true,
})
.then(res => {
  // 
})
```


## (Optional) Native only build
```bash
cd native;
mkdir build; cd build
cmake .. && make
```


## License
[![FOSSA Status](https://app.fossa.io/api/projects/git%2Bgithub.com%2Fgurumian%2Fsimpledb.svg?type=large)](https://app.fossa.io/projects/git%2Bgithub.com%2Fgurumian%2Fsimpledb?ref=badge_large)
