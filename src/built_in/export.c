#include "../execution/execution.h"


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
	if (env == NULL || *env == NULL)
		return;

	t_env *cur = *env;

	if (ft_strcmp(cur->key, to_delete))
	{
		*env = cur->next;
		free(cur);
		free_one_node(env, to_delete);
	}
	cur = *env;
	free_one_node(&cur->next, to_delete);
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
void  ft_export(char **args, t_env **env)
{
    char **str;
    t_env *new;
    int     i;

    i = 1;
    if (!args || !env)
        return;
    while(args[i])
    {
        
        if(is_there_equal(args[i]))
        {
            str = ft_split_exe(args[i], '=');
            if (!str)
                return;
            new = malloc(sizeof(t_env));
            new->key =ft_strdup(str[0]);
            new->value= ft_strdup(str[1]);
            new->full = ft_strdup(args[1]);
            new->next = NULL;
            delete_if_exist(env, str[0]);
            ft_lstadd_back_ex(env, new);
            while(str[i])
            {
                free(str[i]);
                i++;
            }
            free(str);
        }
        i++;
    }
}
