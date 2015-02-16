var Bindings;

try {
  Bindings = require(__dirname + '/build/Release/kyotonode.node');
} catch (e) {
  Bindings = require(__dirname + '/build/Debug/kyotonode.node');
}
module.exports = Bindings;
