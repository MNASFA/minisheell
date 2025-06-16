/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:31:09 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/15 14:40:00 by aboukhmi         ###   ########.fr       */
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

int	is_only_numeric(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

int	handle_multiple(char **args, t_env **env, t_exee **exe)
{
	if (is_only_numeric(args[1]))
		return (ft_putstr_fd("exit\nminishell: exit: too many arguments\n", 2)
			, set_exit_status(1, 1337));
	else
	{
		write (2, "bash: exit: ", 12);
		write(2, args[1], ft_strlen(args[1]));
		write(2, ": numeric argument required\n", 29);
		cleanup_fds(exe);
		free_envir(*env);
		exit(set_exit_status(2, 1337));
	}
}

void	safe_printf(void)
{
	if (isatty(STDIN_FILENO))
		printf("exit\n");
}

int	ft_exit(t_exec **cmd, int last_status, t_env **env, t_exee **exe)
{
	long long	exit_code;
	int			flag;

	flag = 0;
	(void)last_status;
	if ((*cmd)->args[0] && (*cmd)->args[1] && (*cmd)->args[2])
		return (handle_multiple((*cmd)->args, env, exe));
	if ((*cmd)->args[1])
	{
		handle_no_numeric((*cmd)->args, env, exe);
		exit_code = ft_atoi2((*cmd)->args[1], &flag);
		if (flag == -1)
			norm((*cmd)->args, env, exe);
		exit_code = exit_code % 256;
		if (exit_code < 0)
			exit_code += 256;
		free_envir(*env);
		safe_printf();
		cleanup_fds(exe);
		exit(set_exit_status(exit_code, 1337));
	}
	free_envir(*env);
	cleanup_fds(exe);
	safe_printf();
	exit(0);
}
