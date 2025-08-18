/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevisout <kevisout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 14:02:54 by kevisout          #+#    #+#             */
/*   Updated: 2025/08/18 14:03:17 by kevisout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_arg(int argc, char **argv, t_struct *st)
{
	st->arg.nb_philo = ft_atoi(argv[1]);
	st->arg.time2die = ft_atoi(argv[2]);
	st->arg.time2eat = ft_atoi(argv[3]);
	st->arg.time2sleep = ft_atoi(argv[4]);
	st->arg.meals_to_eat = -1;
	if (argc == 6)
		st->arg.meals_to_eat = ft_atoi(argv[5]);
	st->arg.time_start = time_now();
	st->arg.flag = 0;
	st->arg.satiated_philos = 0;
}

void	init_mutex(t_struct *st)
{
	pthread_mutex_init(&st->arg.mtx_print_status, NULL);
	pthread_mutex_init(&st->arg.mtx_print, NULL);
	pthread_mutex_init(&st->arg.mtx_flag, NULL);
	pthread_mutex_init(&st->arg.mtx_time_eat, NULL);
	pthread_mutex_init(&st->arg.mtx_finish, NULL);
}

int	init_philo(t_struct *st)
{
	int	i;

	i = 0;
	while (i < st->arg.nb_philo)
	{
		st->philo[i].id = i + 1;
		st->philo[i].last_eat = st->arg.time_start;
		st->philo[i].meals_eaten = 0;
		st->philo[i].finish = 0;
		st->philo[i].right_fork = NULL;
		pthread_mutex_init(&st->philo[i].left_fork, NULL);
		if (st->arg.nb_philo == 1)
			return (1);
		if (i == st->arg.nb_philo - 1)
			st->philo[i].right_fork = &st->philo[0].left_fork;
		else
			st->philo[i].right_fork = &st->philo[i + 1].left_fork;
		i++;
	}
	return (1);
}
