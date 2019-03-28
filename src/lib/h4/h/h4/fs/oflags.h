#pragma once

#define O_RDONLY  (1<<8)
#define O_WRONLY  (1<<9)
#define O_RDWR    (O_RDONLY|O_WRONLY)
#define O_CREAT   (1<<10)
#define O_DIR     (1<<11)
#define O_APPEND  (1<<12)
#define O_NOTRUNC (1<<13)
