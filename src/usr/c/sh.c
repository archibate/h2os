#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <spawn.h>
#include <errno.h>
#define BLANK " \n\t\r"

struct cmdent {
	struct cmdent *prev;

	char *argv[MAX_ARGV];
	int argc;

	struct spawnattr sat;
};

static struct cmdent *cmds_head;
static void cmdl_parse(char *);
static void cmd_add(char *);
static void cmds_init(void);
static void cmds_exec(void);
static void argv_add(char *);
static void argv_init(void);
static void argv_done(void);
static void argv_exec(void);
static int do_execute(char **argv, struct spawnattr *sat);
void sat_init(struct spawnattr *sat);
void sat_destroy(struct spawnattr *sat);
void sat_pipe(struct spawnattr *satw, struct spawnattr *satr);
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
		cmds_exec();
	}
	return 0;
}

void cmdl_parse(char *cmdl)
{
	char *ep;
	cmds_init();
	goto go;
	do {
		cmdl = ep + 1;
go:		if ((ep = strchrin(cmdl, "|")))
			*ep = 0;
		cmd_add(cmdl);
	} while (ep);
}

void cmds_init(void)
{
	cmds_head = NULL;
}

void cmds_exec(void)
{
	struct cmdent *curr;
	while ((curr = cmds_head)) {
		cmds_head = curr->prev;
		printk("cmds_exec: %s", curr->argv[0]);
		do_execute(curr->argv, &curr->sat);
		sat_destroy(&curr->sat);
		//free(curr);
	}
	wait();
}

#if 0
void cmd_takred(char *cmd)
{
	int n;
	char *p = cmd, *path;
	while ((p = strchrin(p, "<>"))) {
		if (*p == '>') {
			n = 1;
		} else {
			n = 0;
		}
		*p = 0;
		if (cmd != p && '0' <= p[-1] && p[-1] <= '9')
			n = p[-1] - '0';
		path = strskipin(p, BLANK);
		if ((p = strchrin(path, BLANK)))
			*p++ = 0;
		takred_add(n, path);
	}
}
#endif

void cmd_add(char *cmd)
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
	cmds_head->argv[cmds_head->argc++] = arg;
}

void argv_init(void)
{
	struct cmdent *prev = cmds_head;
	cmds_head = malloc(sizeof(struct cmdent));
	cmds_head->prev = prev;

	cmds_head->argv[0] = NULL;
	cmds_head->argc = 0;

	sat_init(&cmds_head->sat);
	if (prev != NULL)
		sat_pipe(&prev->sat, &cmds_head->sat);
}

void argv_done(void)
{
	cmds_head->argv[cmds_head->argc] = NULL;
	printk("argv_done: %s", cmds_head->argv[0]);
}

int do_execute(char **argv, struct spawnattr *sat)
{
	if (!argv[0])
		return 0;

	pid_t pid = spawn(argv[0], argv, NULL, sat);
	if (pid < 0) {
		if (pid == -ENOENT) {
			fprintf(stderr, "%s: command not found\n", argv[0]);
		} else {
			errno = -pid;
			perror(argv[0]);
		}
		return pid;
	}
	return 0;
}

void sat_init(struct spawnattr *sat)
{
	int i;
	for (i = 0; i < 3; i++)
		sat->stdio[i] = i;
}

void sat_pipe(struct spawnattr *satw, struct spawnattr *satr)
{
	int fd[2];
	pipe(fd);
	//printk("fd={%d,%d}", fd[0], fd[1]);
	satr->stdio[0] = fd[0];
	satw->stdio[1] = fd[1];
}

void sat_destroy(struct spawnattr *sat)
{
	int i;
	for (i = 0; i < 3; i++)
		if (sat->stdio[i] >= 3)
			close(sat->stdio[i]);
}
