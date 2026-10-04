/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leonpouet <leonpouet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 15:41:45 by leonpouet         #+#    #+#             */
/*   Updated: 2026/10/04 12:25:27 by leonpouet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>
# include <stdbool.h>

typedef pthread_mutex_t	t_mtx;

typedef struct s_table	t_table;

typedef struct s_fork
{
	t_mtx	fork;
	int		fork_id;
}	t_fork;

typedef struct s_philo
{
	int			id;
	int			nbr_meal;
	long		last_meal;
	bool		full;
	t_mtx		philo_mutex;
	t_fork		*left_fork;
	t_fork		*right_fork;
	pthread_t	thread_id;
	t_table		*table;
}	t_philo;

typedef struct s_table
{
	long		philo_nbr;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	long		max_meals;
	long		start_simulation;
	bool		end_simulation;
	t_mtx		print_mutex;
	t_mtx		sim_mutex;
	t_fork		*forks;
	t_philo		*philos;
	pthread_t	monitor;
}	t_table;

// UTILS

long	ft_atol(char *str);
int		is_digit(char *s);
int		free_all(t_table *table, int n_philo, int n_fork);

// TIME

long	get_time_ms(void);
void	ft_usleep(long ms);

// MUTEX

bool	mtx_sim(t_table *table);
void	mtx_printf(t_philo *philo, char *msg);
bool	mtx_full(t_philo *philo);

// PARSING

int		parsing(t_table *table, char **av);

// INIT

int		init_data(t_table *table);

// DINER

void	dinner_start(t_table *table);

// ACTION

void	think(t_philo *philo);
void	go_sleep(t_philo *philo);
void	eat(t_philo *philo);

#endif
