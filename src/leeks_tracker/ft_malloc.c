#include "../execution/execution.h"

#define ALLOCATE 1
#define FREE_ONE 2
#define FREE_ALL 0


typedef struct s_ft_malloc
{
    void *data;
    struct s_ft_malloc *next;
}t_ft_malloc;


static void	ft_bzero(void *ptr, size_t i)
{
	unsigned char	*ptr1;
	size_t			j;

	ptr1 = (unsigned char *)ptr;
	j = 0;
	if (i == 0)
		return ;
	while (j < i)
	{
		ptr1[j] = 0;
		j++;
	}
}

static void free_list_stats(t_ft_malloc **list , int EXIT)
{
    t_ft_malloc *tmp;

    while(*list)
    {
        tmp = (*list)->next;
        free((*list)->data);
        free((*list));
        *list = tmp;
    }
    exit(EXIT);
}

void free_one_node(t_ft_malloc **list, void *to_delete)
{
	if (list == NULL || *list == NULL)
		return;

	t_ft_malloc *cur = *list;

	if (cur->data == to_delete)
	{
		*list = cur->next;
		free(cur);
		free_one_node(list, to_delete);
	}
	cur = *list;
	free_one_node(&cur->next, to_delete);
}

static void alloc_add_back(t_ft_malloc **lst, void *value)
{
    t_ft_malloc *last;
    t_ft_malloc *tmp;

    if (!lst || !value)
        return;
    tmp = malloc(sizeof(t_ft_malloc));
    if (!tmp)
        free_list_stats(lst, -1);
    tmp->data = value;
    tmp->next = NULL;
    if (!*lst)
    {
        *lst = tmp;
        return;
    }
    last = *lst;
    while (last ->next)
        last = last->next;
    last->next = tmp;   
}

void ft_malloc(size_t size, int key, int exit_status, void *to_delet)
{
    static t_ft_malloc *tracker;
    void                *ptr;
    if (key == ALLOCATE)
    {
        ptr = malloc(size);
        if(!ptr)
            free_list_stats(&tracker, -1);
        alloc_add_back(&tracker, ptr);
        ft_bzero(ptr, size);
    }
    else if (key == FREE_ALL)
        free_list_stats(&tracker, exit_status);
    else if (key == FREE_ONE)
        free_one_node(&tracker, to_delet);
    return(ptr);
}