CC=gcc
INCLUDE_PATH= -I /usr/include/node -I /usr/include -I /usr/include/nodejs
LIBS_PATH= -L /usr/lib
LIBS= -lv8 -lstdc++ -lkyotocabinet
DEFINES=-D LARGEFILE_SOURCE -D FILE_OFFSET_BITS=64
kyotonode.node: main.cc
	TMPDIR=/dev/shm $(CC) -o kyotonode.node -shared -Wall -g  -DPIC -fpic -fPIC $(DEFINES) $(INCLUDE_PATH) $(LIBS_PATH) main.cc $(LIBS)
test.node: test.cc
	TMPDIR=/dev/shm $(CC) -o test.node -shared -Wall -g  -DPIC -fpic -fPIC $(DEFINES) $(INCLUDE_PATH) $(LIBS_PATH) test.cc $(LIBS)

