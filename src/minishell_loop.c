#include "../includes/executor.h"
#include "../includes/minishell.h"
#include "../includes/parser.h"
#include "../includes/tokenizer.h"

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

static t_ast	*handle_input(t_env *env_list)
{
	t_token	*tokens;
	//t_token	*curr;
	t_ast	*ast;
	char	*input;

	input = readline("minishell$ ");
	if (!input)
		return NULL;
	if (*input == '\0')
	{
		free(input);
		return (t_ast *)-1;
	}
	add_history(input);
	tokens = tokenize(input, env_list);
	if (!tokens)
		return (free(input), NULL);
	if (is_syntax_error(tokens))
		return (free_tokens(tokens), free(input), NULL);
	/* curr = tokens;
	while (curr)
	{
		printf("Token: type=%d, value='%s'\n", curr->type, curr->value);
		curr = curr->next;
	} */
	ast = parse_tokens(tokens);
	free_tokens(tokens);
	free(input);
	return (ast);
}

void	minishell_loop(t_env *env_list, int *status)
{
	t_ast	*ast;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_signal);
	while (1)
	{
		ast = handle_input(env_list);
		if (!ast)
			break ;
		if (ast == (t_ast *)-1)
			continue;
		//print_ast(ast, 2);
		*status = execute_ast(ast, &env_list);
		free_ast(ast);
		if (*status >= 128)  // Exit was called
        {
            *status -= 128;  // Get actual exit code
            break;
        }
	}
}
