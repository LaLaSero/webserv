/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 23:41:04 by yutakagi          #+#    #+#             */
/*   Updated: 2023/10/16 18:36:06 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(const char *s, char c)
{
	size_t	i;
	int		count;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		if (i == 0 && s[i] != c)
			count++;
		else if (s[i] != c && s[i - 1] == c)
			count++;
		i++;
	}
	return (count);
}

static void	*clear_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i] != NULL)
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

static size_t	count_len(const char *s, char c)
{
	int	i;

	i = 0;
	while (s[i] != c && s[i] != '\0')
		i++;
	return (i);
}

char	**ft_split(const char *s, char c)
{
	char	**tab;
	size_t	i;
	size_t	start;

	if (s == NULL)
		return (NULL);
	tab = (char **)malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (tab == NULL)
		return (NULL);
	i = 0;
	start = 0;
	while (i < count_words(s, c))
	{
		while (s[start] == c)
			start++;
		tab[i] = ft_substr(&s[start], 0, count_len(&s[start], c));
		if (tab[i] == NULL)
			return (clear_tab(tab));
		tab[i][count_len(&s[start], c)] = '\0';
		start += count_len(&s[start], c);
		i++;
	}
	tab[count_words(s, c)] = NULL;
	return (tab);
}

// int main()
// {
// 	char s[] = "QsplitQthisQforQmeQcanQyouQdoQitQ?";
// 	char c = 'Q';
// 	char **tab = ft_split(s, c);
// 	int i = 0;
// 	while (tab[i] != NULL)
// 	{
// 		printf("%s|\n", tab[i]);
// 		free(tab[i]);
// 		i++;
// 	}
// 	free(tab);
// }