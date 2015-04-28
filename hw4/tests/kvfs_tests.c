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

int try_mount() {
    int argc = 2;
    char* argv[] = { "kvfs", "test_sandbox", NULL };
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
    return mount_success && superblock_access;
}

int mount_test(){
    int mount = try_mount();
    if (!mount) {
        return 0;
    }
    int umount = (system("fusermount -u test_sandbox") == 0);
    return 1;
}

int superblock_size_test(){
    int mount = try_mount();
    if (!mount) {
        return 0;
    }
    int fd = open("./.superblock", O_RDONLY);
    int size = lseek(fd, 0, SEEK_END);
    int umount = (system("fusermount -u test_sandbox") == 0);
    return size == SUPERBLOCK_SIZE;
}

int write_test() {
    int mount = try_mount();
    if (!mount) {
        return 0;
    }
    FILE *fp;
    char filename[] = "test_sandbox/test_file";
    fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Can't open file %s!\n", filename);
        return 0;
    }
    fprintf(fp, "helllooooo");
    fclose(fp);
    int umount = (system("fusermount -u test_sandbox") == 0);
    return 1;
}

int read_test() {
    int mount = try_mount();
    if (!mount) {
        return 0;
    }

    FILE *fp;
    char filename[] = "test_sandbox/test_file";
    char buffer[4096];
    fp = fopen(filename, "r");
    if (fp == NULL) {
      fprintf(stderr, "Can't open file %s!\n", filename);
      return 0;
    }
    bool fail = true;
    while (fscanf(fp, "%s", buffer) == 1) {
        if (!strcmp("helllooooo", buffer)) {
            fail = false;
        }
    }
    fclose(fp);
    int umount = (system("fusermount -u test_sandbox") == 0);
    if (fail) {
        return 0;
    } else {
        return 1;
    }
}

int rename_test() {
    int mount = try_mount();
    if (!mount) {
        return 0;
    }

    char filename[] = "test_sandbox/test_file";
    char new_filename[] = "test_sandbox/moved_file";
    rename(filename, new_filename);
    char buffer[4096];
    FILE* fp = fopen(new_filename, "r");
    if (fp == NULL) {
      fprintf(stderr, "Can't open file %s!\n", new_filename);
      return 0;
    }
    bool fail = true;
    while (fscanf(fp, "%s", buffer) == 1) {
        if (!strcmp("helllooooo", buffer)) {
            fail = false;
        }
    }
    fclose(fp);
    int umount = (system("fusermount -u test_sandbox") == 0);
    if (fail) {
        return 0;
    } else {
        return 1;
    }
}

int open_test() {
    return 0;
}

int move_fs() {
    return 0;
}

test_info_t fs_tests[] = {
    {"FS mount", mount_test},
    {"FS sb size", superblock_size_test},
    {"FS write", write_test},
    {"FS read", read_test},
    {"FS rename", rename_test},
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
    char* argv[] = { "kvfs", "test_sandbox", NULL };
    mountparent = (char*) calloc(1,PATH_MAX);
    realpath(".", mountparent);
    safe_run_suite(&fs_tests_suite);
}
