CC=gcc
INCLUDE_PATH= -I /usr/include/node -I /usr/include -I /usr/include/nodejs
LIBS_PATH= -L /usr/lib
LIBS= -lv8 -lstdc++ -lkyotocabinet
DEFINES=-D LARGEFILE_SOURCE -D FILE_OFFSET_BITS=64
kyotonode.node: main.cc
	$(CC) -o kyotonode.node -shared -Wall -g  -DPIC -fpic -fPIC $(DEFINES) $(INCLUDE_PATH) $(LIBS_PATH) main.cc $(LIBS)
