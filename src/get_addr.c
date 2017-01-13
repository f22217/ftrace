/*
** get_addr.c for ftrace in /home/videau_f/rendu/PSU/PSU_2015_ftrace
**
** Made by florian videau
** Login   <videau_f@epitech.net>
**
** Started on  Sun May  1 10:16:35 2016 florian videau
** Last update Sun May 01 18:26:43 2016 Bastien DHIVER
*/

#include <sys/ptrace.h>
#include "ftrace.h"

long_stuff	addr_relative(t_call *call, long_stuff opcode, char rexw)
{
  int		offset;
  long_stuff	call_addr;

  offset = (int)((opcode >> 8));
  if (rexw)
    {
      offset = ptrace(PTRACE_PEEKTEXT, g_pid, call->regs.rip + 1);
      call_addr = call->regs.rip + offset + 9;
    }
  else
    call_addr = call->regs.rip + offset + 5;
  return (call_addr);
}

long_stuff	get_sib(unsigned char sib, t_call *call, t_rex *rex,
				char mod)
{
  char			scale;
  char			index;
  char			base;
  long_stuff		result;

  scale = sib & 0xC0;
  index = sib & 0x38;
  base = sib & 0x07;
  result = index >= 0 && index <= 7 ? sib_index(call, rex, index) : 0;
  result *= (scale > 0 && scale < 4 ? my_power_rec(2, scale) : 1);
  result += base >= 0 && base <= 7 ? sib_base(call, rex, base, mod) : 0;
  return (result);
}

long_stuff addr_indirect(long_stuff opcode, t_call *call, t_rex *rex)
{
  unsigned char	rmb;

  rmb = (opcode & 0xFF00) >> 8;
  if (rmb >= 0xD0 && rmb <= 0xD7)
      return (D0rmbD7(call, rex, rmb));
  else if (rmb >= 0x10 && rmb <= 0x17)
    return (l0rmb17(call, rex, rmb, opcode));
  else if (rmb >= 0x50 && rmb <= 0x57)
    return (S0rmb57(call, rex, rmb, opcode));
  else if (rmb >= 0x90 && rmb <= 0x97)
    return (J0rmb97(call, rex, rmb, opcode));
  return (0);
}
