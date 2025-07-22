/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4_supervisor.c                                     :+:      :+:    :+:   */
/*                                                    :::      ::::    ::::  */
/*   By: grohr <grohr@student.42.fr>                +:++:+++:  +:++:+++:    */
/*                                                +:++:+++:  +:++:+++:    */
/*   Created: 2025/04/18 18:48:56 by grohr            ###   ###             */
/*   Updated: 2025/07/22 20:00:00 by grohr            ###   ###             */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	check_death(t_data *data, int i)
{
	long	current_time;
	long	time_since_meal;

	pthread_mutex_lock(&data->death_mutex);
	current_time = get_time();
	time_since_meal = current_time - data->philos[i].last_meal;
	if (time_since_meal > data->time_to_die)
	{
		long death_time = data->philos[i].last_meal + data->time_to_die;
		long timestamp = death_time - data->start_time;
		pthread_mutex_lock(&data->log_mutex);
		if (!data->simulation_stop)
		{
			printf("%ld %d died\n", timestamp, data->philos[i].id);
			// Journal de débogage pour comprendre pourquoi
			printf("DEBUG: Philo %d last meal at %ld, died at %ld, time since meal %ld\n",
				data->philos[i].id, data->philos[i].last_meal - data->start_time,
				current_time - data->start_time, time_since_meal);
		}
		data->simulation_stop = 1;
		pthread_mutex_unlock(&data->log_mutex);
		pthread_mutex_unlock(&data->death_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->death_mutex);
	return (0);
}

int	check_all_ate(t_data *data)
{
	int	finished;

	if (data->max_meals <= 0)
		return (0);
	pthread_mutex_lock(&data->death_mutex);
	finished = (data->tot_finish_meal >= data->num_philos);
	if (finished)
		data->simulation_stop = 1;
	pthread_mutex_unlock(&data->death_mutex);
	return (finished);
}

void	*supervisor_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		while (i < data->num_philos)
		{
			if (check_death(data, i))
				return (NULL);
			i++;
		}
		if (check_all_ate(data))
			return (NULL);
		precise_sleep(1);
	}
	return (NULL);
}
