#Node js bindings to [Kyoto Cabinet](http://fallabs.com/kyotocabinet/) database

Implemented as sychronous wrapper over c++ library, trying to copy its interface (When it was possible). So documentaion is present here http://fallabs.com/kyotocabinet/spex.html and http://fallabs.com/kyotocabinet/api/.

##Dependencies
Kyotocabinet library with headers. (apt-get install libkyotocabinet-dev for Debian like OS's)
##Usage
npm install git+ssh://git@github.com:anton-kotenko/kyotocabinet-node.git

or add  
"kyotocabinet": "git+ssh://git@github.com:anton-kotenko/kyotocabinet-node.git",

line into dependencies section of package.json file
##Build
npm install
in folder with sources.
Or
node-gyp condfigure && node-gyp --debug build
to build debug version of library (-g -O0 flags)
