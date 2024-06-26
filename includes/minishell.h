/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jubernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 13:05:00 by jubernar          #+#    #+#             */
/*   Updated: 2023/06/22 13:05:02 by jubernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>
# include <termcap.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "libft.h"
# include "get_next_line.h"
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1

typedef struct s_llist
{
	char			*str;
	struct s_llist	*next;
}t_llist;

typedef struct s_redir
{
	int				type;
	char			*file;
	struct s_redir	*next;
}t_redir;

typedef struct s_minishell
{
	char	*input;
	char	***cmd_table;
	t_list	*env_mini;
	t_llist	*lexer_table;
	t_redir	*redir_start;
	t_redir	*redir_end;
	int		pipe;
	int		error_redir;
	int		error_pipe;
	int		nb_cmd;
	int		exit_status;
	int		main_pid;
	int		here_doc_pid;
	int		child_pid;
	int		error;
	int		error_malloc;
	int		here_doc_f;
	int		execute;
	char	**envp;
}t_minishell;

extern t_minishell	*g_mini;

//----- main.c
int		ft_failure(char *str, int exit_n, int fr_mini, int fr_env);
void	init_t_mini(t_minishell *mini);
int		main(int argc, char **argv, char **envp);

//----- arg.c
void	arg_check(int argc, char **argv);

//----- banner.c
void	banner_print_opening(void);
void	banner_print_exit(void);

//----- debug.c
int		print_lst_redir(t_redir *lst);
int		print_t_mini(t_minishell *mini);
int		print_lst(t_llist *lst);
int		print_cmd(char **cmd_line);
int		print_cmd_table(t_minishell *mini, int cmd);

//----- echo.c
int		echo_builtin(char **cmd_split);

//----- exit.c
int		exit_builtin(char **cmd, t_minishell *mini);

void	send_error(char *str, int quit, int exit_n);
int		prompt(t_minishell *mini);

//----- prompt.c
int		prompt(t_minishell *mini);

//----- lexer.c
int		lexer_iswordstart(char c);
int		tokfun(int *i, int *a, int (*fun)(char *, t_minishell *),
			t_minishell *mini);
int		lexer(t_minishell *mini, int i, int a, int b);

//----- parser_command.c
char	**parser_malloc_command(t_minishell *mini, t_llist *cmd_list);
t_llist	*parser_build_cmd(t_minishell *mini, int i, t_llist *lex, t_llist *cmd);
int		parser_empty_cmd(t_minishell *mini);

//----- parser_main.c
t_llist	*parser_var_split(t_minishell *mini, t_llist **lex, t_llist *split_cmd);
int		parser(t_minishell *mini);
int		parser_error_check(t_minishell *mini);

//----- utils.c
void	deallocate_list(t_llist **head);
void	free_mini(t_minishell *mini);
int		lst_size(t_llist *lst);
int		add_to_redir(t_redir **root, int type, char *str);

// ---- token.c
int		token_yield_redir(char *redir, t_minishell *mini);
int		token_yield_quote(char *token, t_minishell *mini);
int		token_yield_pipe(char *token, t_minishell *mini);
int		token_yield_word(char *token, t_minishell *mini);
int		token_yield_var(char *token, t_minishell *mini);

//----- find_path.c
char	*ft_access_path(char **cmd, int i, t_minishell *mini);
char	**ft_access_list(char **cmd, t_minishell *mini);
char	**ft_access_list_help(char *cmd_2, char **path_from_envp,
			int len, int i);

//----- find_executable.c
char	*find_executable(char **cmd, t_minishell *mini);
int		is_absolute_path(char *path);
int		is_relative_path(char *path);
char	*getenv_mini(char *env_mini, t_minishell *mini);
int		ft_count_trim(char *cmd);

//----- find_executable2.c
char	*ft_relative_path(char *cmd, t_minishell *mini,
			char *temp1, char *temp2);
char	*origine_path(int count_trim, t_minishell *mini, int j, int nb);

//----- builtin functions.c
int		pwd_builtin(t_minishell *mini, char **cmd);
int		env_mini(t_minishell *mini, char **envp);
int		env_builtin(t_minishell *mini);
int		echo_builtin(char **cmd_split);
int		unset_builtin(char **cmd, t_minishell *mini);
int		exit_builtin(char **cmd, t_minishell *mini);

//----- environment function
void	deallocate_env(t_list **root);
int		list_env_update(t_minishell *mini, char *var_update);

//----- singal.c
void	signal_main(void);
void	signal_main_handler(int s);
void	signal_command(t_minishell *mini);
void	signal_command_handler(int s);
void	signal_here(void);
void	signal_here_handler(int s);

//----- token.c
int		token_yield_word(char *token, t_minishell *mini);
int		token_yield_quote(char *token, t_minishell *mini);
int		token_yield_var(char *token, t_minishell *mini);
int		token_yield_pipe(char *token, t_minishell *mini);
int		token_yield_redir(char *redir, t_minishell *mini);

//----- free_functions.c
void	free_null(void *ptr);
void	free_mini(t_minishell *mini);
int		ft_free_fail(char *str1, char *str2, char **tab1, char **tab2);
int		ft_free_success(char *str1, char *str2, char **tab1, char **tab2);

//----- free_functions2.c
void	free_llist(t_llist **head);
void	free_redir(t_redir **head);
void	ft_free_tab(char **tab);
void	*ft_free_pointer(char *str1, char *str2, char **tab1, char **tab2);
void	free_tab_3d(char ***arr);

//----- execute_utils.c
int		max_length(const char *str1, const char *str2);
int		is_builtin(char *cmd);
int		is_env_function(char **cmd);
int		ft_tablen(char **cmd);

//----- execute_process.c
int		child_process(t_minishell *mini, int index, char **cmd, int **fd);
int		create_process_fd(char **cmd, t_minishell *mini, int index, int **fd);
int		**create_pipe(t_minishell *mini);
int		free_pipe(int **fd);
int		close_fd(int **fd);

//----- execute_main.c
int		execute_builtin(char **cmd_split, t_minishell *mini);
int		exec(char **cmd, char **envp, t_minishell *mini);
int		executor(t_minishell *mini);
int		exit_process(pid_t pid);

//----- execute_cmd.c
int		execute_one_cmd(t_minishell *mini);
int		exec_more_cmd(t_minishell *mini, int index, int exit_status, pid_t pid);

//----- cd_relativepath.c
int		ft_tablen(char **cmd);
char	*cd_relpath1(char *cmd, t_minishell *mini);
char	*cd_relpath2(char *cmd, t_minishell *mini);
char	*cd_relpath3(char *cmd, t_minishell *mini);
char	*ft_relative_path_cd(char *cmd, t_minishell *mini);

//----- cd_builtin.c
char	*convert_path_to_absolute(char **cmd, t_minishell *mini);
int		update_env_mini(t_minishell *mini, char *up_var, char *new_value);
int		update_env_cd(t_minishell *mini, char **cd);
int		cd_builtin(char **cmd, t_minishell *mini, char *cmd_replace);

//----- export_builtin.c
char	*adjust_help2(t_minishell *mini, char *start, char *end);
char	*adjust_help1(t_minishell *mini, char *var_env, char *start, char *end);
char	*adjust_var_env(t_minishell *mini, char *var_env, int count);
int		export_with_arg(t_minishell *mini, char **cmd, int i, char *new_var);
int		export_builtin(char **cmd, t_minishell *mini);

//----- export_utils.c
char	*ft_strrchr_set(const char *s, char *set);
int		check_arg_export(char *export_arg);
int		check_update_var(char *cmd, t_list *env_mini);
int		list_env_update(t_minishell *mini, char *var_update);
char	*translate_var(t_minishell *mini, char *str);

//----- parser_redir.c
int		parser_redir_check(t_minishell *mini, char *redir, int cmd_nb);
int		parser_redir_add_lst(t_minishell *mini, int type,
			char *file, int cmd_n);
t_llist	*parser_redir_file(t_minishell *mini, t_llist *lex, int cm_n, int type);

//----- redirection.c
void	here_doc(char *limiter, t_minishell *mini);
int		redirection_function(t_minishell *mini, t_redir *redirection);
int		input_redirection(t_minishell *mini, t_redir *start);
int		output_redirection(t_redir *end, int fd_outfile);
void	exit_redir(int fd_outfile);

//----- redirection_insert.c
void	here_doc_insert(t_minishell *mini, char *limiter);
int		redirection_function_insert(t_minishell *mini, t_redir *redirection);
int		infile_insert(t_minishell *mini, t_redir *start);
int		outfile_insert(t_redir *end, int fd_outfile);

//----- redirection_heredoc.c
char	*ft_reverse_split(char **line_split, char *c);
char	*ft_strlcpy_dollar(char *str, t_minishell *mini);
char	*heredoc_convert_dollar(t_minishell *mini, char *line);
void	here_doc_put_in(char *limiter, int *fds, t_minishell *mini);

//----- redirection_heredoc_insert.c
void	here_doc_put_in_insert(char *limiter);

//----- var.c
int		quote_translation(t_minishell *mini, t_llist *curr);
char	*var_find_translation(t_list *curr, char *var, char *translation);
char	*var_translation(t_minishell *mini, char *var);
char	*add_var_translation(t_minishell *mini, char *str);

//----- utils_add.c
int		add_to_redir(t_redir **root, int type, char *str);
int		add_to_list(t_llist **root, char *str);

#endif
