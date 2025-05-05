/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:07:24 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/05/01 19:07:25 by aboukhmi         ###   ########.fr       */
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
            return(1);
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
            printf("declare -x %s\n", env_array[min_idx]->full);
            printed[min_idx] = 1;
        }
    }
    free(printed);
    free(env_array);
}

void ft_export(char **args, t_env **env)
{
    int i = 1;
    if (!args || !env)
        return;
    if(args[0] && !args[1])
    {
        print_sorted(env);
        return;
    }
    while(args[i])
    {     
        if(is_there_equal(args[i]))
        {
            char **str = ft_split_exe(args[i], '=');
            if (!str)
                continue;   
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
            new->next = NULL;
            delete_if_exist(env, str[0]);
            ft_lstadd_back_ex(env, new);
            int j = 0;
            while(str[j])
                free(str[j++]);
            free(str);
        }
        i++;
    }
}

// int main(void)
// {
//     t_env *env_list = NULL;
//     char *args1[] = { "export", "VAR1=hello", "VAR2=world", NULL };
//     ft_export(args1, &env_list);
//     printf("i'm here\n");

//     printf("First export:\n");
//     print_sorted(&env_list);
//     char *args2[] = { "export", "VAR1=newvalue", "VAR3=test", NULL };
//     ft_export(args2, &env_list);

//     printf("\nAfter updating VAR1 and adding VAR3:\n");
//     print_sorted(&env_list);
//     while (env_list)
//     {
//         t_env *tmp = env_list;
//         env_list = env_list->next;
//         free(tmp->key);
//         free(tmp->value);
//         free(tmp->full);
//         free(tmp);
//     }

//     return 0;
// }