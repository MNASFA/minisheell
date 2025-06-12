/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:13:11 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/12 15:44:00 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"

void	process_6(t_env **env)
{
	t_env	*str;

	str = find_in_env("OLDPWD", *env);
	if (chdir(str->value) == -1)
	{
		perror("bash: cd: ");
		write(2, str->value, ft_strlen(str->value));
		write (2, ":No such file or directory\n", 27);
		set_exit_status(1, 1337);
		return ;
	}
	pwd(*env);
}

void	handle_cd_error(char *arg, char *oldpwd)
{
	write(2, "minishell: cd:", 15);
	write(2, arg, ft_strlen(arg));
	write (2, ":No such file or directory\n", 27);
	set_exit_status(1, 1337);
	free(oldpwd);
}
