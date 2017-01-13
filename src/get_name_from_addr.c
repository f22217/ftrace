/*
** get_name_from_addr.c for ftrace in /home/work/work/projects/PSU_2015_ftrace_doc/PSU_2015_ftrace/src
**
** Made by Bastien DHIVER
** Login   <dhiver_b@epitech.net>
**
** Started on  Wed Apr 20 14:27:12 2016 Bastien DHIVER
** Last update Sun May 01 04:13:12 2016 Bastien DHIVER
*/

#define _GNU_SOURCE
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <dlfcn.h>
#include "ftrace.h"

char	*get_name_from_addr(long_stuff addr)
{
  char	*res;

  if ((res = static_name_resolv(addr)) != NULL)
    return (strdup(res));
  if ((res = dynamic_name_resolv(addr)) != NULL)
    return (res);
  asprintf(&res, "func_0x%llX@%s", addr, rindex(g_bin.name, '/') + 1);
  return (res);
}
