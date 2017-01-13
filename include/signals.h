/*
** signals.h for ftrace in /home/work/work/projects/PSU_2015_ftrace_doc/PSU_2015_ftrace/include
** 
** Made by Bastien DHIVER
** Login   <dhiver_b@epitech.net>
** 
** Started on  Wed Apr 20 10:17:57 2016 Bastien DHIVER
** Last update Sun May 01 21:38:32 2016 Bastien DHIVER
*/

#ifndef SIGNALS_H_
# define SIGNALS_H_

# define _GNU_SOURCE
# include <signal.h>

typedef	struct	s_signals
{
  int		nbr;
  char		*name;
}		t_signals;

const t_signals		g_signals[] =
{
    {SIGHUP, "SIGHUP"},
    {SIGTERM, "SIGTERM"},
    {SIGQUIT, "SIGQUIT"},
    {SIGILL, "SIGILL"},
    {SIGTRAP, "SIGTRAP"},
    {SIGABRT, "SIGABRT"},
    {SIGBUS, "SIGBUS"},
    {SIGFPE, "SIGFPE"},
    {SIGKILL, "SIGKILL"},
    {SIGUSR1, "SIGUSR1"},
    {SIGSEGV, "SIGSEGV"},
    {SIGUSR2, "SIGUSR2"},
    {SIGPIPE, "SIGPIPE"},
    {SIGALRM, "SIGALRM"},
    {SIGTERM, "SIGTERM"},
    {SIGSTKFLT, "SIGTKFLT"},
    {SIGCHLD, "SIGCHLD"},
    {SIGCONT, "SIGCONT"},
    {SIGSTOP, "SIGSTOP"},
    {SIGTSTP, "SIGTSTP"},
    {SIGTTIN, "SIGTTIN"},
    {SIGTTOU, "SIGTTOU"},
    {SIGURG, "SIGURG"},
    {SIGXCPU, "SIGXCPU"},
    {SIGXFSZ, "SIGXFSZ"},
    {SIGVTALRM, "SIGVTALRM"},
    {SIGPROF, "SIGPROF"},
    {SIGWINCH, "SIGWINCH"},
    {SIGPOLL, "SIGPOLL"},
    {SIGPWR, "SIGPWR"},
    {SIGSYS, "SIGSYS"},
    {-1, "UNKNOWN"}
};

#endif /* SIGNALS_H_*/
