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
char superblock_path[1024];

static int open_superblock(){
    const char* superblock_file = "/.superblock";
    char fname[strlen(mountparent) + strlen(superblock_file)];
    strcpy(fname, mountparent);
    strcat(fname,superblock_file);
		strcpy(superblock_path,fname);
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
		return fd;
}

static int read_superblock(){
	int fd = open_superblock();
	int num_file;
	int res = read(fd,&num_file,sizeof(int));
	if( res == -1 ){
		perror("read_superblock");
		exit(EXIT_FAILURE);
	}
	printf("read_superblock: %d bytes fd: %d number of files %d \n",res,fd,num_file);
	free(kvfs);
	kvfs = malloc( sizeof(kvfs_t) + num_file*sizeof(fnode_t) );
	kvfs->size = num_file;
	int i;
	for(i=0;i<kvfs->size;i++){
		read(fd,&(kvfs->data[i]),sizeof(fnode_t));
		if( res == -1 ){
			perror("read_superblock");
			exit(EXIT_FAILURE);
		}
	}
	close(fd);
	return res;
}

static int write_superblock(const char *name,const char *data){
	read_superblock(); 
	kvfs = (kvfs_t*)realloc(kvfs, sizeof(kvfs_t) + (kvfs->size + 1)*sizeof(fnode_t) );
	strcpy(kvfs->data[kvfs->size].name,name);
	kvfs->data[kvfs->size].magic = FNODE_MAGIC;
	int fd = open_superblock();
	kvfs->size += 1;
	write(fd,&kvfs->size,sizeof(uint32_t));
	lseek(fd,4+(kvfs->size-1)*sizeof(fnode_t),SEEK_SET);
	write(fd,&kvfs->data[kvfs->size-1],sizeof(fnode_t));
	close(fd);
	return 0;
}

static void* kvfs_init(struct fuse_conn_info *conn) {
	int fd = open_superblock();
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
	read_superblock();
	int i;
	for(i = 0;i < kvfs->size ;i++){
		filler(buf,kvfs->data[i].name,NULL,0);
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
	read_superblock();
	int i;
	for(i = 0; i < kvfs->size ; i++){
		if(kvfs->data[i].name == (path+1) ){
			break;
		}
	}
	if(i == kvfs->size){
		write_superblock(path+1,NULL);	
	}
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


