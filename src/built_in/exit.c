/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:31:09 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/14 22:52:15 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"

int		ft_exit(char **args, int last_status)
{
	int i;

	i = 0;
	if (args[0] && args[1] && args[2])
	{
		printf("minishell: exit: to many arguments\n");
		return (2);
	}
	if (args[1])
	{
		while (args[i])
		{
			if (!(args[1][i] >= '0' && args[1][i] <= '9'))
			{
				printf("exit\nminishell: exit: %s numeric argument required\n", args[1]);
				set_exit_status(2, 1337);
			}
			i++;
		}
		exit(set_exit_status(ft_atoi(args[1]), 1337));
	}
	exit(set_exit_status(last_status, 1337));
}
