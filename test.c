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

void	print_exec_list(t_exec *execs)
{
	int i = 0;
	
	if (!execs)
		return;
	t_exec	*current = execs;
	while (current)
	{
		printf("Command %d:\n", ++i);
		printf("	cmd	: %s\n", current->cmd);
		printf("	args 	: ");
		int j = 0;
		if (current->args[j])
		{
			while (current->args[j])
			{
				printf("%s ,", current->args[j]);
				j++;
			}
		}
		printf("\n");
		t_redir *redir = current->redirections;
		if (redir)
			printf("Redirections list : \n");
		while (redir)
		{
			printf("	Filename : %s -----\n", redir->filename);
			if (redir->type == REDIR_IN)
				printf("	Type : REDIR_IN -----\n");
			else if (redir->type == REDIR_OUT)
				printf("	Type : REDIR_OUT -----\n");
			else if (redir->type == APPEND)
				printf("	Type : APPEND -----\n");
			else if (redir->type == HEREDOC)
			{
				printf("	Type : HEREDOC -----\n");
				printf("	Delimiter : %s -----\n", redir->delimiter);
				printf("	Herdoc_fd : %d -----\n", redir->herdoc_fd);
				printf("	quote_flag : %d -----\n", redir->quoted_flag);
				
			}
			printf("	Herdoc Count : %d -----\n", redir->heredoc_count);
			printf("\n");
			redir = redir->next;
		}
		current = current->next;
	}
}