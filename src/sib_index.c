/*
** sib_index.c for ftrace in /home/videau_f/rendu/PSU/PSU_2015_ftrace
**
** Made by florian videau
** Login   <videau_f@epitech.net>
**
** Started on  Sun May  1 09:59:15 2016 florian videau
** Last update Sun May 01 17:23:25 2016 Bastien DHIVER
*/

#include <stdlib.h>
#include <sys/ptrace.h>
#include "ftrace.h"

long_stuff	*tab_no_sib_index(t_call *call)
{
  long_stuff	*tab;

  tab = malloc(8 * sizeof(long));

  tab[0] = call->regs.rax;
  tab[1] = call->regs.rcx;
  tab[2] = call->regs.rdx;
  tab[3] = call->regs.rbx;
  tab[4] = call->regs.rsp;
  tab[5] = call->regs.rbp;
  tab[6] = call->regs.rsi;
  tab[7] = call->regs.rdi;
  return (tab);
}

long_stuff	*tab_yes_sib_index(t_call *call)
{
  long_stuff	*tab;

  tab = malloc(8 * sizeof(long));

  tab[0] = call->regs.r8;
  tab[1] = call->regs.r9;
  tab[2] = call->regs.r10;
  tab[3] = call->regs.r11;
  tab[4] = call->regs.r12;
  tab[5] = call->regs.r13;
  tab[6] = call->regs.r14;
  tab[7] = call->regs.r15;
  return (tab);
}

long_stuff	sib_index(t_call *call, t_rex *rex, long_stuff rmb)
{
  long_stuff	*tab;
  long_stuff addr;

  if (!rex->x)
    tab = tab_no_sib_index(call);
  else
    tab = tab_yes_sib_index(call);
  addr = tab[rmb];
  free(tab);
  return (addr);
}
