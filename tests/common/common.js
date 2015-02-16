var Assert = require('assert');
var Os = require('os');
var Fs = require('fs');
var module = require('../../');
var randString = function () {
  return String(Date.now()) + String(Math.random());
};
var randomFill = function (instance, count) {
  var i;
  for (i = 0; i < count; i++) {
    //enshure key is unique to avoid override
    Assert.ok(instance.set(randString() + i, randString()));
  }
}

describe("module", function () {
  it("should contain classes for all database types", function () {
    Assert.ok(module.PolyDB instanceof Function);
    Assert.ok(module.HashDB instanceof Function);
    Assert.ok(module.TreeDB instanceof Function);
    Assert.ok(module.DirDB instanceof Function);
    Assert.ok(module.ForestDB instanceof Function);
    Assert.ok(module.TextDB instanceof Function);
    Assert.ok(module.ProtoHashDB instanceof Function);
    Assert.ok(module.ProtoTreeDB instanceof Function);
    Assert.ok(module.StashDB instanceof Function);
    Assert.ok(module.CacheDB instanceof Function);
    Assert.ok(module.GrassDB instanceof Function);
  });
});
//describe("PolyDB", commonTest.bind(null, "PolyDB")); 
describe("HashDB", commonTest.bind(null, "HashDB")); 
describe("TreeDB", commonTest.bind(null, "TreeDB")); 
describe("DirDB", commonTest.bind(null, "DirDB")); 
describe("ForestDB", commonTest.bind(null, "ForestDB")); 
describe("TextDB", commonTest.bind(null, "TextDB")); 
describe("ProtoHashDB", commonTest.bind(null, "ProtoHashDB")); 
describe("ProtoTreeDB", commonTest.bind(null, "ProtoTreeDB")); 
describe("StashDB", commonTest.bind(null, "StashDB")); 
describe("CacheDB", commonTest.bind(null, "CacheDB")); 
describe("GrassDB", commonTest.bind(null, "GrassDB")); 

//describe("PolyDB", readWriteTest.bind(null, "PolyDB")); 
describe("HashDB", readWriteTest.bind(null, "HashDB")); 
describe("TreeDB", readWriteTest.bind(null, "TreeDB")); 
describe("DirDB", readWriteTest.bind(null, "DirDB")); 
describe("ForestDB", readWriteTest.bind(null, "ForestDB")); 
//describe("TextDB", readWriteTest.bind(null, "TextDB")); 
describe("ProtoHashDB", readWriteTest.bind(null, "ProtoHashDB")); 
describe("ProtoTreeDB", readWriteTest.bind(null, "ProtoTreeDB")); 
describe("StashDB", readWriteTest.bind(null, "StashDB")); 
describe("CacheDB", readWriteTest.bind(null, "CacheDB")); 
describe("GrassDB", readWriteTest.bind(null, "GrassDB")); 

function commonTest (DbClassName) {
  var DbClass = module[DbClassName],
    dbPath = Os.tmpdir() + '/' + DbClassName + Math.random();

  it("should not fail while creaing instance", function () {
      var instance = new DbClass();
      Assert.ok(instance);
  });

  describe(DbClassName + 'instance', function () {
    it("should contain open modes", function () {
      var instance = new DbClass(),
        openModes = {
          OREADER: 1,
          OWRITER: 2,
          OCREATE: 4,
          OTRUNCATE: 8,
          OAUTOTRAN: 16,
          OAUTOSYNC: 32,
          ONOLOCK: 64,
          OTRYLOCK: 128,
          ONOREPAIR: 256
        };
      Assert.ok(instance.OpenMode instanceof Object);
      Object.keys(openModes).forEach(function (mode) {
        Assert.strictEqual(instance.OpenMode[mode], openModes[mode]);
      });
    });
    it("should contain error codes", function () {
      var instance = new DbClass(),
        errorCodes = {
          SUCCESS: 0,
          NOIMPL: 1,
          INVALID: 2,
          NOREPOS: 3,
          NOPERM: 4,
          BROKEN: 5,
          NOREC: 7,
          DUPREC: 6,
          LOGIC: 8,
          SYSTEM: 9,
          MISC: 15
        };
      Assert.ok(instance.ErrorCode instanceof Object);
      Object.keys(errorCodes).forEach(function (code) {
        Assert.strictEqual(instance.ErrorCode[code], errorCodes[code]);
      });
    });
    it("should open and close", function () {
      var instance = new DbClass();
      Assert.ok(instance.open(dbPath, instance.OpenMode.OCREATE | instance.OpenMode.OWRITER)); 
      Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
      Assert.strictEqual(instance.path(), dbPath);
      //FIXME
      //Assert.strictEqual(instance.count(), 0);
      //Assert.ok(instance.size() > 0);
      //Assert.ok(Fs.existsSync(dbPath));
      Assert.ok(instance.close()); 
    });
  });
};

function readWriteTest (DbClassName) {
  var Fs = require('fs'),
    DbClass = module[DbClassName],
    instance;

  beforeEach(function () {
    dbPath = Os.tmpdir() + '/' + DbClassName + Math.random();
    instance = new DbClass();
    instance.open(dbPath, instance.OpenMode.OCREATE | instance.OpenMode.OWRITER); 
    //Assert.strictEqual(instance.count(), 0); FIXME textDB
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
  });
  afterEach(function () {
    var path = instance.path();
    instance.close();
    try {
      //FIXME handle in memory databases
      //FIXME handle directory databases
      Fs.unlinkSync(path);    
    } catch (e) {}
    instance = null;
  });

  it('get for unknown key should return undefined', function () {
    Assert.strictEqual(instance.get('zzz'), undefined);
    Assert.strictEqual(instance.error().code, instance.ErrorCode.NOREC);
  });  
  it('set should work', function () {
    var key = randString(),
      value = randString(),
      replacement = randString();

    Assert.ok(instance.set(key, value));
    Assert.strictEqual(instance.count(), 1);
    Assert.strictEqual(instance.get(key), value);
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.ok(instance.set(key, replacement));
    Assert.strictEqual(instance.count(), 1);
    Assert.strictEqual(instance.get(key), replacement);
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
  });
  it('massive set should work', function () {
    var count = 1e3,
      i,
      values = [],
      last; 

    for (i = 0; i < count; i++) {
      values.push(last = {
        value: randString(),
        //enshure unique keys
        key: randString() + i
      });
      Assert.ok(instance.set(last.key, last.value)); 
    }
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.strictEqual(instance.count(), count);
    for (i = 0; i < count; i++) {
      Assert.strictEqual(instance.get(values[i].key), values[i].value);
    }
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
  });
  it('remove should work', function () {
    var key = randString(),
      value = randString();

    Assert.ok(instance.set(key, value));
    Assert.strictEqual(instance.get(key), value);
    Assert.strictEqual(instance.count(), 1);
    Assert.ok(instance.remove(key)); 
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.strictEqual(instance.count(), 0);
    Assert.strictEqual(instance.get(key), undefined);
    Assert.ok(!instance.remove(key)); 
    Assert.strictEqual(instance.error().code, instance.ErrorCode.NOREC);
  });
  it('clear should work', function () {
    var count = 1e3;
    Assert.strictEqual(instance.count(), 0);
    randomFill(instance, count);
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.strictEqual(instance.count(), count);
    instance.clear(); 
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.strictEqual(instance.count(), 0);
  });
  it('add should work', function () {
    var key = randString(),
      value = randString(),
      replacement = randString();

    Assert.ok(instance.add(key, value)); 
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.strictEqual(instance.count(), 1);
    Assert.strictEqual(instance.get(key), value);
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.ok(!instance.add(key, replacement));
    Assert.strictEqual(instance.get(key), value);
    Assert.strictEqual(instance.error().code, instance.ErrorCode.DUPREC);
  });

  it('replace should work', function () {
    var key = randString(),
      value = randString(),
      replacement = randString();

    Assert.ok(instance.add(key, value)); 
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.strictEqual(instance.count(), 1);
    Assert.strictEqual(instance.get(key), value);
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.ok(instance.replace(key, replacement));
    Assert.strictEqual(instance.get(key), replacement);
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
  });

  it('replace should not work on missing key', function () {
    var key = randString(),
      replacement = randString();

    Assert.ok(!instance.replace(key, replacement));
    Assert.strictEqual(instance.error().code, instance.ErrorCode.NOREC);
    Assert.strictEqual(instance.count(), 0);
    Assert.strictEqual(instance.get(key), undefined);
  });
  it('check should work', function () {
    var key = randString(),
      key2 = randString(),
      value = randString();

    Assert.ok(instance.add(key, value)); 
    Assert.ok(instance.check(key), Buffer.byteLength(value));
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.strictEqual(instance.check(key2), -1); 
    Assert.strictEqual(instance.error().code, instance.ErrorCode.NOREC);
  });
  it('append should work', function () {
    var key = randString(),
      value = randString(),
      append = randString();
    
    Assert.ok(instance.add(key, value)); 
    Assert.strictEqual(instance.get(key), value);
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.ok(instance.append(key, append));
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.strictEqual(instance.get(key), value + append);
  });
  it('append should work on missing key', function () {
    var key = randString(),
      value = randString();
    
    Assert.ok(instance.append(key, value)); 
    Assert.strictEqual(instance.get(key), value);
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
  });
  //it('increment should work', function () {
  // FIXME increment actually is inusable
  //  var key = randString(),
  //    value = Math.round(Math.random() * 1e5);
  //  
  //  Assert.ok(instance.set(key, value)); 
  //  Assert.strictEqual(instance.get(key), value);
  //  Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
  //  Assertt.strictEqual(instance.increment(key), value + 1);
  //});
  it('seize should work', function () {
    var key = randString(),
      value = randString();
    
    Assert.ok(instance.add(key, value)); 
    Assert.strictEqual(instance.get(key), value);
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.strictEqual(instance.seize(key), value);
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.strictEqual(instance.get(key), undefined);
    Assert.strictEqual(instance.count(), 0);
  });

  it('seize should correctly handle unknown key', function () {
    var key = randString();
    
    Assert.strictEqual(instance.seize(key), undefined);
    Assert.strictEqual(instance.error().code, instance.ErrorCode.NOREC);
    Assert.strictEqual(instance.count(), 0);
  });
  
  it('cas should work correctly on value match', function () {
    var key = randString(),
      value = randString(),
      value2 = value + randString();

    Assert.ok(instance.add(key, value)); 
    Assert.strictEqual(instance.get(key), value);
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.ok(instance.cas(key, value, value2));
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.strictEqual(instance.get(key), value2);
  });

  it('cas should work correctly on value mismatch', function () {
    var key = randString(),
      value = randString(),
      value2 = value + randString();

    Assert.ok(instance.add(key, value)); 
    Assert.strictEqual(instance.get(key), value);
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.ok(!instance.cas(key, value2, 'zzz'));
    Assert.strictEqual(instance.error().code, instance.ErrorCode.LOGIC);
    Assert.strictEqual(instance.get(key), value);
  });
  
  it('set_bulk should work', function () {
    var count = 1e3,
      i,
      values = {};

    for (i = 0; i < count; i++) {
      values[randString() + i] = randString();
    }
    Assert.strictEqual(instance.set_bulk(values), count);
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.strictEqual(instance.count(), count);
    Object.keys(values).forEach(function (key) {
      Assert.strictEqual(instance.get(key), values[key]);
    });
  });
  it('remove_bulk should work', function () {
    var count = 1e3,
      i,
      values = {};

    for (i = 0; i < count; i++) {
      values[randString() + i] = randString();
    }
    Assert.strictEqual(instance.set_bulk(values), count);
    Assert.strictEqual(count / 2, instance.remove_bulk(Object.keys(values).filter(function (key, i) {
      return !(i % 2);
    })));
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.strictEqual(instance.count(), count / 2);
  });
  it('get_bulk should work', function () {
   var count = 1e3,
      i,
      values = {},
      keysToFetch,
      fetchedValues;

    for (i = 0; i < count; i++) {
      values[randString() + i] = randString();
    }
    Assert.strictEqual(instance.set_bulk(values), count);
    keysToFetch = Object.keys(values).filter(function (key, i) {
      return !(i % 2);
    });   
    fetchedValues = instance.get_bulk(keysToFetch); 
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.ok(fetchedValues instanceof Object);
    Assert.strictEqual(Object.keys(fetchedValues).length, keysToFetch.length);
    Assert.ok(keysToFetch.every(function (key) {
      return fetchedValues[key] === values[key]; 
    }));
  });

  it('should be impossible to begin transaction in transaction', function () {
    Assert.ok(instance.begin_transaction());
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.ok(!instance.begin_transaction_try());
    Assert.strictEqual(instance.error().code, instance.ErrorCode.LOGIC);
    Assert.ok(instance.end_transaction(false));
    Assert.ok(instance.begin_transaction());
  });
  it('reject transaction should reject transaction', function () {
    var key1 = randString(),
      key2 = randString(),
      value1 = randString(),
      value2 = randString();
    
    Assert.ok(instance.begin_transaction());
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.ok(instance.set(key1, value1));  
    Assert.ok(instance.set(key2, value2));  
    Assert.ok(instance.end_transaction(false));
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.strictEqual(instance.count(), 0);
  });
  it('commit transaction should commit transaction', function () {
    var key1 = randString(),
      key2 = randString(),
      value1 = randString(),
      value2 = randString();
    
    Assert.ok(instance.begin_transaction());
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.ok(instance.set(key1, value1));  
    Assert.ok(instance.set(key2, value2));  
    Assert.ok(instance.end_transaction(true));
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.strictEqual(instance.count(), 2);
    Assert.strictEqual(instance.get(key2), value2);
    Assert.strictEqual(instance.get(key1), value1);
  });
  it('cursor should work', function () {
    var cursor = instance.cursor();
    Assert.strictEqual(instance.error().code, instance.ErrorCode.SUCCESS);
    Assert.ok(cursor instanceof Object);
  });
};

