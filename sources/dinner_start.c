/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_start.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leonpouet <leonpouet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 11:50:02 by ldubau            #+#    #+#             */
/*   Updated: 2026/10/04 12:24:09 by leonpouet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	announce_death(t_table *table, long i)
{
	long	time;

	pthread_mutex_lock(&table->print_mutex);
	pthread_mutex_lock(&table->sim_mutex);
	table->end_simulation = true;
	pthread_mutex_unlock(&table->sim_mutex);
	time = get_time_ms() - table->start_simulation;
	printf("%ld %d %s\n", time, table->philos[i].id, "died");
	pthread_mutex_unlock(&table->print_mutex);
}

bool	monitor_loop(t_table *table, long i, long *nbr_full)
{
	long	last_meal;
	bool	full;

	pthread_mutex_lock(&table->philos[i].philo_mutex);
	last_meal = table->philos[i].last_meal;
	full = table->philos[i].full;
	pthread_mutex_unlock(&table->philos[i].philo_mutex);
	if (!full && get_time_ms() - last_meal >= table->time_to_die)
	{
		announce_death(table, i);
		return (true);
	}
	if (full)
		*nbr_full += 1;
	if (*nbr_full >= table->philo_nbr)
	{
		pthread_mutex_lock(&table->sim_mutex);
		table->end_simulation = true;
		pthread_mutex_unlock(&table->sim_mutex);
		return (true);
	}
	return (false);
}

void	*monitor(void *arg)
{
	t_table	*table;
	long	i;
	long	nbr_full;

	table = (t_table *)arg;
	i = 0;
	nbr_full = 0;
	while (!monitor_loop(table, i, &nbr_full))
	{
		i++;
		if (i >= table->philo_nbr)
		{
			i = 0;
			nbr_full = 0;
			usleep(200);
		}
	}
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->table->philo_nbr == 1)
	{
		pthread_mutex_lock(&philo->right_fork->fork);
		mtx_printf(philo, "has taken a fork");
		while (!mtx_sim(philo->table))
			usleep(100);
		pthread_mutex_unlock(&philo->right_fork->fork);
		return (NULL);
	}
	if (philo->id % 2 == 1)
		ft_usleep(philo->table->time_to_eat / 2);
	while (!mtx_full(philo) && !mtx_sim(philo->table))
	{
		eat(philo);
		go_sleep(philo);
		think(philo);
	}
	return (NULL);
}

void	dinner_start(t_table *table)
{
	int	i;

	i = 0;
	table->start_simulation = get_time_ms();
	while (i < table->philo_nbr)
		table->philos[i++].last_meal = table->start_simulation;
	i = 0;
	pthread_create(&table->monitor, NULL, monitor, table);
	while (i < table->philo_nbr)
	{
		pthread_create(&table->philos[i].thread_id,
			NULL, routine, &table->philos[i]);
		i++;
	}
	i = 0;
	pthread_join(table->monitor, NULL);
	while (i < table->philo_nbr)
	{
		pthread_join(table->philos[i].thread_id, NULL);
		i++;
	}
}
