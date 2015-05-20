#include "kvfs.h"
#include <fcntl.h>

#ifdef HAVE_SETXATTR
#include <sys/xattr.h>
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef linux
/* For pread()/pwrite()/utimensat() */
#define _XOPEN_SOURCE 700
#endif

kvfs_t* kvfs;

//static int read_superblock(int fd,kvfs_t *kvfs){
	//int i;
	//for(int i = 0; i < num_file; i++){
	//	
	//}
	
	//return res;
//}

/*static int open_superblock(){
    const char* superblock_file = "/.superblock";
    char fname[strlen(mountparent) + strlen(superblock_file)];
    strcpy(fname, mountparent);
    strcat(fname,superblock_file);
    int fd = open(fname, O_RDWR);
		printf("open_superblock: file_name: %s fd: %d\n",fname,fd);
		if(fd == -1){
			perror("kvfs_create");
			exit(1);
		}	
		return fd;
}*/

static void* kvfs_init(struct fuse_conn_info *conn) {
    const char* superblock_file = "/.superblock";
    char fname[strlen(mountparent) + strlen(superblock_file)];
    strcpy(fname, mountparent);
    strcat(fname,superblock_file);
    int fd;
    if (access(fname, F_OK) == -1) {
        kvfs = calloc(1, sizeof(kvfs_t));
        kvfs->size = 0;
        int size = 0;
        fd = open(fname, O_CREAT | O_WRONLY, 0644);
        write(fd, &size, sizeof(size));
        lseek(fd, SUPERBLOCK_SIZE - 1, SEEK_SET);
        write(fd, "\0", 1);
        close(fd);
    }
    fd = open(fname, O_RDWR);
    if (fd == -1) {
        printf("kv_fs_init(): open(): %s\n", strerror(errno));
        exit(1);
    }
		int num_file;
		int res = read(fd,&num_file,sizeof(int));
		if( res == -1 ){
			perror("kvfs_init: read");
			exit(EXIT_FAILURE);
		}
		printf("kvfs_init: read: %d bytes fd: %d number of files %d \n",res,fd,num_file);
		kvfs = malloc( sizeof(kvfs_t) + num_file*sizeof(fnode_t) );
		kvfs->size = num_file;
		int i;
		for(i=0;i<kvfs->size;i++){
			read(fd,&(kvfs->data[i]),sizeof(fnode_t));
			if( res == -1 ){
				perror("kvfs_init: read");
				exit(EXIT_FAILURE);
			}
		}
		close(fd);
    return NULL;
}


static int kvfs_getattr(const char *path, struct stat *stbuf)
{
		printf("kvfs_getattr: %s path\n",path);
    if (!strcmp(path, "/")) {
      return lstat(mountparent, stbuf);
    }
		else{
			stbuf->st_mode = S_IFREG | 0444;
		}
    return 0;
}

static int kvfs_truncate(const char *path, off_t size)
{
    return 0;
}

static int kvfs_readdir(const char *path,void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi){
	(void) offset;
	(void) fi;
	if (strcmp(path,"/") !=0 ){
		return -ENOENT;
	}
	filler(buf,".",NULL,0);
	filler(buf,"..",NULL,0);
	return 0;
}

static int kvfs_open(const char *path, struct fuse_file_info *fi){
	return 0;
}

static int kvfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
	memcpy(buf,"hello",5);
	return size;
}

static int kvfs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
	return size;
}

static int kvfs_utimens(const char *path, const struct timespec tv[2]){
	printf("In kvfs_utimens: %s\n",path);
	//int fd = open_superblock();
	//int res = read_superblock(fd,kvfs);
	return 0;
}

struct fuse_operations kvfs_oper = {
    .getattr    = kvfs_getattr,
    .truncate   = kvfs_truncate,
    .init       = kvfs_init,
		.readdir		= kvfs_readdir,	
		.open 			= kvfs_open,
		.read				= kvfs_read,
		.write			= kvfs_write,
		.utimens		= kvfs_utimens,
};


