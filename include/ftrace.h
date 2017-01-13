/*
** ftrace.h for ftrace in /home/videau_f/rendu/PSU/PSU_2015_ftrace
**
** Made by florian videau
** Login   <videau_f@epitech.net>
**
** Started on  Tue Apr 12 14:07:13 2016 florian videau
** Last update Sun May 01 17:25:43 2016 Bastien DHIVER
*/

#ifndef FTRACE_H_
# define FTRACE_H_

# define _GNU_SOURCE
# include <gelf.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/user.h>

# define DEFAULT_PATH		"/usr/local/bin:/usr/bin:/bin"
# define SIGNAL_RCV_MSG		"Received signal %s\n"
# define SIGNAL_UNKN		"UNKNOWN"

# define print_err(...)		fprintf(stderr, __VA_ARGS__)

# define SYSCALL(opcode)        ((opcode & 0x0000FFFF) == 0x0000050f)
# define RELCALL(opcode)	((opcode & 0xFF) == 0xe8)
# define INDCALL(o)		((o & 0xFF) == 0xFF && (o & 0x3800) == 0x1000)
# define CALL(op)		(SYSCALL(op) || RELCALL(op) || INDCALL(op))
# define R_1(op)		((op & 0xFF) == 0xC3 || (op & 0xFF) == 0xC2)
# define R_2(op)		((op & 0xFF) == 0xCA  || (op & 0xFF) == 0xCB)
# define RET(op)		(R_1(op) || R_2(op))

typedef enum			e_call_type
  {
    RELATIVE,
    INDIRECT
  }				t_call_type;

typedef	enum			e_bool
{
  FALSE,
  TRUE
}				t_bool;

typedef struct			s_bin_infos
{
  char				*name;
  int				fd;
  Elf				*e;
}				t_bin_infos;

extern pid_t			g_pid;
extern t_bin_infos		g_bin;

typedef	struct user_regs_struct	t_regs;

typedef unsigned long long int	long_stuff;

typedef	struct			s_args
{
  char				**av;
  char				**env;
}				t_args;

typedef struct			s_pr_type
{
  int				(*ft_p)(long_stuff, void *);
}				t_pr_type;

typedef struct			s_call
{
  t_regs			regs;
  long_stuff			args_val[7];
}				t_call;

typedef struct			s_rex
{
  char				w;
  char				r;
  char				x;
  char				b;
}				t_rex;

/*
** find_executable.c
*/
char				*find_executable(char *name);

/*
** trace.c
*/
int				be_the_child(t_args *arg);
int				one_more_step(int *status, t_call *call,
					      long_stuff *opcode);
int				be_the_parent(t_call *call, char *);

/*
** utils.c
*/
int				display_error(int);
int				my_power_rec(int, int);

/*
** signals.c
*/
void				get_sigint(int);
int				aff_end(int);

/*
** load_elf.c
*/
int				load_elf(char *, t_bin_infos *);
int				unload_elf(t_bin_infos *);

/*
** get_name_from_addr.c
*/
char				*get_name_from_addr(long_stuff);

/*
** print_syscalls.c
*/
void				aff_syscall(t_call *);

/*
** print_signals.c
*/
int				aff_end_signal(int);

/*
** static_name_resolv.c
*/
char				*static_name_resolv(long_stuff);

/*
** dynamic_name_resolv.c
*/
char				*dynamic_name_resolv(long_stuff);

/*
** J0rmb97.c
*/
long_stuff			J0rmb97(t_call *, t_rex *,
					long_stuff, long_stuff);

/*
** S0rmb57.c
*/
long_stuff			S0rmb57(t_call *, t_rex *,
					long_stuff, long_stuff);

/*
** l0rmb17.c
*/
long_stuff			l0rmb17(t_call *, t_rex *,
					long_stuff, long_stuff);

/*
** D0rmbD7.c
*/
long_stuff			D0rmbD7(t_call *, t_rex *, long_stuff);

/*
** sib_base.c
*/
long_stuff			sib_base(t_call *, t_rex *,
					 long_stuff, char);
/*
** sib_index.c
*/
long_stuff			sib_index(t_call *call, t_rex *rex,
					  long_stuff rmb);

/*
** get_addr.c
*/
long_stuff			addr_indirect(long_stuff opcode,
					      t_call *call, t_rex *rex);
long_stuff			get_sib(unsigned char sib, t_call *call,
					t_rex *rex, char mod);
long_stuff			addr_relative(t_call *call,
					      long_stuff opcode, char rexw);
/*
** get_addr.c
*/
long_stuff			be_the_parent_rec(int *status, t_call *call,
						  t_rex *rex,
						  t_call_type calltype);
#endif /* !FTRACE_H_ */
