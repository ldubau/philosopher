/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_start.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldubau <ldubau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 11:50:02 by ldubau            #+#    #+#             */
/*   Updated: 2026/09/29 16:00:01 by ldubau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor(void *arg)
{
	t_table	*table;
	long	last_meal;
	bool	full;
	long	i;
	long	nbr_full;

	table = (t_table *)arg;
	i = 0;
	nbr_full = 0;
	while(1)
	{
		pthread_mutex_lock(&table->philos[i].philo_mutex);
		last_meal = table->philos[i].last_meal;
		full = table->philos[i].full;
		pthread_mutex_unlock(&table->philos[i].philo_mutex);
		if (!full)
			if (get_time_ms() - last_meal >= table->time_to_die)
			{
				long time;
				pthread_mutex_lock(&table->print_mutex);
				pthread_mutex_lock(&table->sim_mutex);
				table->end_simulation = true;
				pthread_mutex_lock(&table->sim_mutex);
				time = get_time_ms() - table->start_simulation;
				printf("%ld %d %s\n", time, table->philos->id, "died");
				pthread_mutex_lock(&table->print_mutex);
				break;
			}
		else
			nbr_full += 1;
		if (nbr_full >= table->philo_nbr)
		{
			pthread_mutex_lock(&table->sim_mutex);
			table->end_simulation = true;
			pthread_mutex_unlock(&table->sim_mutex);
		}
		i++;
		if (i >= table->philo_nbr)
		{
			i = 0;
			usleep(200);
		}
		nbr_full == 0;
	}
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 1 && philo->nbr_meal == 0)
		ft_usleep(philo->table->time_to_eat / 2);
	while (!mtx_full(philo) == false && !mtx_sim(philo->table))
	{
		if (philo->table->philo_nbr > 1)
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
	pthread_create(&table->monitor, NULL, monitor, &table->monitor);
	while (i < table->philo_nbr)
	{
		pthread_create(&table->philos[i].thread_id, NULL, routine, &table->philos[i]);
		i++;
	}

}
