/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 13:32:20 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/05/10 20:45:22 by aboukhmi         ###   ########.fr       */
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
} t_exee;

void    filtre_comands(t_exec **commands);
int     open_infiles(t_exec *commands);
int     open_outfiles(t_exec *commands);
char	**ft_split_exe(char *s, char c);
char    **env_list_to_array(t_env *env);

///////////////////////////built_ins

void    ft_export(char **args, t_env **env);
void pwd(t_env *env);
int		ft_exit(char **args, int last_status);
void	ft_echo(char **arg);
void    cd(char *av, t_env **env);
void    print_env(t_env *env);
void ft_unset(t_env **env, char **args);
char  *satic_stock(char *cmd, t_env **env);

/////////////////////////////////////
#endif