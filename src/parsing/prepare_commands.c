/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:44:14 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/04/30 10:15:27 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int count_args(t_token *tokens)
{
	int count;
	t_token	*current;
	t_token	*prev;
	
	count = 0;
	current = tokens;
	prev = NULL;

	while (current)
	{
		if (current->type == WORD)
		{
			if (!prev || (prev->type != REDIR_IN && prev->type != REDIR_OUT 
				&& prev->type != APPEND && prev->type != HEREDOC))
					count++;		
		}
		prev = current;
		current = current->next;
	}
	return (count);
}

void	add_outfile(t_exec	*exec, char	*filename, int append)
{
	t_redir	*new;
	t_redir	*tmp;
	
	new = malloc(sizeof(t_redir));
	new->filename = ft_strdup(filename);
	new->append = append;
	new->next = NULL;

	if (!exec->outfiles)
		exec->outfiles = new;
	else
	{
		tmp = exec->outfiles;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	add_infile(t_exec  *exec, char *filename)
{
	t_redir *new;
	t_redir	*tmp;
	
	new = malloc(sizeof(t_redir));
	new->filename = ft_strdup(filename);
	new->next = NULL;
	
	if (!exec->infiles)
		exec->infiles = new;
	else	
	{
		tmp = exec->infiles;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

static t_exec *init_exec(int arg_count)
{
	t_exec *exec;
	
	exec = malloc(sizeof(t_exec));
	if (!exec)
		return (NULL);
	
	exec->args = malloc(sizeof(char *) * (arg_count + 1));
	exec->infiles = NULL;
	exec->outfiles = NULL;
	exec->append = 0;
	exec->heredoc = 0;
	exec->delimiter = NULL;
	exec->cmd = NULL;
	exec->next = NULL;

	return (exec);
}

static void	handle_redirections(t_exec *exec, t_token *current)
{
	int is_append;
	
	is_append = 0;
	if (current->type == REDIR_IN && current->next)
		add_infile(exec, current->next->value);
	else if ((current->type == REDIR_OUT || current->type == APPEND) && current->next)
	{
		if (current->type == APPEND)
			is_append = 1; 
		add_outfile(exec, current->next->value, is_append);
	}
	else if (current->type == HEREDOC && current->next)
	{
		exec->delimiter = ft_strdup(current->next->value);
		exec->heredoc = 1;
	}
}

static void	handle_word(t_exec *exec, t_token *current
	, t_token *prev, int *i)
{
	if (!prev || (prev->type != REDIR_IN && prev->type != REDIR_OUT && prev->type != APPEND 
		&& prev->type != HEREDOC))
	{	
		exec->args[*i] = ft_strdup(current->value);
		if (!exec->cmd)
			exec->cmd = exec->args[0];
		(*i)++;
	}
}

t_exec	*parse_command(t_cmd *cmd, int i)
{
	t_exec *exec;
	t_token	*current;
	t_token	*prev;
	int		arg_count;

	current = cmd->token;
	prev = NULL;
	arg_count = count_args(current);
	exec = init_exec(arg_count);
	if (!exec)
		return (NULL);
	while (current)
	{
		if(current->type == WORD)
			handle_word(exec, current, prev, &i);
		else
			handle_redirections(exec, current);
		prev = current;
		current = current->next;
	}
	exec->args[i] = NULL;
	return (exec);
}

void	detect_delimiter(t_token *tokens)
{
	t_token *current;
	
	current = tokens;
	while (current)
	{
		if (current->type == HEREDOC)
		{
			current->next->type = HEREDOC_DELIMITER;
			return ;
		}
		current = current->next;
	}
}

char *remove_quotes(char *str)
{
	int		i;
	int		j;
	char	*result;
	char	quote;

	if (!str)
		return (NULL);
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && quote == 0)
			quote = str[i++];
		else if (str[i] == quote)
		{
			quote = 0;
			i++;
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	
	return (result);
}
 

t_cmd	*prepare_commands(char *input, t_env *env)
{
	t_token	*tokens;
	t_token	*current;
	char	*expanded_value;
	char    *quote_processed;
	t_cmd	*cmds;

	if (is_pipe_at_start(input) || !check_two_pipes(input))
	{
		printf("minishell: syntax error near unexpected token '|'\n");
		return (NULL);
	}
	
	input = handle_pipe_end(input);
	if (!input)
		return (NULL);
		
	input = check_unclosed_quotes(input);
	if (!input)
		return (NULL);
	
	tokens = tokenizer(input);
	if (!tokens)
		return (NULL);
	
	if (check_redirection_err(tokens) == 1)
		return (NULL);
	
	detect_delimiter(tokens);
	current = tokens;
	while (current)
	{
		if (current->type == WORD && current->type != HEREDOC_DELIMITER )
		{
			expanded_value = expand_variables(current->value, env, 0, 0);
			if (expanded_value  && current->type != REDIR_OUT)
			{
				quote_processed = remove_quotes(expanded_value);
				free(expanded_value);
				if (quote_processed)
				{
					free(current->value);
					current->value = quote_processed;
				}
			}
		}
		current = current->next;
	}
	
	cmds = split_by_pipe(tokens);
	remove_pipe_node(cmds);
	
	free_token(tokens);
	return(cmds);
}

t_exec *build_exec_list(char *input, t_env *env)
{
	t_cmd	*cmds;
	t_cmd	*tmp;
	t_exec	*exec_list;
	t_exec	*current;
	t_exec	*new_node;

	exec_list = NULL;
	current = NULL;
	new_node = 	NULL;
	cmds = prepare_commands(input, env);
	if (!cmds)
		return (NULL);

	tmp = cmds;
	while (tmp)
	{
		new_node = parse_command(tmp, 0);
		if (!new_node)
		{
			free_cmd_list(cmds);
			free_exec_list(exec_list);
			return (NULL);
		}
		if (!exec_list)
			exec_list = new_node;
		else
			current->next = new_node;
		
		current = new_node;
		tmp = tmp->next;
	}
	free_cmd_list(cmds);
	return (exec_list);
}
