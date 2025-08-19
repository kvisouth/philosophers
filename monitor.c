/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevisout <kevisout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:25:09 by kevisout          #+#    #+#             */
/*   Updated: 2025/08/19 14:25:49 by kevisout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(t_philo *philo, int i)
{
	int	flag_value;

	pthread_mutex_lock(&philo->sarg->mtx_flag);
	if (i != 0)
		philo->sarg->flag = i;
	flag_value = philo->sarg->flag;
	pthread_mutex_unlock(&philo->sarg->mtx_flag);
	if (flag_value != 0)
		return (1);
	return (0);
}

int	check_stop_flag(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->sarg->nb_philo)
	{
		pthread_mutex_lock(&philo->sarg->mtx_flag);
		if (philo->sarg->flag != 0)
		{
			pthread_mutex_unlock(&philo->sarg->mtx_flag);
			return (1);
		}
		pthread_mutex_unlock(&philo->sarg->mtx_flag);
		i++;
	}
	return (0);
}

void	check_philo_starvation(t_struct *st, t_philo *philo)
{
	pthread_mutex_unlock(&philo->sarg->mtx_flag);
	pthread_mutex_lock(&st->arg.mtx_time_eat);
	pthread_mutex_lock(&st->arg.mtx_finish);
	if (!check_death(philo, 0) && !philo->finish
		&& (get_time_in_ms() - philo->last_eat)
		>= (long)(st->arg.time2die))
	{
		dying(philo);
	}
	pthread_mutex_unlock(&st->arg.mtx_time_eat);
	pthread_mutex_unlock(&st->arg.mtx_finish);
}

void	*monitoring(void *data)
{
	t_struct	*st;
	t_philo		*philo;
	int			i;

	st = (t_struct *)data;
	i = 0;
	while (i < st->arg.nb_philo)
	{
		philo = &st->philo[i];
		while (1)
		{
			ft_usleep(10);
			pthread_mutex_lock(&philo->sarg->mtx_flag);
			if (philo->sarg->flag == 0)
				check_philo_starvation (st, philo);
			else
			{
				pthread_mutex_unlock(&philo->sarg->mtx_flag);
				break ;
			}
		}
		i++;
	}
	return (0);
}
