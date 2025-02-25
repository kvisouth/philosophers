/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevso <kevso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:50:47 by kevso             #+#    #+#             */
/*   Updated: 2025/02/25 15:28:01 by kevso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	main(int ac, char **av)
{
	if (!parsing(ac, av))
	{
		printf("parsing failed\n");
		return (1);
	}
	printf("parsing successful\n");
	return (0);
}
