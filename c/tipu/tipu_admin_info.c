#include <stdio.h>
#include <stdlib.h>

typedef unsigned char		    uint8_t;
typedef unsigned short int	    uint16_t;
typedef unsigned int		    uint32_t;
typedef unsigned long int	    uint64_t;
typedef unsigned long long      khint64_t;

#define MAX_DESC_PER_ADMIN 253

struct virtio_blk_outhdr {
	/* VIRTIO_BLK_T* */
	uint32_t type;
	/* io priority. */
	uint32_t ioprio;
	/* Sector (ie. 512 byte offset) */
	uint64_t sector;
};

struct virtio_desc
{
    uint64_t addr;
    uint32_t length;
    uint16_t buf_id;
    uint16_t flag;
};

struct virtio_admin_info_desc
{
    union {
		/* 32B */
		struct virtio_vfs_outhdr {
			uint32_t len;
			uint32_t opcode;
			uint64_t unique;
			uint64_t nodeid;
			uint32_t uid;
			uint32_t gid;
		} vfs_hdr;
		/* 16B */
		struct virtio_blk_outhdr blk_hdr;
	} virtio_hdr;
    uint32_t compact_descs;
    uint32_t total_descs;
    uint16_t host_desc_idx:15;
    uint16_t host_desc_wrap:1;
    uint8_t stat;
    uint8_t flag_l;
    uint8_t flag_h;
    uint8_t buf_id_l;
    uint8_t buf_id_h;
    uint8_t rsv1;
    struct virtio_desc descs[MAX_DESC_PER_ADMIN];
};


// gcc -o main main.c;./main
int main(){
    struct virtio_admin_info_desc virtio_admin;

    printf("virtio_admin size:%lu\n", sizeof(virtio_admin));

    return 0;
}




