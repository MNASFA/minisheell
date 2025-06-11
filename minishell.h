/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 17:52:42 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/11 11:38:31 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

# include <sys/wait.h>
# include <signal.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/ioctl.h>

# define NAME_LEN 12

extern int g_signum;

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND ,
	HEREDOC_DELIMITER, 
}	t_token_type;

/*
	--> Structures
*/

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	int				quoted_flag;
	int				var_in_quotes;
	int				expanded_flag;
	struct s_token	*next;
}	t_token;   

typedef struct s_env
{
	char			*key;
	char			*value;
	char			*full;
	int				is_first;
	int				is_print;
	int				fd_in;
	int				fd_out;
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
	int				herdoc_fd;
	int				heredoc_count;
	int				quoted_flag;
	char			*delimiter;
	t_token_type	type;
	struct s_redir	*next;
}	t_redir;

typedef struct  s_exec
{
	char			**args;
	t_redir			*redirections;
	char			*cmd;
	int				var_in_quotes;
	int				expanded_flag;
	int				outfd;
	int				infd;
	struct s_exec	*next;
}	t_exec;

// Expand Variables Struct

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
void	*ft_memcpy(void *dest, const void *src, size_t n);
// char	**ft_split(char *input);
int		ft_strcmp(char *s1, char *s2);
char	*ft_strdup(const char *str);
char	*ft_strndup(const char *str, size_t n);
char	*ft_strchr(const char *s, int c);
int		ft_atoi(const char *str);
int		ft_isalnum(int c);
int		ft_isdigit(int c);
char    *ft_strcpy(char *s1, char *s2);
size_t	ft_strlen(const char *str);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_itoa(int n);

t_token	*tokenizer(char *input, int i);
t_token *create_token(char *content);

// envirement functions
t_env *init_env(char **envp, int i, int generated_env);
t_env	*create_env_var(char *env_start);
char	*get_env_value(t_env *env, char *key);
int		extract_var_name(char *str, int i, char **var_name);


char	*expand_variables(char *str, t_env *env, t_token *tokens);
t_cmd	*split_by_pipe(t_token *tokens);
char	*check_unclosed_quotes(char *input);
int	check_errors(t_token *tokens, int check, t_token *prev, int her_count);


t_cmd	*prepare_commands(char *input, t_env *env);
t_exec	*build_exec_list(char *input, t_env *env);
void	handle_all_herdocs(t_exec *execs, t_env *env);
int		detect_delimiter(t_token *tokens);


int		count_dollars(char *str, int i);
void	handle_variable_expansion(t_expand_vars *vars, char *result);
char	*expand_herdoc_variables(char *str, t_env *env);
void	quotes_state(char c, int *in_single, int *in_double);

// Free functions :

void	free_cmd_list(t_cmd *cmd);
void	free_exec_list(t_exec *exec_list);
void	free_env_list(t_env *env);
void	free_token_list(t_token *head);
void	free_split(char	**split);
void	free_envir(t_env *head);

//yfshjfd
void execution(t_exec **commands, t_env **envi);
int is_whitespace(char c);


/////////////////
int set_exit_status(int num, int flag);
int	ft_strncmp(const char *str1, const char *str2, size_t n);
////////////////////////
void	ft_export(char **args, t_env **env);
void	freeee(char **str);
void	ft_putstr_fd(char *s, int fd);
void	debug(t_env **env, char *arr);
void free_envir(t_env *head);

char	**ft_split_exe(char *s, char c);
t_token	*split_token_quotes(t_token *token_origin);


// signals
void	sigint_handler_her(int sig);
void	sigint_handler_main(int sig);
void	sigint_handler_re(int sig);


// env

char	**env_no_env(void);

// expand utils
int		count_dollars(char *str, int i);
void	quotes_state(char c, int *in_single, int *in_double);
int		ft_numlen(int n);
void	init_expand_vars(t_expand_vars *vars, char *str, t_env *env);

// HERDOC

int	open_heredoc_file(char *file_name, int *fd_read, int *fd_write);
char	*generate_filename(void);
int	handle_heredoc(t_redir *redir, t_env *env);

// parse command
t_exec	*parse_command(t_cmd *cmd, int i, t_token *prev);

// syntax err
int	is_redir(t_token *tokens);
int	check_redir(int flag, t_token *tokens);
int	check_pipe(int flag, t_token *tokens, t_token *prev);

//fake herdoc 
int	double_check_errors(t_token *tokens);

void	add_redir(t_exec	*exec, t_redir *new);
int	count_args(t_token *tokens);
t_exec	*init_exec(int arg_count);

int	is_in_quotes(char c, int *in_single, int *in_double);

t_token	*ft_lstlast(t_token *lst);
void	ft_lstadd_back_tpk(t_token **lst, t_token *new);

#endif
