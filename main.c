/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevisout <kevisout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:06:24 by kevisout          #+#    #+#             */
/*   Updated: 2025/08/19 14:12:06 by kevisout         ###   ########.fr       */
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

void	print_status(char *str, t_philo *philo)
{
	long int		time;

	time = get_time_in_ms() - philo->sarg->time_start;
	if (!check_death(philo, 0))
	{
		pthread_mutex_lock(&philo->sarg->mtx_print_status);
		printf("%ld %d %s", time, philo->id, str);
		pthread_mutex_unlock(&philo->sarg->mtx_print_status);
	}
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

void	dying(t_philo *philo)
{
	pthread_mutex_unlock(&philo->sarg->mtx_time_eat);
	pthread_mutex_unlock(&philo->sarg->mtx_finish);
	pthread_mutex_lock(&philo->sarg->mtx_print);
	print_status("died\n", philo);
	pthread_mutex_unlock(&philo->sarg->mtx_print);
	check_death(philo, 1);
	pthread_mutex_lock(&philo->sarg->mtx_time_eat);
	pthread_mutex_lock(&philo->sarg->mtx_finish);
}

void	thinking(t_philo *philo)
{
	pthread_mutex_lock(&philo->sarg->mtx_print);
	if (!check_stop_flag(philo))
		print_status("is thinking\n", philo);
	pthread_mutex_unlock(&philo->sarg->mtx_print);
}

void	sleeping(t_philo *philo)
{
	pthread_mutex_lock(&philo->sarg->mtx_print);
	if (!check_stop_flag(philo))
		print_status("is sleeping\n", philo);
	pthread_mutex_unlock(&philo->sarg->mtx_print);
	ft_usleep(philo->sarg->time2sleep);
}

void	eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->left_fork);
	pthread_mutex_lock(&philo->sarg->mtx_print);
	if (!check_stop_flag(philo))
		print_status("has taken a fork\n", philo);
	pthread_mutex_unlock(&philo->sarg->mtx_print);
	if (!philo->right_fork)
	{
		ft_usleep(philo->sarg->time2die * 2);
		if (philo->sarg->nb_philo == 1)
			pthread_mutex_unlock(&philo->left_fork);
		return ;
	}
	pthread_mutex_lock(philo->right_fork);
	pthread_mutex_lock(&philo->sarg->mtx_print);
	print_status("has taken a fork\n", philo);
	pthread_mutex_unlock(&philo->sarg->mtx_print);
	pthread_mutex_lock(&philo->sarg->mtx_print);
	print_status("is eating\n", philo);
	pthread_mutex_lock(&philo->sarg->mtx_time_eat);
	philo->last_eat = get_time_in_ms();
	pthread_mutex_unlock(&philo->sarg->mtx_time_eat);
	pthread_mutex_unlock(&philo->sarg->mtx_print);
	ft_usleep(philo->sarg->time2eat);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(&philo->left_fork);
}

void	philosopher_cycle(t_philo *philo)
{
	eating(philo);
	sleeping(philo);
	thinking(philo);
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
