/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 09:58:22 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/26 16:58:50 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_redir(t_redir *redir)
{
	if (!redir)
		return ;
	free(redir->filename);
	if (redir->type == HEREDOC)
	{
		free(redir->delimiter);
		if (redir->herdoc_fd > 0)
			close(redir->herdoc_fd);
	}
	free(redir);
}

void	free_redir_list(t_redir *list)
{
	t_redir	*tmp;

	while (list)
	{
		tmp = list->next;
		free_redir(list);
		list = tmp;
	}
}

void	free_exec(t_exec *exec)
{
	if (!exec)
		return ;
	if (exec->args)
		free_split(exec->args);
	free_redir_list(exec->redirections);
	free(exec);
}

void	free_exec_list(t_exec *exec_list)
{
	t_exec	*current;
	t_exec	*next;

	current = exec_list;
	while (current)
	{
		next = current->next;
		free_exec(current);
		current = next;
	}
}

void	freeee(char **str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i] != NULL)
	{
		free(str[i]);
		i++;
	}
	free(str);
}
