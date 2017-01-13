/*
** print_syscalls.c for ftrace in /home/work/work/projects/PSU_2015_ftrace_doc/PSU_2015_ftrace/src
** 
** Made by Bastien DHIVER
** Login   <dhiver_b@epitech.net>
** 
** Started on  Thu Apr 28 15:18:08 2016 Bastien DHIVER
** Last update Thu Apr 28 23:22:19 2016 Bastien DHIVER
*/

#include <string.h>
#include "ftrace.h"
#include "syscalls.h"

void	init_regs_tab(t_call *call)
{
  call->args_val[0] = call->regs.rdi;
  call->args_val[1] = call->regs.rsi;
  call->args_val[2] = call->regs.rdx;
  call->args_val[3] = call->regs.r10;
  call->args_val[4] = call->regs.r8;
  call->args_val[5] = call->regs.r9;
  call->args_val[6] = 0;
}

int	print_syscall_name(int syscall)
{
  int	i;

  i = -1;
  while (strcmp(g_syscalls[++i].name, "") != 0)
    if (i == syscall)
      return (printf("Syscall %s", g_syscalls[i].name));
  return (printf("Unimplemented syscall : %d\n", syscall), 0);
}

void	print_arg_loop(t_call *call, int during)
{
  int	i;

  i = -1;
  while (++i < during)
    {
      printf("0x%llx", call->args_val[i]);
      if (i != during - 1)
	printf(", ");
    }
}

void		print_ret(t_call *call)
{
  t_types	type;

  type = g_syscalls[call->regs.orig_rax].ret;
  if (type == E_UNKN && printf("?"))
    return ;
  printf("0x%llx", call->regs.rax);
}

void		aff_syscall(t_call *call)
{
  init_regs_tab(call);
  if (print_syscall_name((int)call->regs.orig_rax) == 0)
    return ;
  printf("(");
  print_arg_loop(call, strlen(g_syscalls[call->regs.orig_rax].args));
  printf(") = ");
  print_ret(call);
  printf("\n");
}
