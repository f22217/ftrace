/*
** rec.c for ftrace in /home/videau_f/rendu/PSU/PSU_2015_ftrace
**
** Made by florian videau
** Login   <videau_f@epitech.net>
**
** Started on  Sun May  1 10:45:33 2016 florian videau
** Last update Sun May 01 18:27:12 2016 Bastien DHIVER
*/

#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include "ftrace.h"

int	 test_call(long_stuff *opcode, char *fct_name, int *status,
		   t_call *call)
{
  t_rex	rex;

  bzero(&rex, sizeof(t_rex));
  if ((*opcode & 0xF0) == 0x40)
    {
      rex.w = *opcode & 0x8;
      rex.r = *opcode & 0x4;
      rex.x = *opcode & 0x2;
      rex.b = *opcode & 0x1;
      *opcode = ptrace(PTRACE_PEEKTEXT, g_pid, ++call->regs.rip);
    }
  if (RET(*opcode))
    {
      printf("Leaving function %s\n", fct_name);
      return (free(fct_name), 1);
    }
  else if (RELCALL(*opcode))
    {
      if (!(*opcode = be_the_parent_rec(status, call, &rex, RELATIVE)))
	return (0);
    }
  else if (INDCALL(*opcode))
    if (!(*opcode = be_the_parent_rec(status, call, &rex, INDIRECT)))
      return (0);
  return 2;
}

int		loop_rec(long_stuff *opcode, int *status,
				 t_call *call, char *fct_name)
{
  int	test_ret;

  while (!CALL(*opcode) && !RET(*opcode) && (*opcode & 0xF0) != 0x40
	 && !WIFEXITED(*status))
    if (one_more_step(status, call, opcode))
      return (0);
  if (SYSCALL(*opcode))
    {
      if (one_more_step(status, call, opcode))
	return (0);
      if (aff_end_signal(*status))
	aff_syscall(call);
      else
	return (0);
    }
  if (!(test_ret = test_call(opcode, fct_name, status, call)))
    return 0;
  else if (test_ret == 1)
    return 1;
  if (one_more_step(status, call, opcode))
    return (0);
  return 2;
}

long_stuff	be_the_parent_rec(int *status, t_call *call,
					  t_rex *rex, t_call_type calltype)
{
  long_stuff	addr;
  long_stuff	opcode;
  char		*fct_name;
  int		loop_return;

  if (!(opcode = ptrace(PTRACE_PEEKTEXT, g_pid, call->regs.rip, call->regs)))
    return (display_error(errno), 0);
  addr = (calltype == RELATIVE ? addr_relative(call, opcode, rex->w) :
	  addr_indirect(opcode, call, rex));
  fct_name = get_name_from_addr(addr);
  printf("Entering function %s at 0x%llx\n", fct_name, (long_stuff)addr);
  if (one_more_step(status, call, &opcode))
    return (free(fct_name), 0);
  while (!RET(opcode) && aff_end_signal(*status))
    {
      if (!(loop_return = loop_rec(&opcode, status, call, fct_name)))
	return (free(fct_name), 0);
      else if (loop_return == 1)
	return (opcode);
    }
  printf("Leaving function %s\n", fct_name);
  return (free(fct_name), opcode);
}
