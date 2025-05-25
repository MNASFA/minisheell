/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 21:56:54 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/25 12:07:52 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_token	*copy_tokens(t_token *start, t_token *end)
{
	t_token		*copy_start;
	t_token		*copy_current;
	t_token		*new_token;
	int			track;

	copy_start = NULL;
	copy_current = NULL;
	track = start->var_in_quotes;
	while (start != end && start != NULL)
	{
		new_token = create_token(start->value);
		if (!new_token)
			return (free_token_list(copy_start), NULL);
		new_token->type = start->type;
		new_token->quoted_flag = start->quoted_flag;
		new_token->var_in_quotes = track;
		new_token->expanded_flag = start->expanded_flag;
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

t_cmd	*split_by_pipe(t_token *tokens)
{
	t_cmd		*cmd_list = NULL;
	t_cmd		*last_cmd = NULL;
	t_token		*start = tokens;
	t_token		*current = tokens;
	t_token		*copy;
	t_cmd		*new_cmd;

	while (current)
	{
		if (current->type == PIPE || current->next == NULL)
		{
			// Determine end of current command
			t_token *end = (current->type == PIPE) ? current : current->next;

			// Copy tokens between start and end
			copy = copy_tokens(start, end);
			if (!copy)
			{
				free_cmd_list(cmd_list);
				return (NULL);
			}

			// Create and link new command node
			new_cmd = create_new_cmd(copy);
			if (!new_cmd)
			{
				free_token_list(copy);
				free_cmd_list(cmd_list);
				return (NULL);
			}
			if (!cmd_list)
				cmd_list = new_cmd;
			else
				last_cmd->next = new_cmd;
			last_cmd = new_cmd;

			// Move to next segment
			start = current->next;
		}
		current = current->next;
	}
	return (cmd_list);
}



