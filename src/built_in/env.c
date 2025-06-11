/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:06:53 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/10 16:20:55 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"

void print_env(char **args, t_env *env)
{
	t_env *tmp;

	tmp = env;
	if (args[1])
	{
		write(2, "env: ’", 8);
		write(2, args[1], ft_strlen(args[1]));
		write(2, "’: No such file or directory\n", 31);
		set_exit_status(127, 1337);
		return;
	}
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
