/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 21:54:23 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/15 17:36:12 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	set_exit_status(int num, int flag)
{
	static int	exit_status;

	if (flag != -1)
		exit_status = num;
	return (exit_status);
}

void	cle_env_fds(t_env **env)
{
	safe_close(&(*env)->fd_in);
	safe_close(&(*env)->fd_out);
}
