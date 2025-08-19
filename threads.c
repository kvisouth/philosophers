/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevisout <kevisout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:24:36 by kevisout          #+#    #+#             */
/*   Updated: 2025/08/19 14:25:00 by kevisout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosopher_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->id % 2 == 0)
		ft_usleep(philo->sarg->time2eat / 10);
	while (!check_death(philo, 0))
	{
		philosopher_cycle(philo);
		if (++philo->meals_eaten == philo->sarg->meals_to_eat)
		{
			pthread_mutex_lock(&philo->sarg->mtx_finish);
			philo->finish = 1;
			philo->sarg->satiated_philos++;
			if (philo->sarg->satiated_philos == philo->sarg->nb_philo)
			{
				pthread_mutex_unlock(&philo->sarg->mtx_finish);
				check_death(philo, 2);
				pthread_mutex_lock(&philo->sarg->mtx_finish);
			}
			pthread_mutex_unlock(&philo->sarg->mtx_finish);
			return (0);
		}
	}
	return (0);
}

int	start_simulation_threads(t_struct *st)
{
	int			i;
	pthread_t	monithread;

	i = 0;
	while (i < st->arg.nb_philo)
	{
		st->philo[i].sarg = &st->arg;
		if (pthread_create(&st->philo[i].thread_id, NULL,
				philosopher_routine, &st->philo[i]) != 0)
			return (0);
		i++;
	}
	pthread_create(&monithread, NULL, monitoring, &st->philo);
	pthread_join(monithread, NULL);
	return (1);
}
