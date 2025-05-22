/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:06:53 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/05/19 12:31:54 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"

void print_env(t_env *env)
{
	t_env *tmp = env;
	while (tmp)
	{
		printf("%p|%s\n", tmp->full, tmp->full);
		printf("%p, ", tmp->key);
		printf("%p, \n", tmp->value);
		printf("----");
		tmp = tmp->next;
	}
    while (env)
    {
        if (env->is_print == 1 && env->full != NULL)
            printf("%s\n", env->full);
        env = env->next;
    }
}
