#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>

static void
handler(int sig, siginfo_t *si, void *unused)
{
    printf("Got SIGSEGV at address: %p\n",
            si->si_addr);
    exit(EXIT_FAILURE);
}

int main()
{
    char *buffer;
    char *p;
    int pagesize;
    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = handler;
    sigaction(SIGSEGV, &sa, NULL);

    pagesize = sysconf(_SC_PAGE_SIZE);
    printf("Pagesize is:                      %x\n", pagesize);

    buffer = mmap(0, 4 * pagesize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    printf("Start of region:        %p\n", buffer);

    mprotect(buffer + pagesize * 2, pagesize, PROT_NONE);

    for (p = buffer ; ; )
        *(p++) = 'a';

    printf("Loop completed\n");     /* Should never happen */
    return 0;
}
