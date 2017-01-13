/*
** load_elf.c for ftrace in /home/work/work/projects/PSU_2015_ftrace_doc/PSU_2015_ftrace/src
** 
** Made by Bastien DHIVER
** Login   <dhiver_b@epitech.net>
** 
** Started on  Mon Apr 25 23:00:21 2016 Bastien DHIVER
** Last update Sun May 01 03:06:04 2016 Bastien DHIVER
*/

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "ftrace.h"

int	load_elf(char *pathname, t_bin_infos *bin)
{
  bin->name = pathname;
  if (elf_version(EV_CURRENT) == EV_NONE)
    return (print_err("ELF lib initialisation failed: %s.\n",
		      elf_errmsg(-1)), 1);
  if ((bin->fd = open(bin->name, O_RDONLY)) == -1)
    return (print_err("open %s failed: %s.\n", bin->name, strerror(errno)), 1);
  if ((bin->e = elf_begin(bin->fd, ELF_C_READ, NULL)) == NULL)
    return (print_err("elf_begin() failed: %s.\n", elf_errmsg(-1)), 1);
  if (elf_kind(bin->e) != ELF_K_ELF)
    return (print_err("%s is not an ELF object.\n", bin->name), 1);
  if (gelf_getclass(bin->e) == ELFCLASSNONE)
    return (print_err("getclass() failed: %s.\n", elf_errmsg(-1)), 1);
  return (0);
}

int	unload_elf(t_bin_infos *bin)
{
  if (elf_end(bin->e) == -1)
    return (print_err("elf_end() failed: %s.\n", elf_errmsg(-1)), 1);
  if (close(bin->fd) == -1)
    return (print_err("close() %s failed: %s.\n", bin->name,
		      strerror(errno)), 1);
  return (0);
}
