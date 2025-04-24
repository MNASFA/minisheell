/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 09:58:22 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/04/23 17:32:55 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_split(char	**split)
{
	int i;
	
	if (!split)
		return;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	free_token(t_token *token)
{
	t_token *next;
	while (token)
	{
		next = token->next;
		free(token->value);
		free(token);
		token = next;
	}
}

void	free_cmd_list(t_cmd *cmd)
{
	t_cmd	*tmp;
	while (cmd)
	{
		free_token(cmd->token);
		tmp = cmd;
		cmd = cmd->next;
		free(tmp);
	}
}

void	free_redir_list(t_redir *out_in)
{
	t_redir *tmp;
	
	while (out_in)
	{
		tmp = out_in->next;
		free(out_in->filename);
		free(out_in);
		out_in = tmp;
	}
}

void	free_env_list(t_env *env)
{
	t_env *tmp;
	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

void	free_exec_list(t_exec *exec_list)
{
	t_exec	*current;
	t_exec	*next;

	current = exec_list;
	while (current)
	{
		next = current->next;
		if (current->args)
			free_split(current->args);
		free(current->delimiter);
		free_redir_list(current->infiles);
		free_redir_list(current->outfiles);
		free(current);
		current = next;
	}
}
