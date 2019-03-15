#pragma once

#define F_NONE  0x0
#define F_PIPE  0x1
#define F_INODE 0x2

struct file
{
    uint8_t type;
    uint16_t ref;
    uint8_t readable;
    uint8_t writeable;
    //struct pipe *pipe;
    struct inode *ip;
    uint32_t off;
};

struct stat;

void ftable_init(void);
struct file *falloc(void);
struct file *fdup(struct file *fp);
int fread(struct file *fp, void *buf, size_t len);
int fwrite(struct file *fp, const void *buf, size_t len);
int fstat(struct file *fp, struct stat *st);
void fclose(struct file *fp);
