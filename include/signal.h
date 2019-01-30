#pragma once

enum SignalNumber
{
	SIGHUP = 1,
	SIGINT,
	SIGQUIT,
	SIGILL,
	SIGTRAP,
	SIGABRT,
	SIGBUS,
	SIGFPE,
	SIGKILL,
	SIGUSR1,
	SIGSEGV,
	SIGUSR2,
	SIGPIPE,
	SIGALRM,
	SIGTERM,
	SIGSTKFLT,
	SIGCHLD,
	SIGCONT,
	SIGSTOP,
	SIGTSTP,
	SIGTTIN,
	SIGTTOU,
	SIGURG,
	SIGXCPU,
	SIGXFSZ,
	SIGVTALRM,
	SIGPROF,
	SIGWINCH,
	SIGIO,
	SIGPWR,
	SIGSYS,
};

#define SIG_DFL ((sighandler_t)0)
#define SIG_IGN ((sighandler_t)-1)
#define SIG_ERR ((sighandler_t)-2)

typedef void (*sighandler_t)(int);

sighandler_t signal(int sig, sighandler_t handler);
int raise(int sig);
