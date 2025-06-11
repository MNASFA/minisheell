/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 13:32:20 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/11 15:34:14 by aboukhmi         ###   ########.fr       */
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
    int     cmd_count;       // Number of commands
    int     **pipes;         // Array of pipe file descriptors
    int     *pids;          // Array of process IDs
    int     infile;         // Input file descriptor
    int     outfile;        // Output file descriptor
    int     cd_in;
    int     cd_out;
    int     saved_in;
    int     saved_out;
} t_exee;

typedef struct s_helper
{
    int in;
    int out;
}t_helper;

// void    filtre_comands(t_exec **commands);
// int     open_infiles(t_exec *commands);
// int     open_outfiles(t_exec *commands);
char	**ft_split_exe(char *s, char c);
char    **env_list_to_array(t_env *env);
char *get_full_path_f(char *argv, t_env **env);
void cleanup_exe(t_exee *exe);
// void	freeee(char **str);
void safe_close(int *fd);
void cleanup_fds(t_exee **exe);
///////////////////////////built_ins

void    ft_export(char **args, t_env **env);
void pwd(t_env *env);
int	ft_exit(t_exec **cmd, int last_status, t_env **env, t_exee **exe);
void	ft_echo(char **arg);
void    cd(char **av, t_env **env);
void print_env(char **args, t_env *env);
void ft_unset(t_env **env, char **args);
char  *satic_stock(char *cmd, t_env **env);

/////////////////////////////////////exit status
int set_exit_status(int num, int flag);
#endif