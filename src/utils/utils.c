/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 15:01:07 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/16 20:07:06 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "../../minishell.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*buffer_dest;
	unsigned char	*buffer_src;

	if (dest == src)
		return (dest);
	buffer_dest = (unsigned char *) dest;
	buffer_src = (unsigned char *) src;
	i = 0;
	while (i < n)
	{
		buffer_dest[i] = buffer_src[i];
		i++;
	}
	return (dest);
}

int		ft_strcmp(char *s1, char *s2)
{
	int i;
	if(!s1 || !s2)
		return(1);
	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
	{
		i++;
	}
	return (s1[i] - s2[i]);
}

char	*ft_strdup(const char *str)
{
	char	*ptr;
	int		len;

	if (!str)
		return(NULL);
	len = ft_strlen (str);
	ptr = (char *) malloc((len + 1) * sizeof (char));
	if (ptr == NULL)
		return (NULL);
	ft_memcpy(ptr, (char *) str, len + 1);
	return (ptr);
}

char	*ft_strndup(const char *str, size_t n)
{
	char	*ptr;
	size_t	len;

	len = 0;
	while (str[len] && len < n)
		len++;
	ptr = (char *)malloc((len + 1) * sizeof(char));
	if (!ptr)
		return (NULL);
	ptr[len] = '\0';
	while (len--)
		ptr[len] = str[len];
	return (ptr);
}

char	*ft_strchr(const char *s, int c)
{
	char	d;
	int		i;

	d = (char) c;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == d)
			return ((char *) &s[i]);
		i++;
	}
	if (s[i] == d)
		return ((char *) &s[i]);
	return (NULL);
}

int	ft_isalnum(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || (c >= 48 && c <= 57))
		return (1);
	return (0);
}

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	if (!str)
		return (0);
	while (str[len] != '\0')
		len ++;
	return (len);
}

char    *ft_strcpy(char *s1, char *s2)
{
    int i = 0;
	
	if (!s2)
		return(NULL);
    while(s2[i] != '\0')
    {
        s1[i] = s2[i];
        i++;
    }
    s1[i] = '\0';
    return (s1);
}

static void	ft_joined(char *new_str, char const *s1, char const *s2)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (s1[i] != '\0')
	{
		new_str[j] = s1[i];
		i++;
		j++;
	}
	i = 0;
	while (s2[i] != '\0')
	{
		new_str[j] = s2[i];
		i++;
		j++;
	}
	new_str[j] = '\0';
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		total_len;
	char	*new_str;

	if (!s1 && !s2)
		return (NULL);
	if (s1 == NULL)
		return (ft_strdup(s2));
	else if (s2 == NULL)
		return (ft_strdup(s1));
	total_len = ft_strlen(s1) + ft_strlen(s2);
	new_str = (char *) malloc ((total_len + 1) * sizeof(char));
	if (new_str == NULL)
		return (NULL);
	ft_joined(new_str, s1, s2);
	return (new_str);
}


int	check_sign(char str, int sign)
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

char *ft_substr(char const *s, unsigned int start, size_t len)
{
    char *sub;
    size_t i;

    if (!s)
        return (NULL);
    if (start >= ft_strlen(s))
        return (ft_strdup(""));
    sub = malloc(len + 1);
    if (!sub)
        return (NULL);
    i = 0;
    while (i < len && s[start + i])
    {
        sub[i] = s[start + i];
        i++;
    }
    sub[i] = '\0';
    return (sub);
}
