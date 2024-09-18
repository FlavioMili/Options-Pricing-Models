#include <cmath>
#include <random>

using namespace std;

vector<vector<double>> mertonJumpDiffusionModel(double S0, double r, double sigma, double lambda, double mu_j, double sigma_j, double T, int steps, int simulations = 100) {
    vector<vector<double>> paths(simulations, vector<double>(steps + 1));
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<> normal(0, 1);
    poisson_distribution<> poisson(lambda * T / steps);

    double dt = T / steps;

    for (int sim = 0; sim < simulations; ++sim) {
        double S = S0;
        paths[sim][0] = S;

        for (int i = 1; i <= steps; ++i) {
            double Z = normal(gen);
            int N = poisson(gen);

            double sum_jumps = 0;
            for (int j = 0; j < N; ++j) {
                double Y = exp(mu_j + sigma_j * normal(gen));
                sum_jumps += log(Y);
            }

            S *= exp((r - 0.5 * sigma * sigma - lambda * (exp(mu_j + 0.5 * sigma_j * sigma_j) - 1)) * dt
                     + sigma * sqrt(dt) * Z + sum_jumps);
            paths[sim][i] = S;
        }
    }

    return paths;
}