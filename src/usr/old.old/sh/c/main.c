#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define BLANK " \n\t\r"

static int argc;
static char *argv[MAX_ARGV];
static void cmdl_parse(char *cmd);
static void argv_add(char *arg);
static void argv_init(void);
static void argv_done(void);
static void argv_exec(void);
int main(int _argc, char **_argv)
{
	static char buf[1024];
	while (1) {
		printf("sh> ");
		fflush(stdout);
		fgets(buf, sizeof(buf), stdin);
		cmdl_parse(buf);
		argv_exec();
	}
	return 0;
}

void cmdl_parse(char *cmd)
{
	char *ep;
	argv_init();
	while (*(cmd = strskipin(cmd, BLANK))) {
		ep = strchrin(cmd, BLANK);
		if (ep) *ep = 0;
		argv_add(cmd);
		if (ep) cmd = ep + 1;
		else break;
	}
	argv_done();
}

void argv_add(char *arg)
{
	argv[argc++] = arg;
}

void argv_init(void)
{
	argv[0] = NULL;
	argc = 0;
}

void argv_done(void)
{
	argv[argc] = NULL;
}

void argv_exec(void)
{
	if (!fork()) {
		printk("child %s", argv[0]);
		execv(argv[0], argv);
	} else {
		printk("parent wait");
		wait();
	}
}
