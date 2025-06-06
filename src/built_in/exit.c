/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:31:09 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/06 17:29:17 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"

static long long	ft_atoi2(const char *str, int *flag)
{
	long long	prev;
	long long	result;
	int			sign;
	int			i;

	prev = 0;
	result = 0;
	sign = 1;
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i++] == '-')
			sign *= -1;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		prev = result;
		result = (result * 10) + (sign * (str[i++] - '0'));
		if ((result / 10) != prev)
			return (*flag = -1, 0);
	}
	return (result);
}
int is_only_numeric(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return(0);
		i++;
	}
	return (1);
}
int	ft_exit(char **args, int last_status, t_env **env)
{
	int	i;
	long long	exit_code;
	int flag = 0;

	(void)last_status;
	i = 0;
	if (args[0] && args[1] && args[2])
	{
		if(is_only_numeric(args[1]))
			return (printf("exit\nminishell: exit: too many arguments\n"), set_exit_status(1, 1337));
		else
		{
			write (2, "bash: exit: ", 12);
			write(2, args[1], ft_strlen(args[1]));
			write(2, ": numeric argument required\n", 29);
			set_exit_status(2, 1337);
			free_envir(*env);
			exit(2);
		}
	}
	if (args[1])
	{
		if (args[1][i] == '+' || args[1][i] == '-')
			i++;
		while (args[1][i])
		{
			if (!(args[1][i] >= '0' && args[1][i] <= '9'))
			{
				printf("exit\nminishell: exit: %s: numeric argument required\n", args[1]);
				free_envir(*env);
				exit(2);
			}
			i++;
		}
		exit_code = ft_atoi2(args[1], &flag);
		if (flag == -1)
		{
			write(2, "bash: exit: ", 12);
			write(2, args[1], ft_strlen(args[1]));
			write(2, ": numeric argument required\n", 29);
			exit(set_exit_status(2, 1337));
		}
		exit_code = exit_code % 256;
		if (exit_code < 0)
			exit_code += 256;
		free_envir(*env);
		printf("exit\n");
		exit(exit_code);
	}
	printf("exit\n");
	free_envir(*env);
	exit(12);
}
