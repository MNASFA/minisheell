/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 21:56:54 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/04/28 11:55:42 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	remove_pipe_node(t_cmd	*cmd_list)
{
	t_cmd	*current_cmd;
	t_token	*tokens;
	t_token	*prev;

	current_cmd = cmd_list;
	while (current_cmd)
	{
		tokens = current_cmd->token;
		prev = NULL;
		while (tokens && tokens->next)
		{
			prev = tokens;
			tokens = tokens->next;
		}
		if (tokens && tokens->type == PIPE)
		{
			if (prev)
				prev->next = NULL;
			free(tokens);
		}
		current_cmd = current_cmd->next;
	}
}

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
			return (NULL);
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

static t_cmd	*add_cmd_to_list(t_split_vars *var, t_token *copy)
{
	t_cmd	*new_cmd;

	new_cmd = create_new_cmd(copy);
	if (!new_cmd)
		return (NULL);
	if (!var->cmd_list)
		var->cmd_list = new_cmd;
	else
		var->current_cmd->next = new_cmd;
	var->current_cmd = new_cmd;
	return (new_cmd);
}

t_cmd	*split_by_pipe(t_token *tokens)
{
	t_split_vars	var;
	t_token			*copy;

	init_split_vars(&var, tokens);
	while (var.end)
	{
		if (var.end->type == PIPE || var.end->next == NULL)
		{
			var.next_token = var.end->next;
			if (var.end->type == PIPE)
				var.end->next = NULL;
			copy = copy_tokens(var.start, var.next_token);
			if (!add_cmd_to_list(&var, copy))
				return (NULL);
			var.start = var.next_token;
			var.end = var.next_token;
		}
		else
			var.end = var.end->next;
	}
	return (var.cmd_list);
}
