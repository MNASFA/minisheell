/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_vars.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 11:52:48 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/04/28 13:40:24 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// split_by_pipe init

void	init_split_vars(t_split_vars *var, t_token *tokens)
{
	var->start = tokens;
	var->end = tokens;
	var->cmd_list = NULL;
	var->current_cmd = NULL;
	var->next_token = NULL;
}

// init expanding

void init_expand_vars(t_expand_vars *vars, char *str, t_env *env, int i, int j)
{
    vars->str = str;
    vars->i = i;
    vars->j = j;
    vars->env = env;
    vars->in_double = 0;
    vars->in_single = 0;
}

