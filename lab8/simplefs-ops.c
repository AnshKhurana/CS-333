#include "simplefs-ops.h"

extern struct filehandle_t file_handle_array[MAX_OPEN_FILES];
// Array for storing opened files

int simplefs_create(char *filename){
    /*
	    Create file with name `filename` from disk
	*/
	
	struct inode_t *iter_inode = (struct inode_t *) malloc(sizeof(struct inode_t));

	int samename=0;
	for (int i = 0; i < NUM_INODES; ++i)
	{
		simplefs_readInode(i, iter_inode);
		if (strcmp(filename, iter_inode->name)==0)
		{
			samename = 1;
			break;
		}	
	}


	if (samename)
		return -1;

	int freeInodeNumber = simplefs_allocInode();

	if (freeInodeNumber == -1)
	{
		return -1;
	}

	simplefs_readInode(freeInodeNumber, iter_inode);
	iter_inode->status = INODE_IN_USE;
	strcpy(iter_inode->name, filename);
	iter_inode->file_size = 0;
	for (int i = 0; i < MAX_FILE_SIZE; ++i)
	{
		iter_inode->direct_blocks[i] = -1;
	}

	simplefs_writeInode(freeInodeNumber, iter_inode);
	free(iter_inode);
    return freeInodeNumber;
}



// struct inode_t
// {
// 	int status;								// INODE_FREE if free, INODE_IN_USE if used
// 	char name[MAX_NAME_STRLEN];					// name of the file
// 	int file_size;								// size of the file in bytes
// 	int direct_blocks[MAX_FILE_SIZE];			// -1 if free, block number if used
// };

// struct filehandle_t
// {
// 	int offset;		  // current offset in opened file
// 	int inode_number; // Inode number for the file
// };


void simplefs_delete(char *filename){
    /*
	    delete file with name `filename` from disk
	*/
	struct inode_t *iter_inode = (struct inode_t *) malloc(sizeof(struct inode_t));
	int found = 0;
	int inode_num=-1;
	for (int i = 0; i < NUM_INODES; ++i)
	{
		simplefs_readInode(i, iter_inode);
		if (strcmp(filename, iter_inode->name)==0)
		{
			found = 1;
			inode_num = i;
			break;
		}	
	}

	if (!found)
	{
		/* code */
		printf("Error, deleting a file that does not exist.\n");
	}
	else
	{	
		for (int i = 0; i < MAX_FILE_SIZE; ++i)
		{
			if (iter_inode->direct_blocks[i] != -1)
			{
				simplefs_freeDataBlock(iter_inode->direct_blocks[i]);
			}
		}

		simplefs_freeInode(inode_num);
		free(iter_inode);
	}
	return;
}
		
int simplefs_open(char *filename){
    /*
	    open file with name `filename`
	*/
 	
 	struct inode_t *iter_inode = (struct inode_t*) malloc(sizeof(struct inode_t));
	int found = 0;
	int inode_num=-1;
	for (int i = 0; i < NUM_INODES; ++i)
	{
		simplefs_readInode(i, iter_inode);
		if ((strcmp(filename, iter_inode->name)==0) && (iter_inode->status == INODE_IN_USE))
		{
			found = 1;
			// printf("file found\n");
			inode_num = i;

			break;
		}	
	}

	if (found)
	{
		for (int i = 0; i < MAX_OPEN_FILES; ++i)
		{
			if (file_handle_array[i].inode_number == -1)
			{
				file_handle_array[i].inode_number = inode_num;
				file_handle_array[i].offset = 0;
				return i;
			}
		}
	}

    return -1;
}

void simplefs_close(int file_handle){
    /*
	    close file pointed by `file_handle`
	*/

	file_handle_array[file_handle].inode_number = -1;
	file_handle_array[file_handle].offset = 0;
}

int simplefs_read(int file_handle, char *buf, int nbytes){
    /*
	    read `nbytes` of data into `buf` from file pointed by `file_handle` starting at current offset
	*/
	int inode_number = file_handle_array[file_handle].inode_number;
	int curr_offset = file_handle_array[file_handle].offset;

	// printf("Reading inode: %d\n", inode_number);
	// printf("Reading from offset: %d\n", curr_offset);
	struct inode_t *file_inode = (struct inode_t *) malloc(sizeof(struct inode_t));

	simplefs_readInode(inode_number, file_inode);
	// printf("No seg fault yet \n");
	int file_size = file_inode->file_size;

	if (curr_offset + nbytes >  file_size)
	{
		buf = NULL;
		return -1;
	}

	int block_num = curr_offset / BLOCKSIZE;

	int offset_in_block = curr_offset % BLOCKSIZE;

	int available_to_read = (BLOCKSIZE - offset_in_block);
	
	while(available_to_read < nbytes)
	{
		char *blockdata = (char*) malloc(BLOCKSIZE);
		simplefs_readDataBlock(file_inode->direct_blocks[block_num], blockdata);
		// blockdata = blockdata + offset_in_block;
		memcpy(buf, blockdata+offset_in_block, available_to_read);

		block_num++;
		offset_in_block = 0;
		buf = buf + available_to_read;
		nbytes = nbytes - available_to_read;
		available_to_read = BLOCKSIZE - offset_in_block;
		free(blockdata);
	}

	char *blockdata = (char *) malloc(BLOCKSIZE);
	simplefs_readDataBlock(file_inode->direct_blocks[block_num], blockdata);
	// blockdata = blockdata + offset_in_block;
	memcpy(buf, blockdata+offset_in_block, nbytes);
	buf[nbytes] = '\0';

	free(blockdata);	

	return 0;
    
}


int simplefs_write(int file_handle, char *buf, int nbytes){
    /*
	    write `nbytes` of data from `buf` to file pointed by `file_handle` starting at current offset
	*/
	// Also update the file size in inode. 
	// need to get new DataBlocks too.

	int first_time = 0;
 	int curr_offset = file_handle_array[file_handle].offset;
	int inode_number = file_handle_array[file_handle].inode_number;

	// printf("Writing file with inode_num: %d\n", inode_number);
	struct inode_t *file_inode = (struct inode_t*) malloc(sizeof(struct inode_t));
	simplefs_readInode(inode_number, file_inode);

	int file_size = file_inode->file_size;
	// printf("Detected file size: %d\n", file_size);
	if  (curr_offset + nbytes > MAX_FILE_SIZE * BLOCKSIZE)
	 {
	 	return -1;
	 }

	if (curr_offset + nbytes > file_size)
	{
		file_size = curr_offset + nbytes;
		file_inode->file_size = file_size;
	}


	int block_num = curr_offset / BLOCKSIZE;
	int offset_in_block = curr_offset % BLOCKSIZE;

	int available_to_write = (BLOCKSIZE - offset_in_block);

	int bytes_written = 0;
	// printf("%d, %d\n", available_to_write, curr_offset);
	// printf("available_to_write: %d\n", available_to_write);
 	while(nbytes > available_to_write)
 	{
 		// printf("Should not be inside for loop \n");
 		char* blockdata = (char *) malloc(BLOCKSIZE);
 		if (file_inode->direct_blocks[block_num] == -1)
 		{
 			
 			int new_block_number = simplefs_allocDataBlock();
 			if (new_block_number < 0)
 				return -1;
 			file_inode->direct_blocks[block_num] = new_block_number;  
 			first_time = 1;
 		}

 		if (!first_time)
 		{
 			// printf("Reading original data\n");
 			simplefs_readDataBlock(file_inode->direct_blocks[block_num], blockdata);
 		}
 		// printf("offset_in_block: %d\n", offset_in_block);
 		// blockdata = blockdata + offset_in_block;
 		buf  = buf + bytes_written;
 		memcpy(blockdata+offset_in_block, buf, available_to_write);
 		simplefs_writeDataBlock(file_inode->direct_blocks[block_num], blockdata);
 		first_time = 0;

 		bytes_written = bytes_written + available_to_write;
 		nbytes = nbytes - available_to_write;
 		offset_in_block = 0;
 		block_num++;
 		available_to_write = BLOCKSIZE - offset_in_block;
 		free(blockdata);
 	}

 	// printf("Succesfully outside for loop\n");
	char* blockdata = (char *) malloc(BLOCKSIZE);

	if (file_inode->direct_blocks[block_num] == -1)
	{
		int new_block_number = simplefs_allocDataBlock();
		// printf("Block alloted %d\n", new_block_number);
		if (new_block_number < 0)
			return -1;
		file_inode->direct_blocks[block_num] = new_block_number;  
		first_time = 1;
	}

	if (!first_time)
	{
		simplefs_readDataBlock(file_inode->direct_blocks[block_num], blockdata);
	}

	// blockdata = blockdata + offset_in_block;
	buf  = buf + bytes_written;
	memcpy(blockdata+offset_in_block, buf, nbytes);
	simplefs_writeDataBlock(file_inode->direct_blocks[block_num], blockdata);
	first_time = 0;
	free(blockdata);

	simplefs_writeInode(inode_number, file_inode);
	return 0;
}


int simplefs_seek(int file_handle, int nseek){
    /*
	   increase `file_handle` offset by `nseek`
	*/

	int curr_offset = file_handle_array[file_handle].offset;
	int inode_number = file_handle_array[file_handle].inode_number;

	struct inode_t *file_inode = (struct inode_t*) malloc(sizeof(struct inode_t)); 

	simplefs_readInode(inode_number, file_inode);

	int file_size = file_inode->file_size;
	// printf("File size: %d\n", file_size);
	if ((curr_offset + nseek >= 0) && (curr_offset + nseek <= file_size))
	{
		curr_offset = curr_offset + nseek;
		file_handle_array[file_handle].offset = curr_offset;
		// printf("New offset value: %d\n", curr_offset);
		return 0;
	}
    return -1;
}