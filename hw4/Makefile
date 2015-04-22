CC = gcc
FUSE_DIR = lib/fuse
CFLAGS = -std=gnu99 -g -Wall -I $(FUSE_DIR)/include -D_FILE_OFFSET_BITS=64
TFLAGS = -DTESTS

LINKFLAGS = -L$(FUSE_DIR)/lib/.libs -lfuse

SRCS = $(wildcard *.c)

OBJS = kvfs.o

MAINOBJ = kvmount.o

TESTSRCS = $(wildcard tests/*.c)
TESTOBJS = $(TESTSRCS:.c=.o)

TESTEXE = kvfs_tests

all: fuse $(OBJS) $(MAINOBJ)
	$(CC) $(OBJS) $(MAINOBJ) $(LINKFLAGS) -o kvfs

fuse: fuse_conf
	make -C ./lib/fuse

test: $(OBJS) $(TESTOBJS)
	$(CC) -o $(TESTEXE) $^ $(LINKFLAGS)

check: test
	./$(TESTEXE)

fuse_conf:
	cd $(FUSE_DIR); ./configure --prefix=`pwd`
clean:
	rm -rf *.o fs
	rm -rf tests/*.o
	rm -f kvfs
	rm -f kvfs_tests

really-clean: clean
	$(MAKE) -C $(FUSE_DIR) clean

reset_fs:
	fusermount -u test_sandbox
	rm -rf .superblock

.PHONY: all clean check fuse fuse_conf
