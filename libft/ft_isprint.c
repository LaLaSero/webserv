/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 15:22:10 by yutakagi          #+#    #+#             */
/*   Updated: 2023/10/12 14:08:16 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	if (32 <= c && c <= 126)
		return (1);
	return (0);
}

// #include <ctype.h>
// #include<stdio.h>
// int main()
// {
// 	printf("%d\n",isprint('a'));
// 	printf("%d\n",isprint('\t'));
// 	printf("%d\n",ft_isprint('a'));
// 	printf("%d\n",ft_isprint('\t'));
// }