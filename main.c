/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevso <kevso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:50:47 by kevso             #+#    #+#             */
/*   Updated: 2025/04/21 14:37:11 by kevso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	if (!parsing(ac, av))
	{
		printf("Error: parsing error\n");
		return (1);
	}
	printf("parsing successful\n");
	return (0);
}
