/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_help.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 19:41:02 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/12 12:45:15 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	add_redir(t_exec *exec, t_redir *new)
{
	t_redir	*tmp;

	if (!exec->redirections)
		exec->redirections = new;
	else
	{
		tmp = exec->redirections;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

t_exec	*init_exec(int arg_count)
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
	exec->redirections = NULL;
	exec->infd = -2;
	exec->outfd = -2;
	exec->cmd = NULL;
	exec->next = NULL;
	return (exec);
}

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
