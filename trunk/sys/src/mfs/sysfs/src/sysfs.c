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
static MFS_descriptor_t *net = NULL;
static MFS_descriptor_t *netif = NULL;
static MFS_descriptor_t *heaps = NULL;
static MFS_descriptor_t *cli = NULL;

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
	ACE_puts("sysfs\n");
}

extern ACE_bool_t
MFS_sysfs_init(void)
{
	MFS_descriptor_t *dir;
	root = MFS_mount("root", &MFS_sysfs_vfs_op, info);
    if (root != NULL){
    	if ( (net = MFS_create_dir(root, "net")) != NULL){
			netif = MFS_create_dir(net, "netif");
    	}
	    if ( (dir = MFS_create_dir(root, "dev")) != NULL){
			serial = MFS_create_dir(dir, "serial");
	    }
		if ( (dir = MFS_create_dir(root, "uso")) != NULL){
			threads = MFS_create_dir(dir, "threads");
			heaps = MFS_create_dir(dir, "heaps");
		}
		cli = MFS_create_dir(root, "cli");
    }
	return root && netif && serial && heaps && threads && cli;    
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

extern MFS_descriptor_t *
MFS_sysfs_net(void)
{
	return net;
}

extern MFS_descriptor_t *
MFS_sysfs_netif(void)
{
	return netif;
}

extern MFS_descriptor_t *
MFS_sysfs_cli(void)
{
	return cli;
}
