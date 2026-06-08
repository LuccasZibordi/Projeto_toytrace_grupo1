#include "student_api.h"
#include "syscall_names.h"
#include "trace_helpers.h"
#include <stdio.h>
#include <sys/syscall.h>
void student_debug_raw_event(const struct syscall_event *ev, char *buf,
                             size_t bufsz) {
  if (ev->entering) {
    snprintf(
        buf, bufsz,
        "pid=%d syscall=%s entrada argumentos(%#lx, %#lx, %#lx, %#lx, %#lx)",
        ev->pid, syscall_name(ev->syscall_no), ev->args[0], ev->args[1],
        ev->args[2], ev->args[3], ev->args[4]);
  } else {
    snprintf(buf, bufsz, "pid=%d syscall=%s saida retorno=%ld", ev->pid,
             syscall_name(ev->syscall_no), ev->ret);
  }
}

void student_format_event(const struct syscall_event *ev, char *buf,
                          size_t bufsz) {
  char path_buf[256];
                            if (ev->entering) {
        return; 
    }
switch (ev->syscall_no) {
  case SYS_read:
    snprintf(buf, bufsz, "read(%ld, %p, %lu) = %ld", ev->args[0],
             (void *)ev->args[1], ev->args[2], ev->ret);
    break;

  case SYS_write:
    snprintf(buf, bufsz, "write(%ld, %p, %lu) = %ld", ev->args[0],
             (void *)ev->args[1], ev->args[2], ev->ret);
    break;
  case SYS_execve:
    if (read_child_string(ev->pid, ev->args[0], path_buf, sizeof(path_buf)) <
        0) {
      snprintf(path_buf, sizeof(path_buf), "<ilegivel>");
    }

    snprintf(buf, bufsz, "execve(\"%s\", ...) = %ld", path_buf, ev->ret);
    break;

  case SYS_exit_group:
    snprintf(buf, bufsz, "exit_group(%lu) = %ld", ev->args[0], ev->ret);
    break;
  default:
    snprintf(buf, bufsz, "%s(%#lx, %#lx, %#lx, %#lx, %#lx, %#lx) = %ld",
             syscall_name(ev->syscall_no), ev->args[0], ev->args[1],
             ev->args[2], ev->args[3], ev->args[4], ev->args[5], ev->ret);
    break;
  }
}
