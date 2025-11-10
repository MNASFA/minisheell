/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 21:56:54 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/18 15:36:16 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_token	*copy_tokens(t_token *start, t_token *end)
{
	t_token		*copy_start;
	t_token		*copy_current;
	t_token		*new_token;

	copy_start = NULL;
	copy_current = NULL;
	while (start != end && start != NULL)
	{
		new_token = create_token(start->value);
		if (!new_token)
			return (free_token_list(copy_start), NULL);
		new_token->type = start->type;
		new_token->quoted_flag = start->quoted_flag;
		new_token->var_in_quotes = start->var_in_quotes;
		new_token->expanded_flag = start->expanded_flag;
		new_token->original_del = start->original_del;
		if (!copy_start)
			copy_start = new_token;
		else
			copy_current->next = new_token;
		copy_current = new_token;
		start = start->next;
	}
	return (copy_start);
}

static t_cmd	*create_new_cmd(t_token *copy_start)
{
	t_cmd	*new_cmd;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->token = copy_start;
	new_cmd->next = NULL;
	return (new_cmd);
}

t_cmd	*create_and_copy(t_token *start, t_token *end,
	t_cmd **cmd_list, t_cmd **last_cmd)
{
	t_token	*copy;
	t_cmd	*new_cmd;

	copy = copy_tokens(start, end);
	if (!copy)
	{
		free_cmd_list(*cmd_list);
		return (NULL);
	}
	new_cmd = create_new_cmd(copy);
	if (!new_cmd)
	{
		free_token_list(copy);
		free_cmd_list(*cmd_list);
		return (NULL);
	}
	if (!*cmd_list)
		*cmd_list = new_cmd;
	else
		(*last_cmd)->next = new_cmd;
	*last_cmd = new_cmd;
	return (*cmd_list);
}

t_cmd	*split_by_pipe(t_token *tokens)
{
	t_cmd		*cmd_list;
	t_cmd		*last_cmd;
	t_token		*current;
	t_token		*start;
	t_token		*end;

	cmd_list = NULL;
	last_cmd = NULL;
	start = tokens;
	current = tokens;
	while (current)
	{
		if (current->type == PIPE || current->next == NULL)
		{
			if (current->type == PIPE)
				end = current;
			else
				end = current->next;
			if (!create_and_copy(start, end, &cmd_list, &last_cmd))
				return (NULL);
			start = current->next;
		}
		current = current->next;
	}
	return (cmd_list);
}
