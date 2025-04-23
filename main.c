/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevisout <kevisout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:50:47 by kevso             #+#    #+#             */
/*   Updated: 2025/04/23 16:00:42 by kevisout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_args(int ac, char **av, t_philo *philo)
{
	philo->nb_philos = ft_atoi(av[1]);
	philo->time2die = ft_atoi(av[2]);
	philo->time2eat = ft_atoi(av[3]);
	philo->time2sleep = ft_atoi(av[4]);
	if (ac == 6)
		philo->optional = ft_atoi(av[5]);
	return (1);
}

int	main(int ac, char **av)
{
	t_philo	philo;
	if (!parsing(ac, av))
	{
		printf("Error: parsing error\n");
		return (1);
	}
	printf("parsing successful\n\n");
	if (!init_args(ac, av, &philo))
		return (1);
	printf("Numbers of philos : %d\n", philo.nb_philos);
	printf("Time to die : %d\n", philo.time2die);
	printf("Time to eat : %d\n", philo.time2eat);
	printf("Time to sleep : %d\n", philo.time2sleep);
	if (ac == 6)
		printf("Must eat : %d\n", philo.optional);
	return (0);
}
