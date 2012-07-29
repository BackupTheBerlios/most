#include "uso/thread.h"
#include "uso/heap.h"
#include "mfs/sysfs.h"
#include "mfs/descriptor.h"
#include "mfs/super.h"
#include "mfs/directory.h"
#include "mfs/stream.h"

static MFS_descriptor_t *root = NULL;
static MFS_descriptor_t *sys = NULL;
static MFS_descriptor_t *usr = NULL;
static MFS_descriptor_t *uso = NULL;
static MFS_descriptor_t *threads = NULL;
static MFS_descriptor_t *heaps = NULL;
static MFS_descriptor_t *dev = NULL;
static MFS_descriptor_t *serial = NULL;
static MFS_descriptor_t *timer = NULL;
static MFS_descriptor_t *digio = NULL;
static MFS_descriptor_t *mm = NULL;
static MFS_descriptor_t *net = NULL;
static MFS_descriptor_t *netif = NULL;
static MFS_descriptor_t *cli = NULL;
static MFS_descriptor_t *cmd = NULL;

static struct MFS_super_op MFS_sysfs_super_op = {.mount = NULL,
    .umount = NULL
};

static struct MFS_directory_op MFS_sysfs_directory_op = {.open = NULL,
    .close = NULL,
    .create = NULL,
    .remove = NULL,
    .rename = NULL
};

static struct MFS_stream_op MFS_sysfs_stream_op = {.open = NULL,
    .close = NULL,
    .info = NULL,
    .read = MFS_empty_read,
    .write = MFS_empty_write,
    .seek = NULL,
    .flush = NULL
};

static struct MFS_vfs_op MFS_sysfs_vfs_op = {.super_op = &MFS_sysfs_super_op,
    .dir_op = &MFS_sysfs_directory_op,
    .stream_op = &MFS_sysfs_stream_op
};


static void
info (void)
{
    ACE_puts ("sysfs\n");
}

extern ACE_bool_t
MFS_sysfs_init (void)
{
    root = MFS_mount ("root", &MFS_sysfs_vfs_op, info);
    if (root == NULL)
        return FALSE;
    sys = MFS_create_dir (root, "sys");
    usr = MFS_create_dir (root, "usr");
    if (sys != NULL)
    {
        if ((uso = MFS_create_dir (sys, "uso")) != NULL)
        {
            threads = MFS_create_dir (uso, "threads");
            heaps = MFS_create_dir (uso, "heaps");
        }
        if ((dev = MFS_create_dir (sys, "dev")) != NULL)
        {
            serial = MFS_create_dir (dev, "serial");
            timer = MFS_create_dir (dev, "timer");
            digio = MFS_create_dir (dev, "digio");
            mm = MFS_create_dir (dev, "mm");
        }
        if ((net = MFS_create_dir (sys, "net")) != NULL)
        {
            netif = MFS_create_dir (net, "netif");
        }
        if ((cli = MFS_create_dir (sys, "cli")) != NULL)
        {
            cmd = MFS_create_dir (cli, "cmd");
        }
    }
    return usr && netif && serial && timer && digio && mm && heaps && threads && cli && cmd;
}

extern MFS_descriptor_t *
MFS_sysfs_get_dir (enum MFS_sysfs_dir dir)
{
    MFS_descriptor_t *d = NULL;
    switch (dir)
    {
    case MFS_SYSFS_DIR_ROOT:
        d = root;
        break;
    case MFS_SYSFS_DIR_SYS:
        d = sys;
        break;
    case MFS_SYSFS_DIR_USR:
        d = usr;
        break;
    case MFS_SYSFS_DIR_USO:
        d = uso;
        break;
    case MFS_SYSFS_DIR_THREADS:
        d = threads;
        break;
    case MFS_SYSFS_DIR_HEAPS:
        d = heaps;
        break;
    case MFS_SYSFS_DIR_DEV:
        d = dev;
        break;
    case MFS_SYSFS_DIR_SERIAL:
        d = serial;
        break;
    case MFS_SYSFS_DIR_TIMER:
        d = timer;
        break;
    case MFS_SYSFS_DIR_DIGIO:
        d = digio;
        break;
    case MFS_SYSFS_DIR_MM:
        d = mm;
        break;
    case MFS_SYSFS_DIR_NET:
        d = net;
        break;
    case MFS_SYSFS_DIR_NETIF:
        d = netif;
        break;
    case MFS_SYSFS_DIR_CLI:
        d = cli;
        break;
    case MFS_SYSFS_DIR_CMD:
        d = cmd;
        break;
    default:
        d = root;
        break;
    }
    return d;
}
