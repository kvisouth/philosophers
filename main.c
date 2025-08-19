/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevisout <kevisout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:06:24 by kevisout          #+#    #+#             */
/*   Updated: 2025/08/19 14:35:46 by kevisout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long int	get_time_in_ms(void)
{
	long int			time;
	struct timeval		current_time;

	time = 0;
	if (gettimeofday(&current_time, NULL) == -1)
		return (0);
	time = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (time);
}

void	free_structure(t_struct *st)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&st->arg.mtx_flag);
	while (st->arg.flag == 0)
	{
		pthread_mutex_unlock(&st->arg.mtx_flag);
		ft_usleep(1);
		pthread_mutex_lock(&st->arg.mtx_flag);
	}
	pthread_mutex_unlock(&st->arg.mtx_flag);
	while (i < st->arg.nb_philo)
	{
		pthread_join(st->philo[i].thread_id, NULL);
		i++;
	}
	pthread_mutex_destroy(&st->arg.mtx_print);
	i = 0;
	while (i < st->arg.nb_philo)
	{
		pthread_mutex_destroy(&st->philo[i].left_fork);
		i++;
	}
	free(st->philo);
}

int	main(int ac, char **av)
{
	t_struct		st;

	if (!parsing(ac, av))
		return (0);
	init_arg(ac, av, &st);
	st.philo = malloc(sizeof(t_philo) * st.arg.nb_philo);
	if (!st.philo)
		return (0);
	init_mutex(&st);
	init_philo(&st);
	if (!start_simulation_threads(&st))
		return (free(st.philo), 0);
	return (free_structure(&st), 0);
}
