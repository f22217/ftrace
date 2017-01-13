/*
** ftrace.c for ftrace in /home/videau_f/rendu/PSU/PSU_2015_ftrace/src
**
** Made by florian videau
** Login   <videau_f@epitech.net>
**
** Started on  Tue Apr 12 11:59:11 2016 florian videau
** Last update Sun May 01 03:09:23 2016 Bastien DHIVER
*/

#define _GNU_SOURCE
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "ftrace.h"

pid_t		g_pid = 0;
t_bin_infos	g_bin;

int	run_process(t_args *args, t_call *call)
{
  char	*file_w_path;

  if ((file_w_path = find_executable(args->av[0])) == NULL)
    return (print_err("Can't find or execute '%s'\n", args->av[0]), 1);
  args->av[0] = file_w_path;
  if ((g_pid = fork()) == -1)
    return (display_error(errno), 1);
  return ((!g_pid) ? be_the_child(args) : be_the_parent(call, args->av[0]));
}

int		main(int ac, char **av, char **env)
{
  t_args	args;
  t_call	call;
  int		ret;

  if (ac < 2)
    return (print_err("Usage : %s <command>\n", av[0]), 0);
  signal(SIGINT, &get_sigint);
  args.av = av + 1;
  args.env = env;
  ret = run_process(&args, &call);
  if (unload_elf(&g_bin))
    return (1);
  return (ret);
}
