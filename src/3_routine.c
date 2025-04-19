/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_routine.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:48:53 by grohr             #+#    #+#             */
/*   Updated: 2025/04/19 15:00:59 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

//eat:
//- Simule l'action de manger du philosophe.
//- Met à jour le temps du dernier repas et le compteur de repas.
//- Attend le temps de repas (time_to_eat) et relâche les fourchettes.
//- pthread_mutex_unlock libère les mutexes pour permettre à d'autres d'utiliser
//  les fourchettes.
//
void	eat(t_philo *philo)
{
	print_msg(philo, "is eating");
	pthread_mutex_lock(&philo->data->death_mutex);
	philo->last_meal = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->death_mutex);
	precise_sleep(philo->data->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

//take_forks:
//- Prend les deux fourchettes du philosophe avec pthread_mutex_lock.
//- Utilise la méthode asymétrique :
//		- philo impairs prennent la fourchette gauche d'abord
//		- philo pairs celle de droite
//- Affiche "has taken a fork" après chaque prise.
//- pthread_mutex_lock verrouille un mutex, bloquant si fourchette déjà prise.
//
//
void	take_forks(t_philo *philo)
{
	if (philo->id % 2)
	{
		pthread_mutex_lock(philo->left_fork);
		print_msg(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_msg(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_msg(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_msg(philo, "has taken a fork");
	}
}

//philo_routine:
//- Routine exécutée par chaque thread philosophe.
//- Boucle pour prendre les fourchettes, manger, dormir et penser.
//- S'arrête si la simulation est terminée ou si max_meals est atteint.
//- Les philos pairs attendent au départ pour éviter conflits.
//
// *arg est le dernier argument de pthread_create :
// pthread_create(&data->philos[i].thread, NULL, philo_routine, &data->philos[i])
//
void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (!philo->data->simulation_stop)
	{
		take_forks(philo);
		eat(philo);
		if (philo->data->max_meals > 0
			&& philo->meals_eaten >= philo->data->max_meals)
			break ;
		print_msg(philo, "is sleeping");
		precise_sleep(philo->data->time_to_sleep);
		print_msg(philo, "is thinking");
	}
	return (NULL);
}
