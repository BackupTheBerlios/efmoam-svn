#ifndef EFMOAM_H
#define EFMOAM_H

#define ETH_P_EFMOAM 		0x8809 /* To be moved in include/if_ether.h */ 
#define EFMOAM_DEST_MAC 	"\0x01\0x80\0xc2\0x00\0x00\0x02"
#define EFMOAM_SUBTYPE 		0x03 

#define EFMOAM_HASH_SHIFT	5
#define EFMOAM_HASH_SIZE	(1 << EFMOAM_HASH_SHIFT)
#define EFMOAM_HASH_MASK	(EFMOAM_HASH_SIZE  - 1)


struct efmoam_flags {
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u16 link_fault 	: 1;
	__u16 dying_gasp 	: 1;
	__u16 critcal_event 	: 1;
	__u16 local_discovery 	: 1;
	__u16 local_stable 	: 1;
	__u16 remote_discovery 	: 1;
	__u16 remote_stable 	: 1;
	__u16 reserved 		: 9;
#elif defined(__BIG_ENDIAN_BITFIELD)
	__u16 reserved 		: 9;
	__u16 remote_stable 	: 1;
	__u16  remote_discovery : 1;
	__u16 local_stable 	: 1;
	__u16 local_discovery 	: 1;
	__u16 critcal_event 	: 1;
	__u16 dying_gasp 	: 1;
	__u16 link_fault 	: 1;		
#else
#error	"Please fix <asm/byteorder.h>"
#endif
} __attribute__ ((packed));

enum efmoam_code {
	EFMOAM_INF 	= 0x00,
	EFMOAM_EVNT 	= 0x01,
	EFMOAM_VARREQ 	= 0x02,
	EFMOAM_VARRES 	= 0x03,
	EFMOAM_LOOP 	= 0x04,
	EFMOAM_OUI 	= 0xFE,
} __attribute__ ((packed));

struct efmoam_hdr {
	__u8 subtype;
	struct efmoam_flags flags;
	__u8 code;
};

struct inf_state {
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u8 rsvd 	: 1;
	__u8 parser	: 2;
	__u8 mux 	: 1;
	__u8 reserved	: 4;
#elif defined(__BIG_ENDIAN_BITFIELD)
	__u8 reserved	: 4; 
	__u8 mux 	: 1;
	__u8 parser	: 2;
	__u8 rsvd 	: 1;
#else
#error	"Please fix <asm/byteorder.h>"
#endif
} __attribute__ ((packed));

struct oam_cfg {
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u8 mode 	: 1;
	__u8 unidir 	: 1;
	__u8 lb 	: 1;
	__u8 events 	: 1;
	__u8 vars 	: 1;
	__u8 reserved 	: 3;
#elif defined(__BIG_ENDIAN_BITFIELD)
	__u8 reserved 	: 3;
	__u8 vars 	: 1;
	__u8 events 	: 1;
	__u8 lb 	: 1;
	__u8 unidir 	: 1;
	__u8 mode 	: 1;
#else
#error	"Please fix <asm/byteorder.h>"
#endif
} __attribute__ ((packed));

struct pdu_cfg {
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u8 max_pdusz 	: 3;
	__u8 reserved 	: 5;
#elif defined(__BIG_ENDIAN_BITFIELD)
	__u8 reserved 	: 5;
	__u8 max_pdusz	: 3;
#else 
#error 	"Please fix <asm/byteorder.h>"
#endif
	__u16 pdu_maxsz;
} __attribute__ ((packed));


struct inf_tlv {
	__u8 type;
	__u8 length;
	__u8 ver;
	__u16 rev;
	struct inf_state state;
	struct oam_cfg oam_cfg;
	struct pdu_cfg pdu_cfg;
	__u8 oui[3];
	__u8 vendor[4];
} __attribute__ ((packed));

/* general TLV markers */
#define ENDOFTLV = 0x00 /* end of tlv */
#define	OUISPEC  = 0xfe /* organization specific */

enum inf_tlv_marker {
	LOCALINF 	= 0x01, /* local information */
	REMOTEINF	= 0x02, /* remote information */
} __attribute__ ((packed));

struct event_tlv {
	__u16 	seqno;
	__u8  	tlv[40];
} __attribute__ ((packed));


enum event_tlv_marker {
	ESPE 	= 0x01,	/* Errored Symbol Period Event */	
	EFE 	= 0x02,	/* Errored Frame Event */
	EFPE 	= 0x03,	/* Errored Frame Period Event */
	EFSSE 	= 0x04,	/* Errored Frame Seconds Summary Event */
} __attribute__ ((packed));

struct espe_tlv {
	__u8 type;
	__u8 length;
	__u16 timestamp;
	__u64 window;
	__u64 threshold;
	__u64 errors;
	__u64 errtotal;
	__u32 eventtotal;
} __attribute__ ((packed));

struct efe_tlv {
	__u8 type;
	__u8 length;
	__u16 timestamp;
	__u16 window;
	__u32 threshold;
	__u32 errors;
	__u64 errtotal;
	__u32 eventtotal;	
} __attribute__ ((packed));

struct efpe_tlv {
	__u8 type;
	__u8 length;
	__u16 timestamp;
	__u32 window;
	__u32 threshold;
	__u32 errors;
	__u64 errtotal;
	__u32 eventtotal;	
} __attribute__ ((packed));

struct efsse_tlv {
	__u8 type;
	__u8 length;
	__u16 timestamp;
	__u16 window;
	__u16 threshold;
	__u16 errors;
	__u32 errtotal;
	__u32 eventtotal;	
} __attribute__ ((packed));

enum loopback_cmd {
	LOOP_EN = 0x01,		/* enable remote loopback */
	LOOP_DS = 0x02		/* disable remote loopback */
}  __attribute__ ((packed));

#endif
