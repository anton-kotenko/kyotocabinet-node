var Bindings;

var Fs = require('fs');
console.log("cwd: ", process.cwd());
console.log("dirname", __dirname);
console.log(process.versions);
console.log("ls build", Fs.readdirSync(__dirname + '/build'));
try {
  console.log("ls build/Release", Fs.readdirSync(__dirname + '/build/Release'));
} catch (e) {};

try {
  console.log("ls build/Debug", Fs.readdirSync(__dirname + '/build/Debug'));
} catch (e) {};
try {
  Bindings = require(__dirname + '/build/Release/kyotonode.node');
} catch (e) {
  console.log('can\'t load release', e);
  Bindings = require(__dirname + '/build/Debug/kyotonode.node');
} 
module.exports = Bindings;
