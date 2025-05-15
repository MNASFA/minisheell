/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:07:24 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/05/14 23:25:32 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"


t_env	*ft_lstlast(t_env *lst)
{
	if (!lst)
		return (NULL);
	while (lst && lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back_ex(t_env **lst, t_env *new)
{
	if (!lst || !new)
		return ;
	if (*lst)
		ft_lstlast(*lst)->next = new;
	else
		*lst = new;
}

void delete_if_exist(t_env **env, char *to_delete)
{
    if (!env || !*env || !to_delete)
        return;
        
    t_env *current = *env;
    t_env *prev = NULL;
    
    while (current)
    {
        if (strcmp(current->key, to_delete) == 0)
        {
            if (prev)
                prev->next = current->next;
            else
                *env = current->next;
            free(current->key);
            free(current->value);
            free(current->full);
            free(current);
            if (prev)
                current = prev->next;
            else
                current = *env;
        }
        else
        {
            prev = current;
            current = current->next;
        }
    }
}

int is_there_equal(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if(str[i] == '=')
        {
            if (str[i - 1] && str[i - 1] == '+')
                return(2);
            return(1);
        }
        i++;
    }
    return(0);
}
int find_next_min(t_env **env, int *printed, int size)
{
    int min_idx = -1;
    
    for (int j = 0; j < size; j++)
    {
        if (!printed[j])
        {
            if (min_idx == -1 || strcmp(env[j]->full, env[min_idx]->full) < 0)
                min_idx = j;
        }
    }
    return min_idx;
}

void print_sorted(t_env **env)
{
    if (!env)
        return;
    int count = 0;
    t_env *temp = *env;
    while (temp)
    {
        count++;
        temp = temp->next;
    }
    if (count == 0)
        return;
    t_env **env_array = malloc(count * sizeof(t_env*));
    if (!env_array)
        return;   
    temp = *env;
    for (int i = 0; i < count; i++)
    {
        env_array[i] = temp;
        temp = temp->next;
    }
    int *printed = malloc(count * sizeof(int));
    if (!printed)
    {
        free(env_array);
        return;
    }
    
    for (int i = 0; i < count; i++)
        printed[i] = 0;
        
    for (int i = 0; i < count; i++)
    {
        int min_idx = find_next_min(env_array, printed, count);
        if (min_idx != -1 && min_idx < count)
        {
            printf("declare -x %s=\"", env_array[min_idx]->key);
            printf("%s\"\n", env_array[min_idx]->value);
            printed[min_idx] = 1;
        }
    }
    free(printed);
    free(env_array);
}

void update(char *key, t_env **env, char *new)
{
    t_env *copy = *env;
    char *tmp = NULL;

    while (copy && copy->key)
    {
        if (!ft_strcmp(key, copy->key))
        {
            tmp = ft_strdup(copy->value);
            free(copy->value);
            copy->value = ft_strjoin(tmp, new);
            free(tmp);
            if (!copy->value)
                return;
            if (copy->is_print == 0)
            {
                tmp = ft_strjoin(copy->key, "=");
                if (!tmp)
                    return;
                free(copy->full);
                copy->full = ft_strjoin(tmp, copy->value);
                free(tmp);
                if (!copy->full)
                    return;
                copy->is_print = 1;
            }
            else
            {
                tmp = ft_strdup(copy->full);
                if (!tmp)
                    return;
                free(copy->full);
                copy->full = ft_strjoin(tmp, new);
                free(tmp);
                if (!copy->full)
                    return;
            }
            break;
        }
        copy = copy->next;
    }
}

int is_in_env(t_env *env, char *key)
{
    while(env)
    {
        if(!ft_strcmp(env->key, key))
            return (1);
        env = env->next;
    }
    return (0);
}
int is_valid_name(char *str)
{
    int i;

    i = 0;

    while (str[i])
    {
        if (!((str[i] >= 65 && str[i] <= 90) || (str[i] >= 97 && str[i] <= 122) || str[i] == 95))
        {
            if (str[i] >= 48 && str[i] <= 57 && i > 0)
            {
                i++;
                continue;
            }
            return (0);
        }
        i++;
    }
    return(1); 
}
int check_printable(char **args)
{
    int i;

    i = 1;
    while(args[i])
    {
        if (args[i][0])
            return(1);
        i++;
    }
    return(0);
    
}

void ft_export(char **args, t_env **env)
{
    int i = 1;
    if (!args || !env)
        return;
    if(!check_printable(args))
    {
        print_sorted(env);
        return;
    }
    while(args[i])
    {
        if(is_there_equal(args[i]) == 1)
        {
            char **str = ft_split_exe(args[i], '=');
            if (!is_valid_name(str[0]))
            {
                printf("minishell: export: `%s': not a valid identifier\n", args[i]);
                set_exit_status(1, 1337);
                int j = 0;
                while(str[j])
                    free(str[j++]);
                i++;
                continue;
            }
            if (!str)
            {
                perror("bash: export : ");
                printf(" `%s': not a valid identifier", args[i]);
                continue;
            }
            t_env *new = malloc(sizeof(t_env));
            if (!new)
            {
                int j = 0;
                while(str[j])
                    free(str[j++]);
                free(str);
                continue;
            }
            new->key = ft_strdup(str[0]);
            new->value = ft_strdup(str[1]);
            new->full = ft_strdup(args[i]);
            new->is_print = 1;
            new->next = NULL;
            delete_if_exist(env, str[0]);
            ft_lstadd_back_ex(env, new);
            int j = 0;
            while(str[j])
                free(str[j++]);
            free(str);
        }
        else if (is_there_equal(args[i]) == 2)
        {
           char **str = ft_split_exe(args[i], '+');
           if (!is_valid_name(str[0]))
            {
                printf("minishell: export: `%s': not a valid identifier\n", args[i]);
                set_exit_status(1, 1337);
                int j = 0;
                while(str[j])
                    free(str[j++]);
                i++;
                continue;
            }
           update(str[0], env,str[1]+1);
        }
        else
        {
            if (!is_valid_name(args[i]))
            {
                printf("minishell: export: `%s': not a valid identifier\n", args[i]);
                set_exit_status(1, 1337);
                i++;
                continue;
            }
            if (is_in_env(*env, args[i]) == 1)
            {
                i++;
                continue;
            }
            t_env *new = malloc(sizeof(t_env));
            if (!new)
                return;
            new->key = ft_strdup(args[i]);
            new->full = ft_strdup(args[i]);
            new->value = NULL;
            new->next = NULL;
            new->is_print = 0;
            delete_if_exist(env, args[i]);
            ft_lstadd_back_ex(env, new);
        }
        i++;
    }
}
