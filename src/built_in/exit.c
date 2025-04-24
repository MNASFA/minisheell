/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:31:09 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/04/16 09:48:29 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int		ft_exit(char **args, int last_status)
{
	int i;

	i = 0;
	if (args[0] && args[1] && args[2])
	{
		printf("minishell: exit: to many arguments\n");
		return (2);
	}
	if (arg[1])
	{
		while (args[i])
		{
			if (!(args[1][i] >= '0' && args[1][i] <= '9'))
			{
				printf("exit\nminishell: exit: %s numeric argument required\n", args[1]);
				exit(2);
			}
			i++;
		}
		exit(ft_atoi(args[1]));
	}
	exit(last_status);
}
