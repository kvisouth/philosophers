/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevisout <kevisout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:06:24 by kevisout          #+#    #+#             */
/*   Updated: 2025/07/24 20:24:01 by kevisout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Returns the current time in milliseconds since January 1, 1970 00:00 */
long int	time_now(void)
{
	long int			time;
	struct timeval		current_time;

	time = 0;
	if (gettimeofday(&current_time, NULL) == -1)
		return (0);
	time = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (time);
}

/* A more accurate usleep function (not affected by the system's speed) */
void	ft_usleep(long int time_in_ms)
{
	long int	start_time;

	start_time = 0;
	start_time = time_now();
	while ((time_now() - start_time) < time_in_ms)
		usleep(time_in_ms / 10);
}

/* Initializes given arguments into t_struct */
int	init_infos(int ac, char **av, t_struct *st)
{
	st->arg.nb_philo = ft_atoi(av[1]);
	st->arg.time2die = ft_atoi(av[2]);
	st->arg.time2eat = ft_atoi(av[3]);
	st->arg.time2sleep = ft_atoi(av[4]);
	if (ac == 6)
		st->arg.meal2eat = ft_atoi(av[5]);
	st->arg.time_start = time_now();
	st->arg.flag = 0;
	st->arg.satiated_philos = 0;
	return (1);
}

/* Initializes mutexes used in the program */
void	init_mutex(t_struct *st)
{
	pthread_mutex_init(&st->arg.mtx_print_status, NULL);
	pthread_mutex_init(&st->arg.mtx_print, NULL);
	pthread_mutex_init(&st->arg.mtx_flag, NULL);
	pthread_mutex_init(&st->arg.mtx_time_eat, NULL);
	pthread_mutex_init(&st->arg.mtx_finish, NULL);
}

/* Initializes each philosopher needed for the simulation */
int	init_philo(t_struct *st)
{
	int	i;

	i = 0;
	st->philo = malloc(sizeof(t_philo) * st->arg.nb_philo);
	if (!st->philo)
		return (0);
	init_mutex(st);
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

void	free_struct(t_struct *st)
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

/* Updates the flag that indicates if a philo has died. */
int	check_or_set_death_flag(t_philo *philo, int i)
{
	int	flag_value;

	pthread_mutex_lock(&philo->info->mtx_flag);
	if (i != 0)
		philo->info->flag = i;
	flag_value = philo->info->flag;
	pthread_mutex_unlock(&philo->info->mtx_flag);
	if (flag_value != 0)
		return (1);
	return (0);
}

int	check_death_flag(t_philo *philo)
{
	int	flag_value;

	pthread_mutex_lock(&philo->info->mtx_flag);
	flag_value = philo->info->flag;
	pthread_mutex_unlock(&philo->info->mtx_flag);
	return (flag_value != 0);
}

void	print_status(char *str, t_philo *philo)
{
	long int		time;

	time = time_now() - philo->info->time_start;
	if (!check_or_set_death_flag(philo, 0))
	{
		pthread_mutex_lock(&philo->info->mtx_print_status);
		printf("%ld %d %s", time, philo->id, str);
		pthread_mutex_unlock(&philo->info->mtx_print_status);
	}
}

/* Prints the death and updates the flag to indicate a philo has died */
void	dying(t_philo *philo)
{
	pthread_mutex_unlock(&philo->info->mtx_time_eat);
	pthread_mutex_unlock(&philo->info->mtx_finish);
	pthread_mutex_lock(&philo->info->mtx_print);
	print_status("died\n", philo);
	pthread_mutex_unlock(&philo->info->mtx_print);
	check_or_set_death_flag(philo, 1);
	pthread_mutex_lock(&philo->info->mtx_time_eat);
	pthread_mutex_lock(&philo->info->mtx_finish);
}

void	thinking(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->mtx_print);
	if (!check_death_flag(philo))
		print_status("is thinking\n", philo);
	pthread_mutex_unlock(&philo->info->mtx_print);
}

void	sleeping(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->mtx_print);
	if (!check_death_flag(philo))
		print_status("is sleeping\n", philo);
	pthread_mutex_unlock(&philo->info->mtx_print);
	ft_usleep(philo->info->time2sleep);
}

void	handle_single_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->left_fork);
	pthread_mutex_lock(&philo->info->mtx_print);
	print_status("has taken a fork\n", philo);
	pthread_mutex_unlock(&philo->info->mtx_print);
	ft_usleep(philo->info->time2die + 10);
	pthread_mutex_unlock(&philo->left_fork);
}

void	take_forks_and_eat(t_philo *philo, pthread_mutex_t *first_fork,
		pthread_mutex_t *second_fork)
{
	pthread_mutex_lock(second_fork);
	pthread_mutex_lock(&philo->info->mtx_print);
	print_status("has taken a fork\n", philo);
	pthread_mutex_unlock(&philo->info->mtx_print);
	pthread_mutex_lock(&philo->info->mtx_print);
	print_status("is eating\n", philo);
	pthread_mutex_lock(&philo->info->mtx_time_eat);
	philo->last_eat = time_now();
	pthread_mutex_unlock(&philo->info->mtx_time_eat);
	pthread_mutex_unlock(&philo->info->mtx_print);
	ft_usleep(philo->info->time2eat);
	pthread_mutex_unlock(second_fork);
	pthread_mutex_unlock(first_fork);
}

void	determine_fork_order(t_philo *philo, pthread_mutex_t **first,
		pthread_mutex_t **second)
{
	if (&philo->left_fork <= philo->right_fork)
	{
		*first = &philo->left_fork;
		*second = philo->right_fork;
	}
	else
	{
		*first = philo->right_fork;
		*second = &philo->left_fork;
	}
}

void	eating(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philo->info->nb_philo == 1)
	{
		handle_single_philo(philo);
		return ;
	}
	determine_fork_order(philo, &first_fork, &second_fork);
	pthread_mutex_lock(first_fork);
	pthread_mutex_lock(&philo->info->mtx_print);
	if (!check_death_flag(philo))
		print_status("has taken a fork\n", philo);
	pthread_mutex_unlock(&philo->info->mtx_print);
	if (!philo->right_fork)
	{
		ft_usleep(philo->info->time2die * 2);
		if (philo->info->nb_philo == 1)
			pthread_mutex_unlock(first_fork);
		return ;
	}
	take_forks_and_eat(philo, first_fork, second_fork);
}

void	routine(t_philo *philo)
{
	eating(philo);
	sleeping(philo);
	thinking(philo);
}

void	monitor_flagger(t_struct *st, t_philo *philo)
{
	pthread_mutex_unlock(&philo->info->mtx_flag);
	if (st->arg.nb_philo == 1)
	{
		if ((time_now() - philo->last_eat) >= (long)(st->arg.time2die))
		{
			pthread_mutex_lock(&st->arg.mtx_print);
			print_status("died\n", philo);
			pthread_mutex_unlock(&st->arg.mtx_print);
			check_or_set_death_flag(philo, 1);
		}
		return ;
	}
	pthread_mutex_lock(&st->arg.mtx_time_eat);
	pthread_mutex_lock(&st->arg.mtx_finish);
	if (!check_or_set_death_flag(philo, 0) && !philo->finish
		&& (time_now() - philo->last_eat)
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
			pthread_mutex_lock(&philo->info->mtx_flag);
			if (philo->info->flag == 0)
				monitor_flagger(st, philo);
			else
			{
				pthread_mutex_unlock(&philo->info->mtx_flag);
				break ;
			}
		}
		i++;
	}
	return (0);
}

void	*philo_thread(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->id % 2 == 0)
		ft_usleep(philo->info->time2eat / 10);
	while (!check_or_set_death_flag(philo, 0))
	{
		routine(philo);
		if (++philo->meals_eaten == philo->info->meal2eat)
		{
			pthread_mutex_lock(&philo->info->mtx_finish);
			philo->finish = 1;
			philo->info->satiated_philos++;
			if (philo->info->satiated_philos == philo->info->nb_philo)
			{
				pthread_mutex_unlock(&philo->info->mtx_finish);
				check_or_set_death_flag(philo, 2);
				pthread_mutex_lock(&philo->info->mtx_finish);
			}
			pthread_mutex_unlock(&philo->info->mtx_finish);
			return (0);
		}
	}
	return (0);
}

/* Create a thread for each philo, where each one of then will run philo_thread
   Also create a monitoring thread to check if any philo has died on the run */
int	create_threads(t_struct *st)
{
	int			i;
	pthread_t	monithread;

	i = 0;
	while (i < st->arg.nb_philo)
	{
		st->philo[i].info = &st->arg;
		if (pthread_create(&st->philo[i].thread_id, NULL,
				philo_thread, &st->philo[i]) != 0)
			return (0);
		i++;
	}
	pthread_create(&monithread, NULL, monitoring, &st->philo);
	pthread_join(monithread, NULL);
	return (1);
}

int	main(int ac, char **av)
{
	t_struct	st;

	if (!parsing(ac, av))
	{
		printf("Error: parsing error\n");
		return (1);
	}
	if (!init_infos(ac, av, &st))
		return (0);
	if (!init_philo(&st))
	{
		free(st.philo);
		return (0);
	}
	if (!create_threads(&st))
	{
		free(st.philo);
		return (0);
	}
	free_struct(&st);
	return (0);
}
