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


#include <linux/net.h>
#include <linux/if_ether.h>
#include <linux/string.h>
#include <linux/netdevice.h>

#include "efmoam.h" 


/* Our listing of NIC(s) */
static struct hlist_head efmoam_hash[EFMOAM_HASH_SIZE];

#define efmoam_hashfn(IDX)	((((IDX) >> EFMOAM_HASH_SHIFT) ^ (IDX)) & EFMOAM_HASH_MASK)


static int efmoam_rcv(struct sk_buff *skb, 
		struct net_device *dev,
		struct packet_type *pt,
		struct net_device *orig_dev)
{
	struct efmoam_hdr *ph;

	if (!pskb_may_pull(skb, sizeof(struct efmoam_hdr)))
		goto abort;

	if (!(skb = skb_share_check(skb, GFP_ATOMIC))) 
		goto out;
	

	if (memcmp(eth_hdr(skb)->h_dest, EFMOAM_DEST_MAC, ETH_ALEN)) {
		struct ethhdr *st = eth_hdr(skb);
		if (printk_ratelimit())
			printk(KERN_DEBUG"Got an unknown destination address %02X:%02X:%02X:%02X:%02X:%02X\n", 
		st->h_dest[0], st->h_dest[1], st->h_dest[2],
		st->h_dest[3], st->h_dest[4], st->h_dest[5]);		
		goto abort;
	}

	ph = (struct efmoam_hdr *) skb->nh.raw;

	switch (ph->code){
		case EFMOAM_INF:
			printk(KERN_DEBUG"Received an information pdu"); 
			break;
		case EFMOAM_EVNT:
			printk(KERN_DEBUG"Received an event notification pdu"); 
		 	break;	
		case EFMOAM_VARREQ:
			printk(KERN_DEBUG"Received an variable request pdu"); 
			break;
	}
	
	if (ph->subtype != EFMOAM_SUBTYPE) {
		if (printk_ratelimit())
			printk(KERN_DEBUG"Got an unknown EFMOAM subtype %02X\n", ph->subtype);
		goto abort;
	}

abort:
	kfree_skb(skb);
out:
	return NET_RX_SUCCESS; /* Lies... :-) */
}


static struct packet_type efmoam_ptype = {
	.type = __constant_htons(ETH_P_EFMOAM),
	.func = efmoam_rcv,	
}; 

static int efmoam_device_event(struct notifier_block *unused, unsigned long event, void *ptr)
{
	/* struct net_device *dev = ptr;*/
	char mess[20];
	/* It is OK that we do not hold the group lock right now,
	 * as we run under the RTNL lock.
	 */

	switch (event) {
	case NETDEV_CHANGE:
		strcpy(mess, "NETDEV_CHANGE");
		break;

	case NETDEV_DOWN:
		strcpy(mess,"NETDEV_DOWN");
		break;

	case NETDEV_UP:
		strcpy(mess,"NETDEV_UP");
		break;
		
	case NETDEV_UNREGISTER:
		strcpy(mess,"NETDEV_UNREGISTER");
		break;
	case NETDEV_GOING_DOWN:
		strcpy(mess,"NETDEV_GOING_DOWN");
		break;
	};

	printk("Received %s event notification.\n", mess);
	
	
	return NOTIFY_DONE;
}


static struct notifier_block efmoam_notifier_block = {
	.notifier_call = efmoam_device_event,
};



static int __init efmoam_init(void)
{
	
	int err = 0;

    err = efmoam_proc_init();

    if (err < 0) {
        printk(KERN_DEBUG "Could not register /proc/efmoam entry\n");
        return err;
    }


	printk(KERN_DEBUG"Registering efmoam ethertype.\n");
	dev_add_pack(&efmoam_ptype);
	
	/* Register us to receive netdevice events */
	err = register_netdevice_notifier(&efmoam_notifier_block);
	if (err < 0) {
		dev_remove_pack(&efmoam_ptype);
		return err;
	}


	return 0;
}

static void __exit efmoam_exit(void)
{
	printk(KERN_DEBUG"Unregistering efmoam ethertype\n");
	dev_remove_pack(&efmoam_ptype);

    printk(KERN_DEBUG"Unregistering the /proc/efmoam entries\n");
    efmoam_proc_cleanup();
#if 0	
	unregister_netdev(&efmoam_netdevice);
#endif 
}

module_init(efmoam_init);
module_exit(efmoam_exit);

MODULE_AUTHOR("Alex Militaru <amilitaru@batmsystems.ro>");
MODULE_DESCRIPTION("802.3ah protocol");
MODULE_LICENSE("GPL");


