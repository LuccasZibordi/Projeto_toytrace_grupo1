#include "trace_runtime.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

#if !defined(__x86_64__)
#error "Este runtime didatico suporta apenas Linux x86_64."
#endif

static void fill_event_from_regs(pid_t pid,
                                 int entering,
                                 const struct user_regs_struct *regs,
                                 struct syscall_event *ev)
{
    /*
     * TODO Semana 4:
     *
     * Preencha struct syscall_event usando os registradores x86_64.
     *
     * Dicas:
     * - regs->orig_rax contem o numero da syscall.
     * - regs->rax contem o retorno, valido na saida.
     * - os seis argumentos ficam em rdi, rsi, rdx, r10, r8 e r9.
     * - ev->entering deve copiar o parametro entering.
     */
    memset(ev, 0, sizeof(*ev));
    ev->pid = pid;
    ev->entering = entering;
}

static pid_t launch_tracee(char *const argv[])
{
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    } else if (pid == 0) {
        printf("dentro do filho");
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        raise(SIGSTOP);
        execvp(argv[0], argv);
        perror("falha execvp");
        _exit(1);
    }
    return pid;
}

static int wait_for_initial_stop(pid_t child)
{

    int status;

    if (waitpid(child, &status, 0) < 0) {
        fprintf(stderr, "erro no waitpid");
        return -1;
    };

    if (!WIFSTOPPED(status)) {
        fprintf(stderr, "erro generico");
        return -1;
    }

    if (WSTOPSIG(status) != SIGSTOP) {
        fprintf(stderr, "erro generico");
        return -1;
    }
  
    return 0;
}

static int configure_trace_options(pid_t child)
{
    long ret;
ret = ptrace(PTRACE_SETOPTIONS, child, NULL, PTRACE_O_TRACESYSGOOD);
if (ret < 0 ){
    perror("erro ptrace PTRACE_SETOPTIONS");
    return -1;

} else{
    return 0;
}

}

static int resume_until_next_syscall(pid_t child, int signal_to_deliver)
{
      long ret;
    ret = ptrace(PTRACE_SYSCALL, child, 0, signal_to_deliver);

    if (ret < 0){
        return -1;
    };

    return 0;
}

static int wait_for_syscall_stop(pid_t child, int *status)
{
if (waitpid(child, status,0)<0)
    {
        perror("erro");
        return -1;
    }

    if(WIFEXITED(*status) || WIFSIGNALED(*status))
    {
        return 0;
    }


    if(wifstopped(*status)== (SIGTRAP | 0x80))
    {
        return 1;
    }

    return 0;
}

int trace_program(char *const argv[],
                  trace_observer_fn observer,
                  void *userdata)
{
    pid_t child;
    int status = 0;
    int entering = 1;

    if (argv == NULL || argv[0] == NULL) {
        fprintf(stderr, "erro: programa alvo ausente\n");
        return -1;
    }

    child = launch_tracee(argv);
    if (child < 0) {
        return -1;
    }

    if (wait_for_initial_stop(child) < 0) {
        return -1;
    }

    if (configure_trace_options(child) < 0) {
        return -1;
    }

    if (resume_until_next_syscall(child, 0) < 0) {
        return -1;
    }

    while (1) {
        struct user_regs_struct regs;
        struct syscall_event ev;
        int stop_kind;

        stop_kind = wait_for_syscall_stop(child, &status);
        if (stop_kind < 0) {
            return -1;
        }
        if (stop_kind == 0) {
            if (WIFEXITED(status)) {
                return WEXITSTATUS(status);
            }
            if (WIFSIGNALED(status)) {
                return 128 + WTERMSIG(status);
            }
            return 0;
        }

        /*
         * TODO Semana 4:
         *
         * Use PTRACE_GETREGS para preencher regs.
         * Depois chame fill_event_from_regs() e observer().
         */
        memset(&regs, 0, sizeof(regs));
        fill_event_from_regs(child, entering, &regs, &ev);
        if (observer != NULL) {
            observer(&ev, userdata);
        }

        entering = !entering;

        if (resume_until_next_syscall(child, 0) < 0) {
            return -1;
        }
    }
}
