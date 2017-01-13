/*
** find_executable.c for ftrace in /home/videau_f/rendu/PSU/PSU_2015_ftrace
**
** Made by florian videau
** Login   <videau_f@epitech.net>
**
** Started on  Tue Apr 12 15:10:33 2016 florian videau
** Last update Tue Apr 12 15:10:35 2016 florian videau
*/

#define  _GNU_SOURCE
#include <limits.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include "ftrace.h"

int	exist_and_executable(char *path)
{
  return ((access(path, F_OK) == 0 && access(path, X_OK) == 0) ? 1 : 0);
}

char		*loop(char *name, char *path, size_t len)
{
  char		*sav_path;
  size_t	len_p;
  char		buf[PATH_MAX];

  while (1)
    {
      bzero(buf, PATH_MAX);
      sav_path = path;
      while (*path != 0 && *path != ':')
	path++;
      len_p = (sav_path == path && (sav_path = ".")) ? 1 : path - sav_path;
      if (len_p + len + 2 > sizeof(buf))
	continue;
      memcpy(buf, sav_path, len_p);
      buf[len_p] = '/';
      memcpy(buf + len_p + 1, name, len);
      buf[len_p + len + 1] = '\0';
      if (exist_and_executable(buf))
	return (strdup(buf));
      if (*path++ != ':')
	break;
    }
  return (NULL);
}

char	*find_executable(char *name)
{
  char	*path;

  if (!name || name[0] == '\0')
    return (NULL);
  if (strchr(name, '/'))
    return ((exist_and_executable(name)) ? name : 0);
  if ((path = getenv("PATH")) == NULL)
    path = DEFAULT_PATH;
  return (loop(name, path, strlen(name)));
}
