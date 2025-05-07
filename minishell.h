/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 17:52:42 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/06 20:29:10 by aboukhmi         ###   ########.fr       */
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

typedef enum e_token_type
{
	WORD, //(COMMANDS, ARGUMENTS, FILENAMES)
	PIPE, // "|"
	REDIR_IN, // "<"
	REDIR_OUT, // ">"
	HEREDOC, // "<<"
	APPEND ,// ">>"
	// REDIR_INOUT // <>
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
	struct s_exec	*next;
}	t_exec;



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
t_token	*tokenizer(char *input);
t_token *create_token(char *content);
t_env	*init_env(char **envp);

char	*expand_variables(char *str, t_env *env, int i, int j, int in_single, int in_double);
t_cmd	*split_by_pipe(t_token *tokens);
void	remove_pipe_node(t_cmd	*cmd_list);
char	*check_unclosed_quotes(char *input);
int		check_two_pipes(char *input);
int		check_redirection_err(t_token *tokens);
char	*handle_pipe_end(char *input);
int		is_pipe_at_start(char *input);


t_cmd	*prepare_commands(char *input, t_env *env);
t_exec	*build_exec_list(char *input, t_env *env);


// Free functions :

void	free_token(t_token *tokens);
void	free_cmd_list(t_cmd *cmd);
void	free_exec_list(t_exec *exec_list);
void	free_env_list(t_env *env);

//yfshjfd
void execution(t_exec *commands, t_env *envi);

#endif
