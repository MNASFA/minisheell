/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spliiit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 13:45:09 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/12 13:45:23 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	count_words(char *s, char sep)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if ((s[i] != sep && s[i] != 9) && ((s[i + 1] == sep
					|| s[i + 1] == 9) || s[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

static int	word_len(char *s, char sep)
{
	int	i;

	i = 0;
	while (s[i] != '\0' && s[i] != sep && s[i] != 9)
		i++;
	return (i);
}

static char	**free_mem(char **c, int i)
{
	int	index;

	index = 0;
	while (index < i)
	{
		free(c[index]);
		index++;
	}
	free(c);
	return (NULL);
}

static size_t	ft_strlcpy(char *dest, char *src, size_t size)
{
	size_t	i;

	if (src && size != 0)
	{
		i = 0;
		while (i < size - 1 && src[i] != '\0')
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (ft_strlen(src));
	return (0);
}

char	**ft_split_exe(char *s, char c)
{
	char	**arr;
	int		i;
	char	*str;

	i = 0;
	str = (char *)s;
	arr = (char **)malloc(sizeof(char *) * (count_words(str, c) + 1));
	if (!arr)
		return (NULL);
	while (i < count_words((char *)s, c))
	{
		while (*str == c)
			str++;
		arr[i] = (char *)malloc(sizeof(char) *(word_len(str, c) + 1));
		if (!arr[i])
			return (free_mem(arr, i));
		ft_strlcpy(arr[i], str, (word_len(str, c) + 1));
		str = str + word_len(str, c);
		i++;
	}
	arr[i] = NULL;
	return (arr);
}
