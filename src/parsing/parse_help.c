/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_help.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 19:41:02 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/18 17:59:50 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Helper function for process expanssion (start) ==> prepare_commands

int	is_pure_variable(char *str)
{
	int	i;
	int	dollars;

	if (!str || str[0] != '$')
		return (0);
	dollars = count_dollars(str, 0);
	i = dollars;
	if (str[i] == '\0')
		return (0);
	if (str[i] == '?' && dollars == 1)
		return (1);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	copy_without_quotes(const char *src, char *dst, int *removed)
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

// Helper function for process expanssion (end)

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
