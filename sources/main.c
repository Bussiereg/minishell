/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jubernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 13:05:00 by jubernar          #+#    #+#             */
/*   Updated: 2023/06/22 13:05:02 by jubernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_t_mini(t_minishell *mini)
{
	mini->input = NULL;
	mini->lexer_table = NULL;
	mini->cmd_table = NULL;
	mini->limiter = NULL;
	mini->in_file = NULL;
	mini->out_file = NULL;
	mini->pipe = 0;
	mini->input_redirection = 0;
	mini->output_redirection = 0;
	mini->nb_cmd = 0;
	mini->error_redir = 0;
	mini->error_pipe = 0;
	return (0);
}

int main(int argc, char **argv, char **envp)
{
	t_minishell mini;

	(void)argv;
	(void)argc;
	print_opening();
    while(1)
    {
		init_t_mini(&mini);
		prompt(envp, &mini);
		lexer(&mini);
 			//print_lst(mini.lexer_table); 
		parser(&mini);
 			print_t_mini(&mini);
			//print_cmd_table(&mini, mini.nb_cmd);
	/* 	if (mini.error_pipe != 0 || mini.error_redir != 0)
			{
				ft_printf("Parsing ERROR\n");
				//free_cmd_table(mini.cmd_table);
			}
		else
		executor(mini, envp); */
		//free_cmd_table(mini.cmd_table);
		//free_redir_files_names();
    }
    return(0);
} 
