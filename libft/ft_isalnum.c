/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 15:26:38 by yutakagi          #+#    #+#             */
/*   Updated: 2023/10/12 14:07:58 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	if (('0' <= c && c <= '9') || ('a' <= c && c <= 'z')
		|| ('A' <= c && c <= 'Z'))
		return (1);
	return (0);
}

// #include <ctype.h>
// #include<stdio.h>
// int main()
// {
// printf("%d\n",isalnum('a'));
// printf("%d\n",isalnum('3'));
// printf("%d\n",isalnum('\n'));
// printf("%d\n",ft_isalnum('a'));
// printf("%d\n",ft_isalnum('3'));
// printf("%d\n",ft_isalnum('\n'));
// }