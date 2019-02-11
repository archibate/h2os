#pragma once

// http://blog.sina.com.cn/s/blog_48ed03c801016pp7.html
#define ovadd(x, y)      ((x) + (y) < (x))
#define ov_or_add(x, y)   ovadd(x, y) || ((x) + (y))
#define ov_and_add(x, y)  ovadd(x, y) && ((x) + (y))
#define nov_or_add(x, y)  !ovadd(x, y) || ((x) + (y))
#define nov_and_add(x, y) !ovadd(x, y) && ((x) + (y))
