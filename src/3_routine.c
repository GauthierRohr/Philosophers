/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_routine.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:48:53 by grohr             #+#    #+#             */
/*   Updated: 2025/04/19 19:20:26 by grohr            ###   ########.fr       */
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
// Un mutex (mutual exclusion) est utilisé pour synchroniser l'accès
// à des ressources partagées entre plusieurs threads.
// Dans ce cas, last_meal et meals_eaten sont des ressources partagées, car :
//		- Le thread du philosophe écrit dans ces variables
//		- Le thread superviseur lit ces variables
// Si ces accès ne sont pas protégés, il y a un risque de data race,
// c'est-à-dire une situation où deux threads accèdent simultanément à la même variable
// (l'un en écriture, l'autre en lecture), ce qui peut corrompre les données
// ou produire des résultats incohérents.
//
// death_mutex est un verrou qui protège les sections de code modifiant ou
// lisant last_meal et meals_eaten.
// =>ne contient pas ces variables: empêche les threads d'y accéder en simultané
//
// ANALOGIE :
// last_meal et meals_eaten sont comme un carnet partagé.
// death_mutex est une clé qui verrouille le carnet.
// Quand un thread (philosophe ou superviseur) veut écrire ou lire
// dans le carnet, il doit prendre la clé (pthread_mutex_lock).
// Tant qu'il a la clé, les autres doivent attendre.
// Il rend la clé (pthread_mutex_unlock) après avoir fini.
// La clé ne contient pas le carnet, mais elle contrôle qui peut y accéder.
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
//
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
	if (philo->data->num_philos > 1 && philo->id % 2 == 0)
		usleep(1000);
	while (1)
	{
        pthread_mutex_lock(&philo->data->death_mutex);
        if (philo->data->simulation_stop)
		{
            pthread_mutex_unlock(&philo->data->death_mutex);
            break;
        }
        pthread_mutex_unlock(&philo->data->death_mutex);
        take_forks(philo);
        if (philo->data->num_philos == 1)
            break;
        eat(philo);
        if (philo->data->max_meals > 0 && philo->meals_eaten >= philo->data->max_meals)
            break;
        pthread_mutex_lock(&philo->data->death_mutex);
		print_msg(philo, "is sleeping");
		pthread_mutex_unlock(&philo->data->death_mutex);
        precise_sleep(philo->data->time_to_sleep);
		pthread_mutex_lock(&philo->data->death_mutex);
        print_msg(philo, "is thinking");
		pthread_mutex_unlock(&philo->data->death_mutex);
    }
    return (NULL);
}
