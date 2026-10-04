/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leonpouet <leonpouet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 11:16:07 by leonpouet         #+#    #+#             */
/*   Updated: 2026/10/04 12:23:49 by leonpouet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

int	init_philo(t_table *table)
{
	int	i;

	i = 0;
	table->philos = malloc (sizeof(t_philo) * table->philo_nbr);
	if (!table->philos)
	{
		pthread_mutex_destroy(&table->print_mutex);
		pthread_mutex_destroy(&table->sim_mutex);
		return (0);
	}
	while (i < table->philo_nbr)
	{
		table->philos[i].id = i + 1;
		table->philos[i].full = false;
		table->philos[i].nbr_meal = 0;
		table->philos[i].table = table;
		if (pthread_mutex_init(&table->philos[i].philo_mutex, NULL))
			return (free_all(table, i, 0));
		i++;
	}
	return (1);
}

int	init_forks(t_table *table)
{
	int	i;

	i = 0;
	table->forks = malloc (sizeof(t_fork) * table->philo_nbr);
	if (!table->forks)
		return (free_all(table, table->philo_nbr, 0));
	while (i < table->philo_nbr)
	{
		if (pthread_mutex_init(&table->forks[i].fork, NULL))
			return (free_all(table, table->philo_nbr, i));
		table->forks[i].fork_id = i;
		i++;
	}
	i = 0;
	while (i < table->philo_nbr)
	{
		table->philos[i].right_fork = &table->forks[i];
		if (table->philo_nbr == 1)
			table->philos[i].left_fork = NULL;
		else
			table->philos[i].left_fork
				= &table->forks[(i + 1) % table->philo_nbr];
		i++;
	}
	return (1);
}

int	init_mutex(t_table *table)
{
	if (pthread_mutex_init(&table->print_mutex, NULL))
		return (0);
	if (pthread_mutex_init(&table->sim_mutex, NULL))
	{
		pthread_mutex_destroy(&table->print_mutex);
		return (0);
	}
	return (1);
}

int	init_data(t_table *table)
{
	int	check;

	table->forks = NULL;
	table->philos = NULL;
	table->end_simulation = false;
	check = init_mutex(table);
	if (check == 0)
		return (0);
	check = init_philo(table);
	if (check == 0)
		return (0);
	check = init_forks(table);
	if (check == 0)
		return (0);
	return (1);
}
