typedef struct
{
	int magic;
	int eop;
	int block_count;
	int file_size[8];
	char deleted[8];
	int file_name[8];
	int next;
} hdr;
