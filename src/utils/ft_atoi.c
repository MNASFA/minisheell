/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 11:48:22 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/11 11:49:28 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	check_sign(char str, int sign)
{
	if (str == '-')
		sign = -1;
	return (sign);
}

int	ft_atoi(const char *str)
{
	int			i;
	int			sign;
	long		res;
	long		temp;

	i = 0;
	sign = 1;
	res = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
		sign = check_sign (str[i++], sign);
	while (str[i] >= 48 && str[i] <= 57)
	{
		temp = res;
		res = res * 10 + (sign * (str[i] - 48));
		if ((res / 10) != temp && sign == 1)
			return (-1);
		else if ((res / 10) != temp && sign == -1)
			return (0);
		i++;
	}
	return (res);
}
