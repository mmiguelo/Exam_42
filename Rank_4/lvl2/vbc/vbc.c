#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct	node {
	enum {
		ADD,
		MULTI,
		VAL} type;
		int	val;
		struct node	*l;
		struct node *r;
} node;

node    *new_node(node n)
{
    node *ret = calloc(1, sizeof(node));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void    destroy_tree(node *n)
{
    if (!n)
        return ;
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of input\n");
}

int accept(char **s, char c)
{
    if (**s == c)
    {
        (*s)++;
        return (1);
    }
    return (0);
}

int expect(char **s, char c)
{
    if (accept(s, c))
        return (1);
    unexpected(**s);
    return (0);
}

node	*add(char **s)
{
	node *left = multi(s);
	if (!left)
		return NULL;
	while(**s == '+')
	{
		(*s)++;
		node *right = multi(s);
		if (!right)
			return (destroy_tree(left), NULL);
		node n = {.type = ADD, .l = left, .r = right};
		left = (new_node(n));
	}
	return (left);
}

node	*multi(char **s)
{
	node *left = digit(s);
	if(!left)
		return NULL;
	while(**s == '*')
	{
		(*s)++;
		node *right = digit(s);
		if (!right)
			return (destroy_tree(left), NULL);
		node n = {.type = MULTI, .l = left, .r = right};
		left = (new_node(n));
	}
	return (left);
}

node *digit(char **s)
{
	if (**s == '(')
	{
		(*s)++;
		node *res = add(s);
		if (!res || !accept(s, ')'))
			return (destroy_tree(res), NULL);
		return (res);
	}
	else if(isdigit(**s))
	{
		node n = {.type = VAL, .val = (**s) - '0'};
		(*s)++;
		return(new_node(n));
	}
	return NULL;
}

node	*parse_expr(char *s)
{
	node *ret = add(&s);

	if (!ret || *s)
	{
		if (ret)
			destroy_tree(ret);
		if (*s)
			unexpected(*s);
		else
			unexpected(0);
		return (NULL);
	}
	return (ret);
}