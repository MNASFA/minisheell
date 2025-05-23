/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 09:05:33 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/23 09:05:34 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

static size_t	ft_numlen(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*num_str;
	size_t	num_len;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n == 0)
		return (ft_strdup("0"));
	num_len = ft_numlen(n);
	num_str = (char *) malloc(sizeof(char) * (num_len + 1));
	if (num_str == NULL)
		return (NULL);
	num_str[num_len] = '\0';
	if (n < 0)
	{
		num_str[0] = '-';
		n = -n;
	}
	while (n != 0 && num_str[num_len] != '-')
	{
		num_str[num_len - 1] = (n % 10) + 48;
		n /= 10;
		num_len--;
	}
	return (num_str);
}
