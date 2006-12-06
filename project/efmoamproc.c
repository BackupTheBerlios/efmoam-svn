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
