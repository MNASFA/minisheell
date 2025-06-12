/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:30:14 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/12 15:44:33 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"

int	is_valid_name(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	while (str[i])
	{
		if (!((str[i] >= 65 && str[i] <= 90)
				|| (str[i] >= 97 && str[i] <= 122) || str[i] == 95))
		{
			if (str[i] >= 48 && str[i] <= 57 && i > 0)
			{
				i++;
				continue ;
			}
			return (0);
		}
		i++;
	}
	return (1);
}

int	check_printable(char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (args[i][0] != '\0')
			return (1);
		i++;
	}
	return (0);
}
