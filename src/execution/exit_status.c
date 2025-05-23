/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 21:54:23 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/05/23 15:13:08 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"


int set_exit_status(int num, int flag)
{
	static int  exit_status;

	exit_status = 0;
	if (flag != -1)
		exit_status = num;
	return (exit_status);
}

int	sizeofnewargs(char **args)
{
	int		i;
	int		j;
	int		count;
	char	**str;

	i = 0;
	count = 0;
	if (!args)
		return (0);
	while (args[i])
	{
		str = ft_split_exe(args[i], ' ');
		j = 0;
		while(str[j])
		{
			count++;
			j++;
		}
		freeee(str);
		i++;
	}
	return (count);
}

char **renew_args(char **args)
{
	int	size;
	char **new;
	char **str;
	int		i;
	int		j;
	int		k;

	i = 0;
	k = 0;
	size = sizeofnewargs(args);
	new = (char **)malloc(sizeof(char *) * (size + 1));
	while (args[i])
	{
		str = ft_split_exe(args[i], ' ');
		j = 0;
		while(str[j])
		{
			new[k] = ft_strdup(str[j]);

			k++;
			j++;
		}
		freeee(str);
		i++;
	}
	new[k] = NULL;
	freeee(args);
	return (new);
}
