#include <cstdlib>
#include <iostream>
#include <math.h>
#include <unistd.h>

using namespace std;

# define single_walk_mode 		0x01
# define mean_final_dist_mode	0x02
# define mean_final_xy_mode	0x04

void usage()
{
	cout << "usage : " << endl;
	cout << "[run a single random walk]: drunk -s" << endl;
	cout << "[compute distance square-type as a function of the number of steps]: drunk -d" << endl;
	cout << "[compute distribution of x coordinate]: drunk -x|y" << endl;
}

double inline distance(int *pos)
{
	double d;
	d = pos[0] * pos[0] + pos[1] * pos[1];
	d = sqrt(d);
	return d;
}

void single_walk(unsigned int nstep, int *final_pos, bool verbose)
{
	unsigned int i;
	int delta_x, delta_y;
	float x_n(0), y_n(0);

	for (i = 0; i < nstep; i++)
	{
		if ( verbose )
		{
			cout << x_n << " " << y_n << endl;
		}

		/* on tire au sort un deplacement
		 * suivant une loi de probabilite uniforme
		 */
		delta_x = (rand() & 0x01) ? 1 : -1;
		delta_y = (rand() & 0x01) ? 1 : -1;

		x_n += delta_x;
		y_n += delta_y;
	}
	
	if ( verbose )
	{
		cout << x_n << " " << y_n << endl;
	}

	if (final_pos != NULL)
	{
		final_pos[0] = x_n;
		final_pos[1] = y_n;
	}
}

void multiple_walk(unsigned int nwalk, unsigned int nstep, int **final_pos_vector)
{
	unsigned int i;

	for (i = 0; i < nwalk; i++)
	{
		single_walk(nstep, final_pos_vector[i], false);
		//cout << "walk n° " << i << " final pos : (" << final_pos_vector[i][0] << ", " << final_pos_vector[i][1] << ")" << endl;
	}
}

int main(int argc, char **argv)
{
	int c;
	char mode(0);

	while (( c = getopt(argc, argv, "sdxy")) != -1)
	{
		switch (c)
		{
			case 's':
				mode |= single_walk_mode;
				break;
			case 'd':
				mode |= mean_final_dist_mode;
				break;
			case 'x':
			case 'y':
				mode |= mean_final_xy_mode;
				break;
			default:
				usage();
				return 0;
		}
	}
	
	if ( ((mode & single_walk_mode) != 0) && ((mode & mean_final_dist_mode) != 0) && ((mode & mean_final_xy_mode) != 0) )
	{
		usage();
		return 0;
	}

	srand(time(NULL));

	if ( mode == single_walk_mode )
	{
		unsigned int nstep(0);
	
		/* saisie utilisateur du nombre de pas
		 * realises dans chaque marche
		 */
		cerr << "Entrez un nombre de pas : ";
		cin >> nstep;

		if (nstep < 1)
		{
			usage();
			return 0;
		}
		single_walk(nstep, NULL, true);
	}
	else if ( mode == mean_final_dist_mode )
	{
		unsigned int i(0), n(0);
		unsigned int step_min(0);
		unsigned int step_max(0);

		int final_pos[2];

		double d(0), x_mean(0), y_mean(0), d_mean(0), d_sigma(0);

		/* saisie utilisateur du nombre de pas minimal
		 */
		cerr << "Entrez un nombre de pas minimal : ";
		cin >> step_min;

		/* saisie utilisateur du nombre de pas maximal
		 */
		cerr << "Entrez un nombre de pas maximal : ";
		cin >> step_max;

		if (step_min < 1 || step_max < 1 || step_max <= step_min + 50)
		{
			usage();
			return 0;
		}

		for (n = step_min; n < step_max; n += 50)
		{
			cout << n << " ";

#define NMARCHES 10000
		
			for (i = 0; i < NMARCHES; i++)
			{
				single_walk(n, final_pos, false);

				x_mean += final_pos[0];				
				y_mean += final_pos[0];
				
				d = distance(final_pos);
				d_mean += d;
				d_sigma += d * d;
			}

			x_mean = x_mean / NMARCHES;
			y_mean = y_mean / NMARCHES;
			d_mean = d_mean / NMARCHES;

			cout << x_mean << " " << y_mean << " " << d_mean << " ";

			d_sigma = d_sigma / NMARCHES;
			d_sigma = d_sigma - d_mean * d_mean;
			d_sigma = sqrt(d_sigma);
			cout << d_sigma << endl;
		}
	}
	else if ( mode == mean_final_xy_mode )
	{
		int nwalk(0), nstep(0), i(0);

		/* saisie utilisateur du nombre de marches
		 */
		cerr << "Entrez un nombre de marches : ";
		cin >> nwalk;

		/* saisie utilisateur du nombre de pas
		 */
		cerr << "Entrez un nombre de pas : ";
		cin >> nstep;

		int **final_pos_vector = (int **)malloc(nwalk * sizeof(int *));

		for (i = 0; i < nwalk; i++)
		{
			final_pos_vector[i] = (int *)malloc(2 * sizeof(int));
		}

		double *proba_x = (double *)malloc( (2* nstep + 1) * sizeof(double));
		double *proba_y = (double *)malloc( (2* nstep + 1) * sizeof(double));

		multiple_walk(nwalk, nstep, final_pos_vector);

		for (i = 0; i < nwalk; i++)
		{
			proba_x[ final_pos_vector[i][0] + nstep ] += 1;
			proba_y[ final_pos_vector[i][1] + nstep ] += 1;
		}
		
		for (i = 0; i < 2 * nstep + 1; i++)
		{
			proba_x[i] = proba_x[i] / nwalk;
			proba_y[i] = proba_y[i] / nwalk;
			
			cout << i - nstep << " " << proba_x[i] << " " << proba_y[i] << endl;
		}
	}
	else
	{
		usage();
	}
	
	return 0;
}
