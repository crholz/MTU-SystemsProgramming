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

// init
// Initialize the values of hdr
// @passedHDR, the header
void init(hdr* passedHDR)
{
	// Initialize passedHDR
	int i;
	passedHDR->magic = 0x63746172;
	passedHDR->eop = sizeof(hdr);
	passedHDR->block_count = 0;
	for (i = 0; i < 8; i++) 
	{
		passedHDR->file_size[i] = 0;
		passedHDR->deleted[i] = 1;
		passedHDR->file_name[i] = 0;
	}

	passedHDR->next = 0;
}