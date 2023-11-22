/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anayef <anayef@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 12:13:03 by m_kamal           #+#    #+#             */
/*   Updated: 2023/11/22 20:17:23 by anayef           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/*	COMMAND_EXPANSION
**	-----------------
**	DESCRIPTION
**	This function will alternate the t_token list nodes in place according to
**	specific expansion rules of shell's grammar. The expansion rules include:
**	#1. Tilde expansion according to the HOME environment variable
**	#2. Variable expansion according to the environment and local variables
**	(prompted by '$')
**	#3. Quotation expansion both happening upon start and closing marks.
**	PARAMETERS
**	#1. cmd struct
**	RETURN VALUES
**	
*/

static void	token_expansion(t_token **token)
{
	int	pos;

	pos = 0;
	if ((*token)->tkn[pos] == '~')
		tilde_expansion(token, &pos);
	while ((*token)->tkn[pos])
	{
		if ((*token)->tkn[pos] == '$')
			variable_expansion(token, &pos);
		else if ((*token)->tkn[pos] == '\'' || (*token)->tkn[pos] == '\"')
			quote_expansion(token, &pos, (*token)->tkn[pos]);
		pos++;
	}
}

static void	expand_redirects(t_cmd *cmd)
{
	t_token	*token;

	token = cmd->redirects;
	while (token)
	{
		if (ft_strncmp(token->tkn, "<<", 3))
		{
			token = token->next;
			token_expansion(&token);
		}
		else
			token = token->next;
		token = token->next;
	}
}

void	cmd_expansion(t_cmd *cmd)
{
	t_token	*token;

	while (cmd)
	{
		token = cmd->commands;
		while (token)
		{
			token_expansion(&token);
			token = token->next;
		}
		expand_redirects(cmd);
		cmd = cmd->next;
	}
}
