/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_start.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldubau <ldubau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 11:50:02 by ldubau            #+#    #+#             */
/*   Updated: 2026/09/29 14:14:33 by ldubau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	monitor(void *arg)
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
		ft_usleep(10);
		pthread_mutex_lock(&table->philos[i].philo_mutex);
		last_meal = table->philos[i].last_meal;
		full = table->philos[i].full;
		pthread_mutex_unlock(&table->philos[i].philo_mutex);
		if (!full)
		{
			if (get_time_ms() - last_meal >= table->time_to_die)
				mtx_printf(&table->philos[i], "died");
		}
		else
			nbr_full += 1;
		if (nbr_full >= table->philo_nbr)
			table->end_simulation = true;
		i++;
		if (i >= table->philo_nbr)
			i = 0;
	}
}

void	routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (philo->full == false)
	{
		if (philo->id % 2 == 1 && philo->nbr_meal == 0)
			ft_usleep(philo->table->time_to_eat / 2);
		eat(philo);
		sleep(philo);
	}
}

void	dinner_start(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_nbr)
		pthread_create(&table->philos[i].thread_id, NULL, routine, &table->philos[i++]);
	i = 0;
	table->start_simulation = get_time_ms();
	while (i < table->philo_nbr)
		table->philos[i++].last_meal = table->start_simulation;

}
