#include "../includes/executor.h"
#include "../includes/minishell.h"
#include "../includes/parser.h"
#include "../includes/tokenizer.h"


static void execute_pipechaining(t_ast *ast, t_env **env_list)
{
    (void)ast;
    (void)env_list;

    printf("...executing pipe\n");
}

static t_ast	*handle_input(t_env *env_list)
{
	t_token	*tokens;
	t_token	*curr;
	t_ast	*ast;
	char	*input;

	input = readline("minishell$ ");
	if (!input)
		exit(env_list->last_ex_status);
	else if (*input)
		add_history(input);
	tokens = tokenize(input, env_list);
	if (!tokens)
		return (printf("minishell: tokenize error\n"), free(input), NULL);
	if (is_syntax_error(tokens))
		return (free_tokens(tokens), free(input), NULL);
	curr = tokens;
	while (curr)
	{
		printf("Token: type=%d, value='%s'\n", curr->type, curr->value);
		curr = curr->next;
	}
	ast = parse_tokens(tokens);
	free_tokens(tokens);
	return (ast);
}
///inlude header somewhere
#include <signal.h>
//


void    handle_signal(int sig)
{

	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();

	}
}

void	minishell_loop(t_env *env_list)
{
	t_ast	*ast;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_signal);
	
	while (1)
	{
		ast = handle_input(env_list);
		if (!ast)
			continue ;
		if (ast)
		{
			print_ast(ast, 3);
            if (ast->type != NODE_PIPE && ast->type != NODE_REDIR)
                execute_ast(ast, &env_list);
            else
                execute_pipechaining(ast, &env_list);
			free_ast(ast);
		}
	}
}
