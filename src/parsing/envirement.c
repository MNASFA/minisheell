/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envirement.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:37:51 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/10 20:27:57 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_env	*create_env_var(char *env_start)
{
	t_env	*new_var;
	char	*equal;

	equal = ft_strchr(env_start, '=');
	if (!equal)
		return (NULL);
	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return (NULL);
	new_var->key = ft_strndup(env_start, equal - env_start);
	new_var->value = ft_strdup(equal + 1);
	new_var->full = ft_strdup(env_start);
	new_var->next = NULL;
	return (new_var);
}

t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*current;
	t_env	*new_var;
	int		i;

	head = NULL;
	current = NULL;
	i = 0;
	while (envp[i])
	{
		new_var = create_env_var(envp[i]);
		new_var->is_first = 1;
		new_var->is_print = 1;
		if (!new_var)
		{
			free_env_list(head);
			return (NULL);
		}
		if (!head)
			head = new_var;
		else
			current->next = new_var;
		current = new_var;
		i++;
	}
	return (head);
}

char	*get_env_value(t_env *env, char *key)
{
	if (!key || *key == '\0')
		return ("");
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return ("");
}

static int calc_varname_len(char *str, int i)
{
	int len;
	
	len = 0;
	if (str[i] == '{')
	{
		i++;
		while (str[i] && str[i] != '}')
		{
			len++;
			i++;
		}
	}
	else
	{
		while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		{
			len++;
			i++;
		}
	}
	return (len);
}

// int	extract_var_name(char *str, int i, char **var_name)
// {
// 	int	j;
// 	int var_len;

// 	j = 0;
// 	if (str[i] == '$')
// 		i++;
// 	var_len = calc_varname_len(str, i);
// 	*var_name = malloc(var_len + 1);
// 	if (!*var_name)
// 		return (i);
// 	if (str[i] == '{')
// 	{
// 		i++;
// 		while (str[i] && str[i] != '}')
// 			(*var_name)[j++] = str[i++];
// 		(*var_name)[j] = '\0';
// 		if (str[i] == '}')
// 			i++;
// 	}
// 	else
// 	{
// 		while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
// 			(*var_name)[j++] = str[i++];
// 		(*var_name)[j] = '\0';
// 	}
// 	return (i);
// }

// Fixed extract_var_name to be safer with memory
int extract_var_name(char *str, int i, char **var_name)
{
    int j = 0;
    int var_len;
    int start_i = i;

    // Skip past $ character
    if (str[i] == '$')
        i++;
    
    // Check for valid first character of variable name
    if (!str[i] || !(ft_isalnum(str[i]) || str[i] == '_' || str[i] == '{'))
    {
        *var_name = NULL;
        return start_i + 1; // Return just past the $
    }
    
    var_len = calc_varname_len(str, i);
    if (var_len == 0)
    {
        *var_name = NULL;
        return i;
    }
    
    *var_name = malloc(var_len + 1);
    if (!*var_name)
        return i;
    
    if (str[i] == '{')
    {
        i++; // Skip the {
        while (str[i] && str[i] != '}' && j < var_len)
            (*var_name)[j++] = str[i++];
        
        (*var_name)[j] = '\0';
        if (str[i] == '}')
            i++; // Skip the closing }
    }
    else
    {
        while (str[i] && (ft_isalnum(str[i]) || str[i] == '_') && j < var_len)
            (*var_name)[j++] = str[i++];
        
        (*var_name)[j] = '\0';
    }
    
    return i;
}
