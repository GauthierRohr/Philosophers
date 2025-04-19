/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_init.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:48:44 by grohr             #+#    #+#             */
/*   Updated: 2025/04/19 18:57:47 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

//init_philos:
//- Alloue et configure les structures des philosophes.
//- Associe les fourchettes gauche et droite à chaque philosophe.
//- Initialise le temps du dernier repas et le compteur de repas.
//- Retourne 1 en cas d'erreur, 0 sinon.
//
// Chaque philosophe a des attributs, dont left_fork et right_fork,
// qui sont des pointeurs vers des mutexes représentant les fourchettes
// qu'il peut utiliser pour manger.
//
//data->philos : Un tableau de num_philos structures t_philo,
// alloué dynamiquement dans init_philos.
//
// data->forks : tableau de num_philos mutexes (pthread_mutex_t),
// alloué dans init_mutexes.
// => Chaque mutex représente une fourchette.
//
// data->philos[i].left_fork = &data->forks[i];
// Assigne au champ left_fork du philosophe i l'adresse du mutex
// représentant la fourchette à sa gauche dans le tableau data->forks.
//
// &data->forks[(i + 1) % data->num_philos];
// Pour que la fourchette droite du dernier philo soit la 1ere.
// Ex : i = 2 (3eme tour car i start a 0)
// [(2 + 1) % 5] = 3 : tant qu'on est pas au max, res = fourchette suiv.
// 					   Puis des que i + 1 = n_max philo
//					   ==> 4 + 1 % 5 = 0 : c'est = a la 1ere fourchette.
//
// data->philos[i].data = data :
// On prend l'adresse de la structure principale data (type t_data*)
// On la stocke dans le champ data de la structure du philo (type t_data*)
// => crée une référence bidirectionnelle :
// 		- data contient tous les philosophes (data->philos)
//		- Chaque philosophe peut accéder à data via son champ data
// =>Permet un accès aux variables partagées :
//		Les philos ont besoin d'accéder aux mutex (log_mutex, death_mutex).
//		Ils doivent vérifier simulation_stop pour savoir quand s'arrêter.
///		Ils utilisent start_time pour calculer les timings.
// SANS cette assignation cela aurait ete impossible
// ===> RESUME :
// "Chaque philos doit connaître l'adresse de la struct principale qui contient
// toutes les infs partagées dont il aura besoin pendant son exécution."
//       
int	init_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (0);
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		data->philos[i].last_meal = get_time();
		data->philos[i].meals_eaten = 0;
		data->philos[i].data = data;
		i++;
	}
	return (1);
}

//init_mutexes:
//- Alloue et initialise mutexes pour les fourchettes
//- Initialise les mutexes pour les logs et la vérification des morts.
//- pthread_mutex_init : crée un mutex pour synchroniser l'accès aux
//  ressources partagées, empêchant les data races.
//
// la fonction pthread_mutex_init() suit la convention UNIX standards :
//		Elle retourne 0 en cas de succès
//		Elle retourne un code d'erreur (nombre positif) en cas d'échec
//
// init_mutex : Retourne 0 en cas d'erreur, 1 sinon.
//
int	init_mutexes(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
		return (0);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (0);
		i++;
	}
	if (pthread_mutex_init(&data->log_mutex, NULL))
		return (0);
	if (pthread_mutex_init(&data->death_mutex, NULL))
		return (0);
	return (1);
}

//parse_args:
//- Vérifie et convertit les arguments en paramètres de simulation.
//- Stocke nombre de philo, temps de mort, repas, sommeil, et repas max.
//- Retourne 0 si les arguments sont invalides, 1 sinon.
//
int	init_parse_args(int argc, char **argv, t_data *data)
{
	if (argc < 5 || argc > 6)
		return (0);
	data->num_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->max_meals = -1;
	if (argc == 6)
		data->max_meals = ft_atoi(argv[5]);
	if (data->num_philos <= 0
		|| data->time_to_die < 0
		|| data->time_to_eat < 0
		|| data->time_to_sleep < 0
		|| (argc == 6 && data->max_meals < 0))
		return (0);
	return (1);
}
