
#define BLOCK_OFFSET(block) (block_size * (block))
#define BLOCK_SIZE 4096
#define _XOPEN_SOURCE 501
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>/*strlen*/

#include <unistd.h>
#include <sys/stat.h>   /*S_ISDIR*/
#include <sys/types.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>

typedef struct ext2_super_block sb_t;
typedef struct ext2_group_desc gd_t;
typedef struct ext2_inode inode_t;
typedef struct ext2_dir_entry de_t;

size_t block_size = sizeof(struct ext2_super_block);
int ram0 = 0;
size_t i = 0;
struct ext2_super_block *sb;
struct ext2_group_desc *gd;
unsigned char *bitmap;
struct ext2_inode *i_node;
struct ext2_dir_entry *de;
unsigned char block[BLOCK_SIZE];

sb_t *CreateSuperBlock()
{
    sb_t *superblock = malloc(sizeof(sb_t));
    return superblock;
}
gd_t *CreateGroupDesc()
{
    gd_t *group_desc = malloc(sizeof(gd_t));
    return group_desc;
}
inode_t *CreateInode()
{
    inode_t *node = malloc(sizeof(inode_t));
    return node;
}
de_t *CreateDirEntry()
{
    de_t *entry = malloc(sizeof(de_t));
    return entry;
}

void ReadSuperBlock(sb_t *sb)
{
    lseek(ram0, block_size, SEEK_SET);
    read(ram0, sb, block_size);
}
void ReadGroupDesc()
{
    lseek(ram0, block_size, SEEK_SET);
    read(ram0, gd, sizeof(gd_t));
}
void ReadInode(int num, int size)
{
    lseek(ram0, BLOCK_OFFSET(gd->bg_inode_table)+(num*size), SEEK_SET);
	read(ram0, i_node, sizeof(inode_t));
}
void ReadDirEntry()
{
    lseek(ram0, BLOCK_OFFSET(i_node->i_block[0]), SEEK_SET);
    read(ram0, block, block_size);
}


int main(int argc, char **argv)
{
    ram0 = open("/dev/ram0", O_RDONLY);

    sb = CreateSuperBlock();
    ReadSuperBlock(sb);

    if(ram0 < 0)
    {
        printf("ram0 id: %d\n\n" , ram0);
    }
/*  printf("number of blocks: %d\n", sb->s_blocks_count);
    printf("number of free blocks: %d\n", sb->s_free_blocks_count);
    printf("bloacks per group: %d\n", sb->s_blocks_per_group);
    printf("bloacks size: %d\n", sb->s_log_block_size);
    printf("number of i_node: %d\n", sb->s_inodes_count);
    printf("number of i_node per group: %d\n", sb->s_inodes_per_group);
    printf("i_node size: %d\n", sb->s_inode_size);
    printf("group descriptor size: %d\n\n", sb->s_desc_size);
    printf("size of super block struct: %ld\n", sizeof(sb_t));
    printf("size of group descriptor struct: %ld\n\n", sizeof(gd_t));  */

    block_size = 1024 << sb->s_log_block_size;

    gd = CreateGroupDesc();
    ReadGroupDesc();

/*     printf("free_blocks_count: %d\n", gd->bg_free_blocks_count);
    printf("free inodes count: %d\n", gd->bg_free_inodes_count);
    printf("inodes table: %d\n", gd->bg_inode_table);
    printf("block_bitmap: %d\n", gd->bg_block_bitmap);
    printf("inode_bitmap: %d\n", gd->bg_inode_bitmap);
    printf("flags: %d\n", gd->bg_flags);
    printf("used_dirs_count: %d\n\n", gd->bg_used_dirs_count); */

    i_node = CreateInode();
    ReadInode(2,sizeof(inode_t));

/*     printf("i_node->i_mode: %d \n", i_node->i_mode);
    printf("i_node->i_ctime: %d \n", i_node->i_ctime);
    printf("i_node->i_blocks: %d \n", i_node->i_blocks);
    printf("i_node->i_gid: %d \n", i_node->i_gid);
    printf("i_node->i_uid: %d \n", i_node->i_uid); */

    de = CreateDirEntry();
    if(S_ISDIR(i_node->i_mode) == 0)
    {
        printf("IS_DIR: %d\n", S_ISDIR(i_node->i_mode));
    }

    ReadDirEntry();
    de = (de_t *)(block);
    
/*     printf("de->name: %s\n", de->name);
    printf("de->name_len: %d\n", de->name_len);
    printf("de->rec_len: %d\n", de->rec_len);
    printf("i_node->i_size: %d\n\n", i_node->i_size); */

    while (de->name_len != 0)
    {
        printf("%10u %.*s\n", de->inode, de->name_len, de->name);
        de = (struct ext2_dir_entry *)((char*)de + de->rec_len);
    }

    ReadInode(93, sb->s_inode_size);
    ReadDirEntry();

    printf("%s\n", block);

    free(sb);
    free(gd);
    free(i_node);
/*     free(de); */

    return 0;
}