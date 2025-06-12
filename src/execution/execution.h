/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 13:32:20 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/12 13:35:16 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../../minishell.h"
# include <sys/wait.h>
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>

# define BUFFER_SIZE 42

typedef struct s_exee
{
	int	cmd_count;
	int	**pipes;
	int	*pids;
	int	infile;
	int	outfile;
	int	cd_in;
	int	cd_out;
	int	saved_in;
	int	saved_out;
}	t_exee;

typedef struct s_helper
{
	int	in;
	int	out;
}	t_helper;

// void    filtre_comands(t_exec **commands);
// int     open_infiles(t_exec *commands);
// int     open_outfiles(t_exec *commands);
char		**ft_split_exe(char *s, char c);
char		**env_list_to_array(t_env *env);
char		*get_full_path_f(char *argv, t_env **env);
// void	freeee(char **str);
void		cleanup_fds(t_exee **exe);
///////////////////////////built_ins
void		ft_export(char **args, t_env **env);
void		pwd(t_env *env);
int			ft_exit(t_exec **cmd, int last_status, t_env **env, t_exee **exe);
void		ft_echo(char **arg);
void		cd(char **av, t_env **env);
void		print_env(char **args, t_env *env);
void		ft_unset(t_env **env, char **args);
char		*satic_stock(char *cmd, t_env **env);
/////////////////////////////////////exit status
int			set_exit_status(int num, int flag);
//////////////////////////////////////handle_redir
int			open_in(char *filename, int *i);
int			open_out(char *filename, int *i, t_redir *red);
int			open_out_her(t_redir *red, int *i);
int			open_in_out(t_exec **commands);
////////////////env_list_array
char		**env_list_to_array(t_env *env);
///////////////init_execution
t_exee		*init_execution(t_exec **commands);
/////////////setup_command_io
void		setup_last_command_io(t_exee **exee, t_exec **cmd, int cmd_index);
int			is_there_any_out(t_redir *red);
int			is_there_any_in(t_redir *red);
void		setup_command_io(t_exee **exee, t_exec **cmd, int cmd_index);
//////custom_execve
int			custom_execve(char *str, t_exec **cmd, t_env **env, t_exee **exe);
/////////execute child
void		execute_child_process(t_exee **exee, t_exec **cmd, t_env **env);
/////////single_command
int			handle_single_command(t_exee **exee, t_exec **cmd, t_env **env);
/////////pipeline
void		handle_pipeline(t_exee **exee, t_exec **cmds, t_env **env);
void		setup_pipes(t_exee *exee);
/////////////clean_close
void		close_all_pipes(t_exee *exee);
void		cleanup_exe(t_exee *exe);
void		closeallfiles(t_exec **commands);
void		safe_close(int *fd);
int			is_built_in(char *str);

#endif