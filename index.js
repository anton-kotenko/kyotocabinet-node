var Bindings;
try {
  Bindings = require(__dirname + '/build/Release/kyotonode');
} catch (e) {
  Bindings = require(__dirname + '/build/Debug/kyotonode');
}
module.exports = Bindings;
