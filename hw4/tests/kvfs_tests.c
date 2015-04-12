#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "tester.h"
#include "../kvfs.h"

struct fuse_operations kvfs_oper;

int mount_test(){
   int argc = 2;
   char* argv[] = { "kvfs",
                    "test_sandbox",
                    NULL,
                  };
   int status;
   int mount_success;
   int pid = fork();
   if (pid == 0) {
        exit(fuse_main(argc, argv, &kvfs_oper, NULL));
   } else {
     waitpid(pid, &status, 0);
     mount_success = WEXITSTATUS(status) == 0;
   }
   usleep(1000);
   int superblock_access = (access("./.superblock", F_OK) != -1);
   return  mount_success && superblock_access;
}

int superblock_size_test(){
   int argc = 2;
   char* argv[] = { "kvfs",
                    "test_sandbox",
                    NULL,
                  };
   int status;
   int mount_success;
   int pid = fork();
   if (pid == 0) {
        exit(fuse_main(argc, argv, &kvfs_oper, NULL));
   } else {
     waitpid(pid, &status, 0);
     mount_success = WEXITSTATUS(status) == 0;
   }
   usleep(1000);
   int superblock_access = (access("./.superblock", F_OK) != -1);
   int fd = open("./.superblock", O_RDONLY);
   int size = lseek(fd, 0, SEEK_END);
   return size == SUPERBLOCK_SIZE;
}


int read_test() {
    return 0;
}

int write_test() {
    return 0;
}

int open_test() {
  return 0; 
}

int move_fs() {
    return 0;
}

int delete_fs() {
    return 0;
}

test_info_t fs_tests[] = {
  {"FS mount", mount_test},
  {"FS sb size", superblock_size_test},
  {"FS read", read_test},
  {"FS write", write_test},
  {"FS move", move_fs},
  {"FS delete", delete_fs},
  NULL_TEST_INFO
};

int test_init() {
    return system("mkdir -p test_sandbox") != -1;
}

int test_clean() {
    int ret1 = system("fusermount -u test_sandbox");
    int ret2 = system("rm -rf test_sandbox");
    return 0;
    int ret3 = system("rm -rf .superblock");
    return (ret1 != -1) && (ret2 != -1) && (ret3 != -1);
}
suite_info_t fs_tests_suite = {"FS Tests", test_init, test_clean, fs_tests};

int main() {
    umask(0);

   int argc = 2;
   char* argv[] = { "kvfs",
                    "test_sandbox",
                    NULL,
                  };
   mountparent = (char*) calloc(1,PATH_MAX);
   realpath(".", mountparent);
   safe_run_suite(&fs_tests_suite);
}
