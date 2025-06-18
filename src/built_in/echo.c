/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 12:39:58 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/18 14:38:12 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_flag(char *str)
{
	if (*str != '-')
		return (0);
	str++;
	while (*str)
	{
		if (*str != 'n')
			return (0);
		str++;
	}
	return (1);
}

void	print_args(char **arg)
{
	int	i;
	int	j;

	i = 0;
	while (arg[i])
	{
		j = 0;
		while (arg[i][j])
		{
			write(1, &arg[i][j], 1);
			j++;
		}
		if (arg[i + 1])
			write(1, " ", 1);
		i++;
	}
}

void	ft_echo(char **arg)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	if (!arg[1])
	{
		printf("\n");
		return ;
	}
	while (arg[i] && check_flag(arg[i]))
	{
		n_flag = 1;
		i++;
	}
	print_args(&arg[i]);
	if (!n_flag)
		printf("\n");
	set_exit_status(0, 1337);
}
