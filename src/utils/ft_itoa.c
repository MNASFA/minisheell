/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 09:05:33 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/11 11:58:16 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	len_n(long long int n)
{
	int	len;

	len = 0;
	if (n == 0)
		len = 1;
	if (n < 0)
	{
		len ++;
		n = -n;
	}
	while (n > 0)
	{
		len++;
		n = n / 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	int				len;
	long long int	temp;
	char			*ptr;

	temp = n;
	len = len_n(temp);
	ptr = (char *)malloc(len + 1);
	if (!ptr)
		return (NULL);
	ptr[len] = '\0';
	if (temp == 0)
		ptr[0] = '0';
	if (temp < 0)
	{
		ptr[0] = '-';
		temp = -temp;
	}
	while (temp > 0 && --len >= 0)
	{
		ptr[len] = (temp % 10) + '0';
		temp = temp / 10;
	}
	return (ptr);
}
