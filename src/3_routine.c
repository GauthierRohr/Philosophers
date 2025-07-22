/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_routine.c                                        :+:      :+:    :+:   */
/*                                                    :::      ::::    ::::  */
/*   By: grohr <grohr@student.42.fr>                +:++:+++:  +:++:+++:    */
/*                                                +:++:+++:  +:++:+++:    */
/*   Created: 2025/07/22 18:04:27 by grohr            ###   ###             */
/*   Updated: 2025/07/22 21:00:00 by grohr            ###   ###             */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	eat(t_philo *philo)
{
	print_msg(philo, "is eating");
	pthread_mutex_lock(&philo->data->death_mutex);
	philo->last_meal = get_time();
	philo->meals_eaten++;
	if (philo->data->max_meals > 0 && philo->meals_eaten == philo->data->max_meals)
	{
		philo->data->tot_finish_meal++;
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
	precise_sleep(philo->data->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	take_forks(t_philo *philo)
{
	if (philo->data->num_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_msg(philo, "has taken a fork");
		precise_sleep(philo->data->time_to_die);
		pthread_mutex_unlock(philo->left_fork);
		return ;
	}
	if (philo->id % 2)
	{
		print_msg(philo, "waiting for left fork");
		pthread_mutex_lock(philo->left_fork);
		print_msg(philo, "has taken a fork");
		print_msg(philo, "waiting for right fork");
		pthread_mutex_lock(philo->right_fork);
		print_msg(philo, "has taken a fork");
	}
	else
	{
		print_msg(philo, "waiting for right fork");
		pthread_mutex_lock(philo->right_fork);
		print_msg(philo, "has taken a fork");
		print_msg(philo, "waiting for left fork");
		pthread_mutex_lock(philo->left_fork);
		print_msg(philo, "has taken a fork");
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	long	time_since_meal;

	philo = (t_philo *)arg;
	// Délai initial échelonné pour éviter la contention au démarrage
	if (philo->data->num_philos > 1)
		precise_sleep((philo->id - 1) % 2); // 0 ou 1ms selon pair/impair
	while (1)
	{
		pthread_mutex_lock(&philo->data->death_mutex);
		if (philo->data->simulation_stop)
		{
			pthread_mutex_unlock(&philo->data->death_mutex);
			break;
		}
		time_since_meal = get_time() - philo->last_meal;
		if (time_since_meal > philo->data->time_to_die * 0.9)
			print_msg(philo, "is at risk of starvation");
		pthread_mutex_unlock(&philo->data->death_mutex);
		// Délai adaptatif : priorité maximale si proche de la mort
		if (time_since_meal > philo->data->time_to_die * 0.5)
			precise_sleep(0); // Pas de délai si risque de mort
		else
			precise_sleep(1); // Délai constant pour les autres
		take_forks(philo);
		if (philo->data->num_philos == 1)
			break;
		eat(philo);
		if (philo->data->max_meals > 0 && philo->meals_eaten >= philo->data->max_meals)
			break;
		print_msg(philo, "is sleeping");
		precise_sleep(philo->data->time_to_sleep);
		print_msg(philo, "is thinking");
		precise_sleep(1); // Délai pour réduire la contention
	}
	return (NULL);
}
