/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 20:05:47 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/07 10:03:22 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_token_type get_token_type(char *token)
{
	if (!ft_strcmp(token, "|"))
		return (PIPE);
	if (!ft_strcmp(token, "<"))
		return (REDIR_IN);
	if (!ft_strcmp(token, ">"))
		return (REDIR_OUT);
	if (!ft_strcmp(token, "<<"))
		return (HEREDOC);
	if (!ft_strcmp(token, ">>"))
		return (APPEND);
	return (WORD);
}

t_token *create_token(char *content)
{
	t_token *new_token;
	
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
		
	new_token->value = ft_strdup(content);
	new_token->type = get_token_type(content);
	new_token->next = NULL;
	
	return (new_token);
}


/* Helper function to check if we're inside quotes */
int is_in_quotes(char c, int *in_single, int *in_double)
{
    if (c == '\'' && !(*in_double))
        *in_single = !(*in_single);
    else if (c == '\"' && !(*in_single))
        *in_double = !(*in_double);
    
    return (*in_single || *in_double);
}

/* Get the length of the next token with proper quote handling */
int get_token_length(char *input, int start)
{
    int i = start;
    int in_single = 0;
    int in_double = 0;
    
    /* Handle special characters */
    if ((input[i] == '|' || input[i] == '<' || input[i] == '>') && 
        !in_single && !in_double)
    {
        /* Check for << or >> */
        if ((input[i] == '<' || input[i] == '>') && input[i + 1] == input[i])
            return 2;
        return 1;
    }
    
    /* Regular token - continue until whitespace or special char outside quotes */
    while (input[i])
    {
        is_in_quotes(input[i], &in_single, &in_double);
        
        /* Break on whitespace or special char if not in quotes */
        if (!in_single && !in_double && 
            (is_whitespace(input[i]) || input[i] == '|' || 
             input[i] == '<' || input[i] == '>'))
            break;
            
        i++;
    }
    
    return (i - start);
}

t_token *tokenizer(char *input)
{
    t_token *head = NULL;
    t_token *current = NULL;
    t_token *new_token;
    int i = 0;
    int token_len;
    char *token_content;
    
    while (input[i])
    {
        /* Skip whitespace */
        while (input[i] && is_whitespace(input[i]))
            i++;
            
        if (!input[i])
            break;
            
        /* Get token length with proper quote handling */
        token_len = get_token_length(input, i);
        
        /* Extract token content */
        token_content = malloc(token_len + 1);
        if (!token_content)
        {
            /* Clean up and return error */
            if (head)
                free_token(head);
            return (NULL);
        }
        
        ft_memcpy(token_content, &input[i], token_len);
        token_content[token_len] = '\0';
        
        /* Create token */
        new_token = create_token(token_content);
        free(token_content);
        if (!new_token)
        {
            if (head)
                free_token(head);
            return (NULL);
        }
        
        /* Add to token list */
        if (!head)
            head = new_token;
        else
            current->next = new_token;
        current = new_token;
        
        i += token_len;
    }
    
    return (head);
}
