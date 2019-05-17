#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <spawn.h>
#include <errno.h>
#define BLANK " \n\t\r"

static int argc;
static char *argv[MAX_ARGV];
static void cmdl_parse(char *cmd);
static void argv_add(char *arg);
static void argv_init(void);
static void argv_done(void);
static void argv_exec(void);
static int do_execute(void);
int main(int _argc, char **_argv)
{
	FILE *fin = stdin;
	if (_argv[1]) {
		fin = fopen(_argv[1], "r");
		if (!fin) {
			perror(_argv[1]);
			return 1;
		}
	}
	static char buf[1024];
	while (1) {
		if (fin == stdin) {
			printf("sh> ");
			fflush(stdout);
		}
		if (!fgets(buf, sizeof(buf), fin))
			break;
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
	if (argv[0])
		do_execute();
}

#define wait4(pid) wait() //T
int do_execute(void)
{
	pid_t pid = spawn(argv[0], argv, NULL, NULL);
	if (pid < 0) {
		if (pid == -ENOENT) {
			fprintf(stderr, "%s: command not found\n", argv[0]);
		} else {
			errno = -pid;
			perror(argv[0]);
		}
		return pid;
	}
	return wait4(pid);
}
