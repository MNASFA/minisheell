/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:31:47 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/15 14:34:33 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	wait_for_pipeline_children(t_exee *exee)
{
	int	status;
	int	i;

	i = 0;
	while (i < exee->cmd_count)
	{
		if (exee->pids[i] > 0)
		{
			waitpid(exee->pids[i], &status, 0);
			if (i == exee->cmd_count - 1)
			{
				if (WIFEXITED(status))
					set_exit_status(WEXITSTATUS(status), 0);
				if (WIFSIGNALED(status))
					set_exit_status(WTERMSIG(status) + 128, 0);
			}
		}
		i++;
	}
}

static void	wait_for_single_child(void)
{
	int	status;

	while (waitpid(-1, &status, 0) != -1)
	{
		if (WIFEXITED(status))
			set_exit_status(WEXITSTATUS(status), 0);
		if (WIFSIGNALED(status))
			set_exit_status(WTERMSIG(status) + 128, 0);
	}
}

void	wait_for_all_children(t_exee *exee)
{
	if (exee && exee->cmd_count > 1)
		wait_for_pipeline_children(exee);
	else
		wait_for_single_child();
}
