#include "uso/thread.h"
#include "uso/heap.h"
#include "mfs/sysfs.h"
#include "mfs/descriptor.h"
#include "mfs/super.h"
#include "mfs/directory.h"
#include "mfs/stream.h"

static MFS_descriptor_t *root = NULL;
static MFS_descriptor_t *threads = NULL;
static MFS_descriptor_t *serial = NULL;
static MFS_descriptor_t *ether = NULL;
static MFS_descriptor_t *heaps = NULL;

static struct MFS_super_op MFS_sysfs_super_op = { .mount = NULL,
												 .umount = NULL };

static struct MFS_directory_op MFS_sysfs_directory_op = { .open = NULL,
										.close = NULL,
										.create = NULL,
										.remove = NULL,
										.rename = NULL };

static struct MFS_stream_op MFS_sysfs_stream_op = { .open = NULL,
									.close = NULL,
									.info = NULL,
									.read = MFS_empty_read,
									.write = MFS_empty_write,
									.seek = NULL,
									.flush = NULL };

static struct MFS_vfs_op MFS_sysfs_vfs_op = { .super_op = &MFS_sysfs_super_op,
											.dir_op = &MFS_sysfs_directory_op,
											.stream_op = &MFS_sysfs_stream_op };


static void info (void)
{
	puts("sysfs\n");
}

extern bool_t
MFS_sysfs_init(void)
{
	MFS_descriptor_t *dir;
	root = MFS_mount("root", &MFS_sysfs_vfs_op, info);
    if (root != NULL){
    	if ( (dir = MFS_create_dir(root, "net")) != NULL){
			ether = MFS_create_dir(dir, "ether");
    	}
	    if ( (dir = MFS_create_dir(root, "dev")) != NULL){
			serial = MFS_create_dir(dir, "serial");
	    }
		if ( (dir = MFS_create_dir(root, "uso")) != NULL){
			threads = MFS_create_dir(dir, "threads");
			heaps = MFS_create_dir(dir, "heaps");
		}
    }
	return root && ether && serial && heaps && threads;    
}

extern MFS_descriptor_t *
MFS_sysfs_root(void)
{
	return root;
}

extern MFS_descriptor_t *
MFS_sysfs_serial(void)
{
	return serial;
}

extern MFS_descriptor_t *
MFS_sysfs_threads(void)
{
	return threads;
}

extern MFS_descriptor_t *
MFS_sysfs_heaps(void)
{
	return heaps;
}

