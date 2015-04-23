#include "kvfs.h"

struct fuse_operations kvfs_oper;

const char* USAGE = "Usage: kvfs mountpoint [OPTIONS]\n"
"\n"
"OPTIONS\n"
"  -d   enable debug output\n"
"  -f   foreground operation\n"
"  -s   disable multi-threaded operation\n";

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("%s", USAGE);
        exit(1);
    }

    if (access(argv[1], F_OK) == -1) {
        printf("kvfs: %s: %s\n", strerror(ENOENT), argv[1]);
        exit(1);
    }

    char* mountpoint = (char*) calloc(1, PATH_MAX);
    realpath(argv[1], mountpoint);
    mountparent = dirname(mountpoint);

    return fuse_main(argc, argv, &kvfs_oper, NULL);
}
