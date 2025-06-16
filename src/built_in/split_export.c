/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 13:07:41 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/16 13:52:36 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"

static int	count_len(char *s, char sep)
{
	int	i;

	i = 0;
	if (!s || (s && !(*s)))
		return (0);
	while (s[i])
	{
		if (s[i] == sep)
		{
			if (s[i + 1])
				return (2);
			else
				return (1);
		}
		i++;
	}
	return (1);
}

static char	*first(char *str, char sep)
{
	int		i;
	char	*first;

	i = 0;
	while (str[i] && str[i] != sep)
		i++;
	first = (char *)malloc(i + 1);
	i = 0;
	while (str[i] && str[i] != sep)
	{
		first[i] = str[i];
		i++;
	}
	first[i] = '\0';
	return (first);
}

static char	*second(char *str, char sep, int j)
{
	int		i;
	int		tack;
	char	*sec;

	i = 0;
	while (str[i] && str[i] != sep)
		i++;
	i++;
	tack = i;
	while (str[i])
	{
		i++;
		j++;
	}
	sec = (char *)malloc(j + 1);
	j = 0;
	while (str[tack])
	{
		sec[j] = str[tack];
		j++;
		tack++;
	}
	sec[j] = '\0';
	return (sec);
}

char	**splite_exp(char *s, char sep)
{
	char	**arr;
	int		len;
	int		i;

	i = 0;
	len = count_len(s, sep);
	arr = (char **)malloc((len + 1) * sizeof(char *));
	while (i < len)
	{
		if (i == 0)
			arr[i] = first(s, sep);
		else
			arr[i] = second(s, sep, 0);
		i++;
	}
	arr[i] = NULL;
	return (arr);
}
