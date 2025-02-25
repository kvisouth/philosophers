/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevso <kevso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:26:58 by kevso             #+#    #+#             */
/*   Updated: 2025/02/25 15:28:19 by kevso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

/* Checks if all the arguments are digits */
int	check_full_digits(int ac, char **av)
{
	int	i;
	int	j;

	i = 1;
	while (i < ac)
	{
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] < '0' || av[i][j] > '9')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

/* Checks if all the arguments exeeds a length of 10 (INT_MAX nb of digits)*/
int	check_lengths(int ac, char **av)
{
	int	i;
	int	j;

	i = 1;
	while (i < ac)
	{
		j = 0;
		while (av[i][j])
			j++;
		if (j > 10)
			return (0);
		i++;
	}
	return (1);
}

/* Checks if all the arguments exeeds INT_MAX */
int	check_overflows(int ac, char **av)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		if (ft_atol(av[i]) > INT_MAX || ft_atol(av[i]) < INT_MIN)
			return (0);
		i++;
	}
	return (1);
}

int	parsing(int ac, char **av)
{
	if (ac < 5 || ac > 6)
		return (0);
	if (!check_full_digits(ac, av))
		return (0);
	if (!check_lengths(ac, av))
		return (0);
	if (!check_overflows(ac, av))
		return (0);
	if (ft_atoi(av[1]) < 1)
		return (0);
	return (1);
}
