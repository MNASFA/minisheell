/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 13:32:20 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/16 13:41:42 by aboukhmi         ###   ########.fr       */
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
void		wait_for_all_children(t_exee *exee);
///////////////////////////built_ins
void		ft_export(char **args, t_env **env);
void		ft_lstadd_back_ex(t_env **lst, t_env *new);
t_env		*free_all_env(t_env *to_free);
void		delete_if_exist(t_env **env, char *to_delete);
int			is_there_equal(char *str);
int			find_next_min(t_env **env, int *printed, int size);
int			env_count_hh(t_env *tmp);
t_env		**env_t_array(t_env *env, int count);
int			*printed_tracker(t_env **env_array, int count);
void		printtt(t_env **env_array, int *printed, int count);
void		print_sorted(t_env **env);
void		update(char *key, t_env **env, char *new);
int			is_in_env(t_env *env, char *key);
int			is_valid_name(char *str);
int			check_printable(char **args);
void		pwd(t_env *env);
int			ft_exit(t_exec **cmd, int last_status, t_env **env, t_exee **exe);
void		handle_no_numeric(char **args, t_env **env, t_exee **exe);
void		norm(char **args, t_env **env, t_exee **exe);
void		cleanup_fds(t_exee **exe);
void		ft_echo(char **arg);
void		cd(char **av, t_env **env);
void		go_home(t_env **env);
void		update_old_pwd(char *av, t_env **env);
void		export_env_var(char *key, char *value, t_env **env);
void		free_array(char **arr);
t_env		*find_in_env(char *key, t_env *env);
void		process_6(t_env **env);
void		handle_cd_error(char *arg, char *oldpwd);
void		print_env(char **args, t_env *env);
void		ft_unset(t_env **env, char **args);
char		*satic_stock(char *cmd, t_env **env);
void		cleanup_pwd(void);
int			is_in_env(t_env *env, char *key);
int			path_exists_in_env(t_env **env);
int			is_directory(char *path);
char		**splite_exp(char *s, char sep);
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
void		cle_env_fds(t_env **env);
#endif