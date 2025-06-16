/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:30:14 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/15 15:57:02 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"

static int	is_alpha_or_underscore(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_');
}

static int	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

int	is_valid_name(char *str)
{
	int	i;
	int	plus_count;

	if (!str || !str[0])
		return (0);
	i = 0;
	plus_count = 0;
	while (str[i])
	{
		if (is_alpha_or_underscore(str[i]))
			i++;
		else if (is_digit(str[i]) && i > 0)
			i++;
		else if (str[i] == '+' && plus_count < 1)
		{
			plus_count++;
			i++;
		}
		else
			return (0);
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
