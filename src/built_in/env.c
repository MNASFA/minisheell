/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:06:53 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/17 17:19:28 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"

void	print_env(char **args, t_env *env)
{
	if (!env)
		return;
	if (args[1])
	{
		write(2, "env: ’", 8);
		write(2, args[1], ft_strlen(args[1]));
		write(2, "’: No such file or directory\n", 31);
		set_exit_status(127, 1337);
		return ;
	}
	while (env)
	{
		if (env && env->is_print == 1 && env->full != NULL)
			printf("%s\n", env->full);
		env = env->next;
	}
	set_exit_status(0, 1337);
}
