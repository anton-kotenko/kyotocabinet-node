#Node js bindings to [Kyoto Cabinet](http://fallabs.com/kyotocabinet/) database

<img src="https://api.travis-ci.org/anton-kotenko/kyotocabinet-node.svg?branch=node-0.12-support"/>

Implemented as sychronous wrapper over c++ library, trying to copy its interface (When it was possible). So documentaion is present here http://fallabs.com/kyotocabinet/spex.html and http://fallabs.com/kyotocabinet/api/.
A lot of examples may be also found in [tests](https://github.com/anton-kotenko/kyotocabinet-node/blob/master/tests/common/common.js)

##Dependencies
Kyotocabinet library with headers. (apt-get install libkyotocabinet-dev for Debian like OS's)

##Engines
  Works with nodejs-v0.10.*, nodejs-v0.12.* and iojs-v1.* 

##Usage
```shell
npm install git+ssh://git@github.com:anton-kotenko/kyotocabinet-node.git
```
or add
```json
"kyotocabinet": "git+ssh://git@github.com:anton-kotenko/kyotocabinet-node.git",
```
line into dependencies section of package.json file
##Build
###For normal usage just run
```shell
npm install
```
in folder with sources.

###For debug version of library (compiled with -g -O0 CCFLAGS) run one of next commands
```shell
node-gyp configure && node-gyp --debug build
```
```shell
npm --debug install
```
