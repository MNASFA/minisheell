/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:44:14 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/25 15:16:27 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../minishell.h"

int	count_args(t_token *tokens)
{
	int		count;
	t_token	*current;
	t_token	*prev;

	count = 0;
	current = tokens;
	prev = NULL;
	while (current)
	{
		if (current->type == WORD)
		{
			if (!prev || (prev->type != REDIR_IN && prev->type != REDIR_OUT
					&& prev->type != APPEND && prev->type != HEREDOC))
				count++;
		}
		prev = current;
		current = current->next;
	}
	return (count);
}

void	add_outfile(t_exec	*exec, char	*filename, int append)
{
	t_redir	*new;
	t_redir	*tmp;

	new = malloc(sizeof(t_redir));
	new->filename = ft_strdup(filename);
	new->append = append;
	new->next = NULL;
	new->delimiter = NULL;
	new->is_herdoc = 0;
	if (!exec->outfiles)
		exec->outfiles = new;
	else
	{
		tmp = exec->outfiles;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	add_infile(t_exec *exec, char *filename)
{
	t_redir	*new;
	t_redir	*tmp;

	new = malloc(sizeof(t_redir));
	new->filename = ft_strdup(filename);
	new->next = NULL;
	new->is_herdoc = 0;
	new->delimiter = NULL;
	if (!exec->infiles)
		exec->infiles = new;
	else
	{
		tmp = exec->infiles;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	add_heredoc(t_exec *exec, t_redir *new)
{
	t_redir	*tmp;

	new->next = NULL;
	new->is_herdoc = 1;
	if (!exec->infiles)
		exec->infiles = new;
	else
	{
		tmp = exec->infiles;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

static t_exec	*init_exec(int arg_count)
{
	t_exec	*exec;

	exec = malloc(sizeof(t_exec));
	if (!exec)
		return (NULL);
	exec->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!exec->args)
	{
		free(exec);
		return (NULL);
	}
	exec->infiles = NULL;
	exec->outfiles = NULL;
	exec->append = 0;
	exec->cmd = NULL;
	exec->next = NULL;
	return (exec);
}

static void	handle_redirections(t_exec *exec, t_token *current)
{
	int		is_append;
	t_redir	*new;

	is_append = 0;
	if (current->type == REDIR_IN && current->next)
		add_infile(exec, current->next->value);
	else if ((current->type == REDIR_OUT || current->type == APPEND)
		&& current->next)
	{
		if (current->type == APPEND)
			is_append = 1;
		add_outfile(exec, current->next->value, is_append);
	}
	else if (current->type == HEREDOC && current->next)
	{
		new = malloc(sizeof(t_redir));
		new->filename = NULL;
		new->delimiter = ft_strdup(current->next->value);
		if (!new->delimiter)
		{
			free(new);
			return ;
		}
		new->quoted_flag = current->next->quoted_flag;
		new->append = 0;
		new->next = NULL;
		new->heredoc_count = detect_delimiter(current);
		add_heredoc(exec, new);
	}
}

static void	handle_word(t_exec *exec, t_token *current
	, t_token *prev, int *i)
{
	if (!prev || (prev->type != REDIR_IN && prev->type != REDIR_OUT
			&& prev->type != APPEND && prev->type != HEREDOC))
	{	
		exec->var_in_quotes = current->var_in_quotes;
		exec->expanded_flag = current->expanded_flag;
		exec->args[*i] = ft_strdup(current->value);
		if (!exec->cmd)
			exec->cmd = exec->args[0];
		(*i)++;
	}
}

t_exec	*parse_command(t_cmd *cmd, int i)
{
	t_exec	*exec;
	t_token	*current;
	t_token	*prev;
	int		arg_count;

	if (!cmd || !cmd->token)
		return (NULL);
	current = cmd->token;
	prev = NULL;
	arg_count = count_args(current);
	exec = init_exec(arg_count);
	if (!exec)
		return (NULL);
	while (current)
	{
		if (current->type == WORD)
			handle_word(exec, current, prev, &i);
		else
			handle_redirections(exec, current);
		prev = current;
		current = current->next;
	}
	exec->args[i] = NULL;
	return (exec);
}

int	detect_delimiter(t_token *tokens)
{
	t_token	*current;
	int		heredoc_count;

	heredoc_count = 0;
	current = tokens;
	while (current)
	{
		if (current->type == HEREDOC && current->next && current->next->type == WORD)
		{
			current->next->type = HEREDOC_DELIMITER;
			heredoc_count++;
		}
		current = current->next;
	}
	return (heredoc_count);
}

static void	copy_without_quotes(const char *src, char *dst, int *removed)
{
	int	i;
	int	j;
	int	quote;

	i = 0;
	j = 0;
	quote = 0;
	while (src[i])
	{
		if ((src[i] == '\'' || src[i] == '\"') && quote == 0)
		{
			quote = src[i++];
			*removed = 1;
		}
		else if (src[i] == quote)
		{
			quote = 0;
			i++;
		}
		else
			dst[j++] = src[i++];
	}
	dst[j] = '\0';
}

int	remove_quotes(char **str_ptr, char **new_str)
{
	char	*str;
	int		quotes_removed;

	if (!str_ptr || !new_str)
		return (0);
	str = *str_ptr;
	quotes_removed = 0;
	if (!str)
	{
		*new_str = NULL;
		return (0);
	}
	*new_str = malloc(ft_strlen(str) + 1);
	if (!*new_str)
		return (0);
	copy_without_quotes(str, *new_str, &quotes_removed);
	return (quotes_removed);
}

static void	process_expansion(t_token *tokens, t_env *env)
{
	t_token	*current;
	char	*expanded_value;
	char	*quote_processed;

	current = tokens;
	int track = 0;
	while (current)
	{
		if (current->type == WORD)
		{
			expanded_value = expand_variables(current->value, env, 0, 0, current);
			if (expanded_value)
			{
				remove_quotes(&expanded_value, &quote_processed);
				free(expanded_value);
				if (quote_processed)
				{
					free(current->value);
					current->value = quote_processed;
				}
				else
					current->value = ft_strdup("");
			}
		}
		current = current->next;
		track++;
	}
}

static void	process_heredoc(t_token *tokens)
{
	t_token	*current;
	char	*new_del;

	current = tokens;
	while (current)
	{
		if (current->type == HEREDOC_DELIMITER)
		{
			current->quoted_flag = remove_quotes(&current->value, &new_del);
			free(current->value);
			current->value = new_del;
		}
		current = current->next;
	}
}

t_cmd    *prepare_commands(char *input, t_env *env)
{
	t_token    *tokens;
	t_cmd    *cmds;

	input = check_unclosed_quotes(input);
	if (!input)
		return (NULL);
	tokens = tokenizer(input);
	if (!tokens)
		return (NULL);
	detect_delimiter(tokens);
	if (check_errors(tokens, 0, NULL, 0) == 1)
	{
		free_token_list(tokens);
		return (NULL);
	}
	process_expansion(tokens, env);
	t_token *tokens2 = split_token_quotes(tokens);
	if (!tokens2)
		return (NULL);
<<<<<<< HEAD
	free_token_list(tokens);
=======
>>>>>>> anouar
	process_heredoc(tokens2);
	cmds = split_by_pipe(tokens2);
	free_token_list(tokens2);
	return (cmds);
}

static t_exec	*append_exec_nodes(t_cmd *cmds, t_exec **exec_list)
{
	t_exec	*current;
	t_exec	*new_node;

	current = NULL;
	while (cmds)
	{
		new_node = parse_command(cmds, 0);
		if (!new_node)
		{
			free_exec_list(*exec_list);
			return (NULL);
		}
		if (!*exec_list)
			*exec_list = new_node;
		else
			current->next = new_node;
		current = new_node;
		cmds = cmds->next;
	}
	return (*exec_list);
}

t_exec	*build_exec_list(char *input, t_env *env)
{
	t_cmd	*cmds;
	t_exec	*exec_list;

	exec_list = NULL;
	cmds = prepare_commands(input, env);
	if (!cmds)
		return (NULL);
	if (!append_exec_nodes(cmds, &exec_list))
	{
		free_cmd_list(cmds);
		return (NULL);
	}
	free_cmd_list(cmds);
	return (exec_list);
}
