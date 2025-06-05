void print_token_list(t_token *tokens)
{
    t_token *current = tokens;
    printf("=== Token List ===\n");
    while (current)
    {
        printf("Value: %-20s | Type: %d\n", current->value, current->type);
        current = current->next;
    }
    printf("==================\n");
}

void print_cmd_list(t_cmd *cmd_list)
{
    t_cmd *current = cmd_list;
    while (current)
    {
        t_token *token = current->token;
        while (token)
        {
            printf("%s ==> %d    ", token->value, token->type);
            token = token->next;
        }
        printf("\n");
        current = current->next;
    }
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