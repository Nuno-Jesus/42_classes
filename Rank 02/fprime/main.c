/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 20:09:37 by marvin            #+#    #+#             */
/*   Updated: 2023/01/30 20:09:37 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

void fprime(int n)
{
	if (n == 1)
		printf("1");

	for (int i = 2; i <= n; i++)
	{
		while (n % i == 0)
		{
			printf("%d", i);
			n /= i;
			if (n == 1)
				return ;
			printf("*");
		}
	}
}

int main (int argc, char **argv)
{
	if (argc - 1 == 1)
		fprime(atoi(argv[1]));
	printf("\n");
}