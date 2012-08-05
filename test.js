kc = require('./kyotonode.node');
db = new kc.N12kyotocabinet6PolyDBE();
console.log(db.open('/tmp/db.kch', db.OCREATE | db.OWRITER));
console.log('size:', db.size());
console.log('count:', db.count());
console.log('path:', db.path());

console.log("append zzz:", db.append('zzz', 'asdfasdf'));
console.log('check zzz:', db.check('zzz'));
console.log('check qqq:', db.check('qqq'));
console.log('get zzz:', db.get('zzz'));
console.log('get qqq:', db.get('qqq'));
console.log('increment aaaa:', db.increment('aaaa', 123));
console.log('get aaaa:', db.get('aaaa'));
console.log('remove zzz:', db.remove('zzz'));
console.log('check zzz:', db.check('zzz'));
console.log('set bulk', db.set_bulk({
  aaa: 'bbb',
  ccc: 'ddd',
  eee: 'fff'
}));
console.log('remove bulk', db.remove_bulk([
  'aaa', 'ccc'
]));
console.log('get bulk', db.get_bulk(['eee', 'aaaa']));
