#include "kvfs.h"

struct fuse_operations kvfs_oper;

const char* USAGE = "Usage: kvfs mountpoint";

int main(int argc, char *argv[])
{
    umask(0);
        char* mountpoint = (char*) calloc(1,PATH_MAX);
        realpath(argv[1], mountpoint);
        mountparent = dirname(mountpoint);
        printf("%s", mountparent);
   return fuse_main(argc, argv, &kvfs_oper, NULL);
}
