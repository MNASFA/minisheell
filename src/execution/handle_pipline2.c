/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipline2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:49:43 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/12 13:36:31 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	setup_pipes(t_exee *exee)
{
	int	i;

	i = 0;
	if (!exee || exee->cmd_count <= 1)
		return ;
	while (i < exee->cmd_count - 1)
	{
		if (!exee->pipes[i])
		{
			exee->pipes[i] = malloc(sizeof(int) * 2);
			if (!exee->pipes[i])
			{
				perror("malloc failed for pipe");
				exit(set_exit_status(1, 1337));
			}
		}
		if (pipe(exee->pipes[i]) < 0)
		{
			perror("pipe creation failed");
			exit(set_exit_status(1, 1337));
		}
		i++;
	}
}
