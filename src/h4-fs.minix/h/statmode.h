// https://github.com/archibate/OS67/blob/master/inc/stat.h
#pragma once

/* - - - - - - - | - - - - - - - - -
 * r d   p         r w x r w x r w x
 */
/*ref: http://minix1.woodhull.com/manpages/man2/stat.2.html */
/* http://faculty.qu.edu.qa/rriley/cmpt507/minix/stat_8h-source.html#l00016 */

/* octal */
#define S_IFMT  0170000    /* type of file */
#define S_IFIFO 0010000    /* named pipe */
#define S_IFCHR 0020000    /* character special */
#define S_IFDIR 0040000    /* directory */
#define S_IFBLK 0060000    /* block special */
#define S_IFREG 0100000    /* regular */
#define S_IFLNK 0120000    /* symbolic link (Minix-vmd) */

#define S_SUID  04000      /* set user-id on execution */
#define S_SGID  02000      /* set group-id on execution */
#define S_SVTX  01000      /* limited deletion */

#define S_RWX   0777       /* default mode */

#define S_ISREG(m)      (((m) & S_IFMT) == S_IFREG)     /* is a reg file */
#define S_ISDIR(m)      (((m) & S_IFMT) == S_IFDIR)     /* is a directory */
#define S_ISCHR(m)      (((m) & S_IFMT) == S_IFCHR)     /* is a char spec */
#define S_ISBLK(m)      (((m) & S_IFMT) == S_IFBLK)     /* is a block spec */
#define S_ISLNK(m)      (((m) & S_IFMT) == S_IFLNK)     /* is a symlink */
#define S_ISFIFO(m)     (((m) & S_IFMT) == S_IFIFO)     /* is a pipe/FIFO */
