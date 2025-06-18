/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 19:27:58 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/18 18:14:36 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	fill_heredoc_redir(t_redir *new, t_token *current)
{
	new->delimiter = extract_her_delimiter(current->next->value,
			current->next->quoted_flag, current->next->original_del);
	free(current->next->original_del);
	if (!new->delimiter)
		return (0);
	new->type = HEREDOC;
	new->heredoc_count = detect_delimiter(current);
	return (1);
}

static void	fill_simple_redir(t_redir *new, t_token *current)
{
	new->filename = ft_strdup(current->next->value);
	if (current->type == REDIR_IN)
		new->type = REDIR_IN;
	else if (current->type == REDIR_OUT)
		new->type = REDIR_OUT;
	else if (current->type == APPEND)
		new->type = APPEND;
}

static void	handle_redirections(t_exec *exec, t_token *current)
{
	t_redir	*new;

	if (!current || !current->next)
		return ;
	new = malloc(sizeof(t_redir));
	if (!new)
		return ;
	new->filename = NULL;
	new->delimiter = NULL;
	new->next = NULL;
	new->herdoc_fd = -1;
	new->heredoc_count = 0;
	new->quoted_flag = current->next->quoted_flag;
	if (current->type == HEREDOC)
	{
		if (!fill_heredoc_redir(new, current))
			return (free(new), (void)0);
	}
	else if (current->type == REDIR_IN || current->type == REDIR_OUT
		|| current->type == APPEND)
		fill_simple_redir(new, current);
	else
		return (free(new), (void)0);
	add_redir(exec, new);
}

static void	handle_word(t_exec *exec, t_token *current
	, t_token *prev, int *i)
{
	if (current->expanded_flag && !(*current->value) && !current->var_in_quotes)
		return ;
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

t_exec	*parse_command(t_cmd *cmd, int i, t_token *prev)
{
	t_exec	*exec;
	t_token	*current;
	int		arg_count;

	if (!cmd || !cmd->token)
		return (NULL);
	current = cmd->token;
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
