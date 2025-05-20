/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envirement.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:37:51 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/19 12:39:08 by aboukhmi         ###   ########.fr       */
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


char **env_no_env()
{
	char **args;
	char *str;
	char *temp = NULL;
	
	str = getcwd(0, 0);
	if (str)
	{
		args = malloc(5 * sizeof(char *));
		temp = ft_strjoin("PWD=", str);
		args[0] = ft_strdup(temp);
		free(temp);
		free(str);
		args[1] = ft_strdup("SHLVL=1");
		args[2] = ft_strdup("_=/usr/bin/env");
		args[3] = ft_strdup("PATH=/home/aboukhmi/bin:/usr/local/sbin:/usr/local/bin\
:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin");
		args[4] = NULL;
	}
	else
	{
		args = malloc(4 * sizeof(char *));
		args[0] = ft_strdup("SHLVL=1");
		args[1] = ft_strdup("_=/usr/bin/env");
		args[2] = ft_strdup("PATH=/home/aboukhmi/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin");
		args[3] = NULL;
	}
	return (args);
}

t_env *init_env(char **envp)
{
    t_env *head = NULL;
    t_env *current = NULL;
    t_env *new_var;
    int i = 0;
    int lo = 0; // Initialize lo to 0

    if (envp && !envp[0])
    {
        lo = 1;
        envp = env_no_env(); // Ensure this is freed later
    }

    while (envp && envp[i]) // Check if envp is not NULL
    {
        new_var = create_env_var(envp[i]);
        if (!new_var)
        {
            free_env_list(head); // Free previously allocated nodes
            if (lo == 1)
                freeee(envp); // Free envp if it was allocated
            return NULL;
        }

        new_var->is_first = 1;
        new_var->is_print = 1;
        new_var->last_exit_status = set_exit_status(1337, -1);

        if (!head)
            head = new_var;
        else
            current->next = new_var;

        current = new_var;
        i++;
    }

    if (lo == 1)
        freeee(envp); // Free envp if it was allocated

    return head;
}

void debug(t_env **env, char *arr) {
	
	t_env *tmp = *env;
	while (tmp)
	{
		printf("%p|%s\n", tmp->full, tmp->full);
		printf("%p, ", tmp->key);
		printf("%p, \n", tmp->value);
		printf("%s\n", arr);
		tmp = tmp->next;
	}
}

char	*get_env_value(t_env *env, char *key)
{
	if (!env || !key || *key == '\0')
		return ("");
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return ("");
}

static int	calc_varname_len(char *str, int i)
{
	int	len;

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

int	copy_var_name(char *str, int i, int var_len, char **var_name)
{
	int	j;

	j = 0;
	if (str[i] == '{')
	{
		i++;
		while (str[i] && str[i] != '}' && j < var_len)
			(*var_name)[j++] = str[i++];
		(*var_name)[j] = '\0';
		if (str[i] == '}')
			i++;
	}
	else
	{
		while (str[i] && (ft_isalnum(str[i]) || str[i] == '_') && j < var_len)
			(*var_name)[j++] = str[i++];
		(*var_name)[j] = '\0';
	}
	return (i);
}

int	extract_var_name(char *str, int i, char **var_name)
{
	int	var_len;
	int	start_i;

	start_i = i;
	if (str[i] == '$')
		i++;
	if (!str[i] || !(ft_isalnum(str[i]) || str[i] == '_' || str[i] == '{'))
	{
		*var_name = NULL;
		return (start_i + 1);
	}
	var_len = calc_varname_len(str, i);
	if (var_len == 0)
	{
		*var_name = NULL;
		return (i);
	}
	*var_name = malloc(var_len + 1);
	if (!*var_name)
		return (i);
	return (copy_var_name(str, i, var_len, var_name));
}
