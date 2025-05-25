/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 09:58:22 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/24 20:01:45 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_split(char	**split)
{
	int	i;

	if (!split)
		return ;
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
	t_token	*next;

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

void	free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

void free_redir(t_redir *redir)
{
    if (!redir)
		return;
    free(redir->filename);
    if (redir->is_herdoc) 
	{
        free(redir->delimiter);
		if (redir->herdoc_fd > 0)
			close(redir->herdoc_fd);
    }
    free(redir);
}

void free_redir_list(t_redir *list)
{
    t_redir *tmp;
    while (list)
    {
        tmp = list->next;
        free_redir(list);
        list = tmp;
    }
}

void free_exec(t_exec *exec)
{
    if (!exec)
		return;
    if (exec->args)
        free_split(exec->args);
    free_redir_list(exec->infiles);
    free_redir_list(exec->outfiles);
    free(exec);
}

void free_exec_list(t_exec *exec_list)
{
    t_exec *current;
    t_exec *next;

    current = exec_list;
    while (current)
    {
        next = current->next;
        free_exec(current);
        current = next;
    }
}

void free_token_list(t_token *head)
{
    t_token *tmp;

    while (head)
    {
        tmp = head;
        head = head->next;
        if (tmp->value)
            free(tmp->value);  // Only if value was dynamically allocated
        free(tmp);
    }
}