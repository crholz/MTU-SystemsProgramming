#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

void handler(int receivedSig) {
	printf("Handler caught sig <%d>\n",receivedSig);
	fflush(stdout);
	//_exit(1);
}

int main (int argc, char **argv) {
	sigset_t termMask;
	sigset_t intMask;
	sigset_t oldMask;
	sigset_t pendingMask;

	signal(SIGTERM, handler);
	signal(SIGINT, handler);

	sigemptyset(&termMask);
	sigaddset(&termMask, SIGTERM);
	sigemptyset(&intMask);
	sigaddset(&intMask, SIGINT);

	sigprocmask(SIG_BLOCK, &intMask, &oldMask);
	printf("SIGINT Blocked. Process <%d> sleeping.\n", getpid());
	fflush(stdout);
	sleep(10);

	sigpending(&pendingMask);
	if (sigismember(&pendingMask, SIGINT)) {
		printf("SIGINT pending. Ready to block TERM.\n");
		fflush(stdout);
	}

	sigprocmask(SIG_BLOCK, &termMask, NULL);
	printf("SIGTERM blocked. Sleeping.\n"); fflush(stdout);
	sleep(10);

	printf("Unblocking.\n"); fflush(stdout);

	sigprocmask(SIG_UNBLOCK, &termMask, NULL);
	sigprocmask(SIG_SETMASK, &oldMask, NULL);
	sleep(30);
}
