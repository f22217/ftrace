/*
** static_name_resolv.c for ftrace in /home/work/work/projects/PSU_2015_ftrace_doc/PSU_2015_ftrace/src
** 
** Made by Bastien DHIVER
** Login   <dhiver_b@epitech.net>
** 
** Started on  Sun May 01 00:01:49 2016 Bastien DHIVER
** Last update Sun May 01 00:03:28 2016 Bastien DHIVER
*/

#include "ftrace.h"

char		*iterat_over_static_sym(GElf_Shdr *shdr, Elf_Data *data,
				       long_stuff addr)
{
  GElf_Sym	sym;
  int		count;
  int		i;

  count = shdr->sh_size / shdr->sh_entsize;
  i = -1;
  while (++i < count)
    {
      if (gelf_getsym(data, i, &sym) == NULL)
	return (print_err("gelf_getsym() failed: %s.",
			  elf_errmsg(-1)), NULL);
      if ((!sym.st_name) || sym.st_value == 0)
	continue;
      else if ((long_stuff)sym.st_value == addr)
	return (elf_strptr(g_bin.e, shdr->sh_link, sym.st_name));
    }
  return (NULL);
}

char		*static_name_resolv(long_stuff addr)
{
  GElf_Shdr	shdr;
  Elf_Scn	*scn;
  Elf_Data	*data;
  char		*res;

  scn = NULL;
  while ((scn = elf_nextscn(g_bin.e, scn)) != NULL)
    {
      if (gelf_getshdr(scn, &shdr) != &shdr)
	return (print_err("gelf_getshdr() failed: %s.", elf_errmsg(-1)), NULL);
      if (shdr.sh_type == SHT_SYMTAB)
	{
	  if ((data = elf_getdata(scn, NULL)) == NULL)
	    return (print_err("elf_getdata() failed: %s.",
			      elf_errmsg(-1)), NULL);
	  if ((res = iterat_over_static_sym(&shdr, data, addr)) != NULL)
	    return (res);
	}
    }
  return (NULL);
}
