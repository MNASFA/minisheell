/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:17:10 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/12 19:35:18 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"

void	handle_no_numeric(char **args, t_env **env, t_exee **exe)
{
	int	i;

	i = 0;
	if (args[1][i] == '+' || args[1][i] == '-')
		i++;
	while (args[1][i])
	{
		if (!(args[1][i] >= '0' && args[1][i] <= '9'))
		{
			write(2, "minishell: exit: ", 17);
			write(2, args[1], ft_strlen(args[1]));
			write(2, ": numeric argument required\n", 28);
			free_envir(*env);
			cleanup_fds(exe);
			exit(set_exit_status(2, 1337));
		}
		i++;
	}
}

void	norm(char **args, t_env **env, t_exee **exe)
{
	write(2, "bash: exit: ", 12);
	write(2, args[1], ft_strlen(args[1]));
	write(2, ": numeric argument required\n", 29);
	free_envir(*env);
	cleanup_fds(exe);
	exit(set_exit_status(2, 1337));
}

void	cleanup_fds(t_exee **exe)
{
	cleanup_pwd();
	safe_close(&(*exe)->infile);
	safe_close(&(*exe)->outfile);
	safe_close(&(*exe)->saved_in);
	safe_close(&(*exe)->saved_out);
}
