#include <cmath>
#include <random>
#include <vector>

using namespace std;

vector<vector<double>> monteCarloOption(double S0, double K, double T, double r, double sigma, int steps, bool isCall, int simulations)
{
    vector<vector<double>> paths(simulations, vector<double>(steps + 1));
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<> d(0, 1);

    double dt = T / steps;
    double drift = (r - 0.5 * sigma * sigma) * dt;
    double vol = sigma * sqrt(dt);

    for (int sim = 0; sim < simulations; ++sim)
    {
        paths[sim][0] = S0;
        for (int step = 1; step <= steps; ++step)
        {
            double Z = d(gen);
            paths[sim][step] = paths[sim][step - 1] * exp(drift + vol * Z);
        }
    }

    return paths;
}
