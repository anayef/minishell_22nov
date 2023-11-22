/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anayef <anayef@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 08:02:48 by m_kamal           #+#    #+#             */
/*   Updated: 2023/11/22 21:34:06 by anayef           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*	TOKENIZE_INPUT
**	---------
**	DESCRIPTION
**	The tokenizer function will split the original input line into "tokens"
**	(meaninfull words), the first step to properly "understand" what the user
**	is trying to accomplish with the command. 
**	PARAMETERS
**	#1. Parser struct.
**	RETURN VALUES
**	-
*/

t_token	*create_token(char *tkn, int flag)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		ft_error(NULL, EXIT, 12);
	token->tkn = tkn;
	if (!flag)
		token->lexema = lexical_analysis(tkn);
	else
		token->lexema = WORD;
	token->next = NULL;
	return (token);
}

static void	add_part(t_parser *parser, const char *input, size_t size)
{
	t_token	*token;
	char	*tkn;

	while (*input == ' ')
	{
		size--;
		input++;
	}
	if (!size || !(*input))
		return ;
	tkn = ft_strndup(input, size);
	token = create_token(tkn, 0);
	token_add_back(&parser->tokens, token);
}

void	tokenize_input(t_parser *parser)
{
	int		i;
	int		prev;
	int		next;
	int		size;

	i = 0;
	prev = i;
	next = i;
	size = 0;
	while (TRUE)
	{
		if (is_new_token(parser->input, i, prev))
			next = i;
		size = next - prev;
		if (size > 0)
			add_part(parser, &parser->input[prev], size);
		if (!parser->input[i])
			break ;
		prev = next;
		i++;
	}
}
