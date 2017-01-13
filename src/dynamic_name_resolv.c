/*
** dynamic_name_resolv.c for ftrace in /home/work/work/projects/PSU_2015_ftrace_doc/PSU_2015_ftrace/src
** 
** Made by Bastien DHIVER
** Login   <dhiver_b@epitech.net>
** 
** Started on  Sun May 01 00:04:01 2016 Bastien DHIVER
** Last update Sun May 01 10:08:59 2016 Bastien DHIVER
*/

#define _GNU_SOURCE
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ftrace.h"

int		is_in_addr_range(long_stuff addr, char *tok,
				 long_stuff *lib_load_at)
{
  char		*dash_at;
  long_stuff	begin_addr;
  long_stuff	end_addr;

  if ((dash_at = strchr(tok, '-')) == NULL)
    return (0);
  begin_addr = strtoll(tok, NULL, 16);
  if (errno == ERANGE)
    return (0);
  end_addr = strtoll(dash_at + 1, NULL, 16);
  if (errno == ERANGE)
    return (0);
  if (addr < begin_addr || addr > end_addr)
    return (0);
  *lib_load_at = begin_addr;
  return (1);
}

char	*found_name_and_unload(t_bin_infos *bin, GElf_Word sh_link,
			       GElf_Word st_name)
{
  char	*ret;

  ret = strdup(elf_strptr(bin->e, sh_link, st_name));
  if (unload_elf(bin))
    return (free(ret), NULL);
  return (ret);
}

char		*find_sym_in_dyn_lib(long_stuff addr, char *lib_path,
				     long_stuff begin_addr)
{
  GElf_Shdr	shdr;
  Elf_Scn	*scn;
  t_bin_infos	bin;
  Elf_Data	*data;
  int		count;
  GElf_Sym	sym;
  int		i;

  if (load_elf(lib_path, &bin))
    return (NULL);
  scn = NULL;
  while ((scn = elf_nextscn(bin.e, scn)) != NULL)
    {
      i = -1;
      gelf_getshdr(scn, &shdr);
      if (shdr.sh_type == SHT_SYMTAB && (data = elf_getdata(scn, NULL))
	  && data->d_size != 0 && (count = shdr.sh_size / shdr.sh_entsize))
	while (++i < count && gelf_getsym(data, i, &sym))
	  if (sym.st_name && sym.st_value
	      && (GELF_ST_TYPE(sym.st_info) == STT_FUNC))
	    if (begin_addr + sym.st_value == addr)
	      return (found_name_and_unload(&bin, shdr.sh_link, sym.st_name));
    }
  unload_elf(&bin);
  return (NULL);
}

char		*in_any_dynamic_lib(long_stuff addr, char *line)
{
  char		*saveptr;
  char		*tok;
  int		nb_col;
  char		*res;
  long_stuff	begin_addr;

  nb_col = 0;
  res = NULL;
  while ((tok = strtok_r(line, " ", &saveptr)) != NULL)
    {
      if (nb_col == 0 && !is_in_addr_range(addr, tok, &begin_addr))
	break;
      if (nb_col == 5)
	if (strchr(tok, '/') && strstr(tok, ".so") && access(tok, R_OK) == 0)
	  if ((res = find_sym_in_dyn_lib(addr, tok, begin_addr)) != NULL)
	    break;
      nb_col++;
      line = NULL;
    }
  return (res);
}

char		*dynamic_name_resolv(long_stuff addr)
{
  char		*path;
  FILE		*maps_file;
  char		*line;
  size_t	len;
  char		*ret;
  ssize_t	err;

  line = NULL;
  len = 0;
  ret = NULL;
  if (asprintf(&path, "/proc/%d/maps", g_pid) == -1)
    return (print_err("asprintf() failed: %s.\n", strerror(errno)), NULL);
  if ((maps_file = fopen(path, "r")) == NULL)
    return (print_err("fopen() failed: %s.\n", strerror(errno)), NULL);
  free(path);
  while ((err = getline(&line, &len, maps_file)) != -1
	 && !(line[strlen(line) - 1] = 0))
    if ((ret = in_any_dynamic_lib(addr, line)) != NULL)
      break;
  if (err == -1 && errno && errno != ERANGE)
    print_err("getline() failed: %s.\n", strerror(errno));
  free(line);
  if (fclose(maps_file) == EOF)
    print_err("fclose() failed: %s.\n", strerror(errno));
  return (ret);
}

