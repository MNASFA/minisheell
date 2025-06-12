/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 19:38:29 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/10 18:26:02 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	detect_delimiter(t_token *tokens)
{
	t_token	*current;
	int		heredoc_count;

	heredoc_count = 0;
	current = tokens;
	while (current)
	{
		if (current->type == HEREDOC && current->next
			&& current->next->type == WORD)
		{
			current->next->type = HEREDOC_DELIMITER;
			heredoc_count++;
		}
		current = current->next;
	}
	return (heredoc_count);
}

static t_exec	*append_exec_nodes(t_cmd *cmds, t_exec **exec_list)
{
	t_exec	*current;
	t_exec	*new_node;

	current = NULL;
	while (cmds)
	{
		new_node = parse_command(cmds, 0, NULL);
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
