#include <cmath>
#include <random>

using namespace std;

vector<vector<double>> hullWhiteModel(double r0, double a, double sigma, double T, int steps, int simulations = 100) {
    vector<vector<double>> paths(simulations, vector<double>(steps + 1));
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<> d(0, 1);

    double dt = T / steps;

    for (int sim = 0; sim < simulations; ++sim) {
        double r = r0;
        paths[sim][0] = r;

        for (int i = 1; i <= steps; ++i) {
            double theta = 0.1; // This should be made dynamical later on
            double dr = (theta - a * r) * dt + sigma * sqrt(dt) * d(gen);
            r += dr;
            paths[sim][i] = r;
        }
    }

    return paths;
}
