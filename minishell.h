/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 17:52:42 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/02 11:35:11 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>

#define NAME_LEN 12

typedef enum e_token_type
{
	WORD, //(COMMANDS, ARGUMENTS, FILENAMES)
	PIPE, // "|"
	REDIR_IN, // "<"
	REDIR_OUT, // ">"
	HEREDOC, // "<<"
	APPEND ,// ">>"
	HEREDOC_DELIMITER, // <>
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_env
{
	char			*key;
	char			*value;
	char			*full;
	struct  s_env	*next;
}	t_env;

typedef struct s_cmd
{
	t_token			*token;
	struct s_cmd	*next;
}	t_cmd;


typedef	struct s_redir
{
	char			*filename;
	int				append;
	struct s_redir	*next;
}	t_redir;

// Structure for the execution phase

typedef struct  s_exec
{
	char			**args;	// ["ls" , "-l" , NULL]
	t_redir			*infiles; // for '< input.txt'
	t_redir			*outfiles;	// for '> output.txt' or '>>'
	int				append;	// 0 for '>' , 1 for '>>'
	int				heredoc; // 1 if it's a herdoc
	char			*delimiter;
	char			*cmd; // original command name
	int				herdoc_fd;
	struct s_exec	*next;
}	t_exec;

// struct for splitbypipe function 
typedef struct s_split_vars
{
	t_token *start;
	t_token *end;
	t_token *next_token;
	t_cmd   *cmd_list;
	t_cmd   *current_cmd;
}	t_split_vars;

// struct for expanding function
typedef struct s_expand_vars
{
	int		i;
	int		j;
	int		in_single;
	int		in_double;
	char	*str;
	t_env	*env;
}	t_expand_vars;


// init functions
void	init_split_vars(t_split_vars *var, t_token *tokens);
void	init_expand_vars(t_expand_vars *vars, char *str, t_env *env, int i, int j);


void	*ft_memcpy(void *dest, const void *src, size_t n);
char	**ft_split(char *input);
int		ft_strcmp(char *s1, char *s2);
char	*ft_strdup(const char *str);
char	*ft_strndup(const char *str, size_t n);
char	*ft_strchr(const char *s, int c);
int		ft_atoi(const char *str);
int		ft_isalnum(int c);
char    *ft_strcpy(char *s1, char *s2);
size_t	ft_strlen(const char *str);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_itoa(int n);
char	*ft_substr(char const *s, unsigned int start, size_t len);

t_token	*tokenizer(char *input);
t_token *create_token(char *content);

// envirement functions
t_env	*init_env(char **envp);
t_env	*create_env_var(char *env_start);
char	*get_env_value(t_env *env, char *key);
int		extract_var_name(char *str, int i, char **var_name);


char	*expand_variables(char *str, t_env *env, int init_i, int init_j);
t_cmd	*split_by_pipe(t_token *tokens);
void	remove_pipe_node(t_cmd	*cmd_list);
char	*check_unclosed_quotes(char *input);
int		check_two_pipes(char *input);
int		check_redirection_err(t_token *tokens);
char	*handle_pipe_end(char *input);
int		is_pipe_at_start(char *input);


t_cmd	*prepare_commands(char *input, t_env *env);
t_exec	*build_exec_list(char *input, t_env *env);
void	handle_all_herdocs(t_exec *execs, t_env *env);
void	add_infile(t_exec  *exec, char *filename);

int	count_dollars(char *str, int i);
void	handle_variable_expansion(t_expand_vars *vars, char *result);
char	*expand_herdoc_variables(char *str, t_env *env, int init_i, int init_j);

// Free functions :

void	free_token(t_token *tokens);
void	free_cmd_list(t_cmd *cmd);
void	free_exec_list(t_exec *exec_list);
void	free_env_list(t_env *env);

//yfshjfd
void execution(t_exec *commands, t_env *envi);
int is_special_char(char c);
int is_whitespace(char c);

#endif
