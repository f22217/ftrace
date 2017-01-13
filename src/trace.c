/*
** trace.c for ftrace in /home/work/work/projects/PSU_2015_ftrace_doc/PSU_2015_ftrace/src
**
** Made by Bastien DHIVER
** Login   <dhiver_b@epitech.net>
**
** Started on  Sun May 01 08:56:04 2016 Bastien DHIVER
** Last update Sun May 01 17:26:14 2016 Bastien DHIVER
*/

#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>
#include "ftrace.h"

int	be_the_child(t_args *args)
{
  int	fd;

  if ((fd = open("/dev/null", O_WRONLY)) == -1)
    return (1);
  if (dup2(fd, 1) == -1 || dup2(fd, 2) == -1)
    return (1);
  if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1)
    return (display_error(errno), 1);
  if (execve(args->av[0], args->av, args->env) == -1)
    return (display_error(errno), 1);
  return (0);
}

int	one_more_step(int *status, t_call *call, long_stuff *opcode)
{
  if (ptrace(PTRACE_SINGLESTEP, g_pid, NULL, NULL) == -1)
    return (display_error(errno), 1);
  if (waitpid(g_pid, status, 0) == -1)
    return (display_error(errno), 1);
  if (aff_end_signal(*status))
    {
      if (ptrace(PTRACE_GETREGS, g_pid, NULL, &(call->regs)) == -1)
	return (display_error(errno), 1);
      if (!(*opcode = ptrace(PTRACE_PEEKTEXT, g_pid,
			     call->regs.rip, call->regs)))
	return (display_error(errno), 1);
      return (0);
    }
  return (1);
}

int	during_signal_to_appear(long_stuff opcode, int *status,
				t_call *call, t_rex *rex)
{
  while (!opcode || !CALL(opcode))
    if (one_more_step(status, call, &opcode))
      return (1);
  if (SYSCALL(opcode))
    {
      if (one_more_step(status, call, &opcode))
	return (1);
      if (aff_end_signal(*status))
	aff_syscall(call);
    }
  if (RELCALL(opcode))
    {
      if (!(opcode = be_the_parent_rec(status, call, rex, RELATIVE)))
	return (1);
    }
  else if (INDCALL(opcode))
    {
      if (!(opcode = be_the_parent_rec(status, call, rex, INDIRECT)))
	return (1);
    }
  if (one_more_step(status, call, &opcode))
    return (1);
  return (0);
}

int		be_the_parent(t_call *call, char *pathname)
{
  int		status;
  long_stuff	opcode;
  t_rex		rex;

  if (load_elf(pathname, &g_bin))
    return (1);
  if (waitpid(g_pid, &status, 0) == -1)
    return (display_error(errno), 1);
  if (ptrace(PTRACE_GETREGS, g_pid, NULL, &(call->regs)) == -1)
    return (display_error(errno));
  opcode = ptrace(PTRACE_PEEKTEXT, g_pid, call->regs.rip, call->regs);
  while (aff_end_signal(status))
    {
      bzero(&rex, sizeof(t_rex));
      if (during_signal_to_appear(opcode, &status, call, &rex))
	return (1);
    }
  return (0);
}
