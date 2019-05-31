#include <stdio.h>

void copy(const char *src, const char *dst)
{
	FILE *fin = fopen(src, "r");
	if (!fin) { perror(src); return; }
	FILE *fout = fopen(dst, "w+");
	if (!fout) { perror(dst); fclose(fin); return; }
	char buf[1024];
	/*while (fgets(buf, sizeof(buf), fin))
		fputs(buf, fout);*/
	ssize_t ret;
	while (0 < (ret = fread(buf, 1, sizeof(buf), fin))) {
		//printk("(%02X) ret=%d", (unsigned char) buf[0], ret);
		fwrite(buf, ret, 1, fout);
	}
	if (ferror(fin)) perror(src);
	if (ferror(fout)) perror(dst);
	fclose(fout);
	fclose(fin);
}

int main(int argc, char **argv)
{
	if (!argv[1] || !argv[2]) {
		fprintf(stderr, "cp: bad argument\n");
		return 1;
	}
	copy(argv[1], argv[2]);
	return 0;
}
