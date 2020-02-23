## Prerequisites
```bash
apt install libsqlite3-dev

(or brew)
```

You may need to install `cmake-js` as well
```
npm install -g cmake-js
```


## Install
```bash
npm i simpledb
```

## Example
See `example/`

### CREATE
```js
let connection = new Connection(db);
let stmt = connection.createStatement();
stmt.execute(`CREATE TABLE ${table}(idx INTEGER PRIMARY KEY AUTOINCREMENT, passwd TEXT, date DATETIME);`)
.then(res => {
  console.log(`create: {res}`);
})
.catch(err => {
  console.log(err);
});
```

### INSERT
```js
let connection = new Connection(db);
let stmt = connection.prepareStatement(`INSERT INTO ${table} (passwd, date) VALUES(?,datetime(\'now\',\'localtime\'));`);
stmt.setString({
  index: 1,
  value: `admin_passwd${i}`,
});

stmt.execute()
.then(res => {
  console.log(`insert: {res}`);
})
.catch(err => {
  console.log(err);
});
```

### SELECT
```js
let connection = new Connection(db);
let stmt = connection.createStatement();
stmt.executeQuery('SELECT idx, passwd, date FROM admin')
.then(res => {
  while(res.next()) {
    console.log(`${res.getInt(0)}, ${res.getString(1)}, ${res.getString(2)}`);
  }
})
.catch(err => {
  console.log(err);
});
```

### UPDATE
```js
let connection = new Connection(db);
let stmt = connection.prepareStatement(`UPDATE ${table} set passwd=?, date=datetime(\'now\',\'localtime\') WHERE idx=1;`);
stmt.setString({
  index: 1,
  value: 'new_passcode',
});
stmt.execute()
.then(res =>{
  console.log(`update: {res}`);
})
.catch(err => {
  console.log(err);
});
```

### DELETE
```js
let connection = new Connection(db);
let stmt = connection.prepareStatement('DELETE FROM admin WHERE idx=?;');
let id = 1;
stmt.setInt({
  index: 1,
  value: id,
});
stmt.execute()
.then(res => {
  console.log(`delete: ${res}`);
})
.catch(err => {
  console.log(err);
});
```


## (Optional) Native only build
```bash
cd native;
mkdir build; cd build
cmake .. && make
```
