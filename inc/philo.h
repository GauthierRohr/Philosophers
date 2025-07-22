/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:59:29 by grohr             #+#    #+#             */
/*   Updated: 2025/07/22 16:44:42 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_data t_data;

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	long			last_meal;
	int				meals_eaten;
	t_data			*data;
}					t_philo;

typedef struct s_data
{
	int				num_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				max_meals;
	//rajout
	int				tot_finish_meal;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	death_mutex;
	int				simulation_stop;
	long			start_time;
}					t_data;

// 1_init.c
int		init_mutexes(t_data *data);
int		init_philos(t_data *data);
int		init_parse_args(int argc, char **argv, t_data *data);

// 2_threads.c
void	join_threads(t_data *data, pthread_t supervisor);
int		create_threads(t_data *data, pthread_t *supervisor);

// routine.c
void	*philo_routine(void *arg);
void	take_forks(t_philo *philo);
void	eat(t_philo *philo);

// supervisor.c
void	*supervisor_routine(void *arg);
int		check_death(t_data *data, int i);
int		check_all_ate(t_data *data);

// utils.c
long	get_time(void);
void	print_msg(t_philo *philo, char *msg);
void	precise_sleep(long ms);
int		ft_atoi(char *str);

#endif