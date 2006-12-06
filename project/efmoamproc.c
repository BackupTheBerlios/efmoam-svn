/* IEEE Std 802.3ah 
 * Ethernet in the First Mile Operations, Administration and Maintenance
 * 
 * Author: Alexandru Ioan Militaru <amilitaru@batmsystems.ro>
 * Contributors:
 *
 * License:
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 */

#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>


static void *efmoam_seq_start(struct seq_file *seq, loff_t *pos);
static void *efmoam_seq_next(struct seq_file *seq, void *v, loff_t *pos);
static void efmoam_seq_stop(struct seq_file *seq, void *v);
static int efmoam_seq_show(struct seq_file *seq, void *v);

static const char name_root[]	 = "efmoam";

/*
 *	/proc/net/efmoam 
 */

static struct proc_dir_entry *proc_efmoam_dir;


/*
 *	/proc/net/efmoam/config 
 */

static struct proc_dir_entry *proc_efmoam_conf;

static const char name_conf[]	 = "config";

static struct seq_operations efmoam_seq_ops = {
	.start = efmoam_seq_start,
	.next = efmoam_seq_next,
	.stop = efmoam_seq_stop,
	.show = efmoam_seq_show,
};

static int efmoam_seq_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &efmoam_seq_ops);
}

static struct file_operations efmoam_fops = {
	.owner	 = THIS_MODULE,
	.open    = efmoam_seq_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release,
};


static void *efmoam_seq_start(struct seq_file *seq, loff_t *pos)
{
    return NULL;
}

static void *efmoam_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
    return NULL;
}

static void efmoam_seq_stop(struct seq_file *seq, void *v)
{
   
}

static int efmoam_seq_show(struct seq_file *seq, void *v)
{
    return 0;
}

void efmoam_proc_cleanup(void)
{
    if (proc_efmoam_conf)
		remove_proc_entry(name_conf, proc_efmoam_dir);

	if (proc_efmoam_dir)
		proc_net_remove(name_root);
}

int __init  efmoam_proc_init(void)
{
	proc_efmoam_dir = proc_mkdir(name_root, proc_net);
	if (proc_efmoam_dir) {
		proc_efmoam_conf = create_proc_entry(name_conf,
						   S_IFREG|S_IRUSR|S_IWUSR,
						   proc_efmoam_dir);
		if (proc_efmoam_conf) {
			proc_efmoam_conf->proc_fops = &efmoam_fops;
			return 0;
		}
	}
	efmoam_proc_cleanup();
	return -ENOBUFS;
}


