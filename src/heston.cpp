#include <cmath>
#include <random>

using namespace std;

struct HestonResult {
    double price;
    double volatility;
    vector<vector<double>> pricePaths;
};

HestonResult hestonModel(double S0, double V0, double kappa, double theta, double sigma, double rho, double r, double T, int steps, int simulations = 100) {
    vector<vector<double>> pricePaths(simulations, vector<double>(steps + 1));
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<> d(0, 1);

    double dt = T / steps;
    double finalPrice = 0.0;
    double finalVol = 0.0;

    for (int sim = 0; sim < simulations; ++sim) {
        double S = S0;
        double V = V0;
        pricePaths[sim][0] = S;

        for (int step = 1; step <= steps; ++step) {
            double Z1 = d(gen);
            double Z2 = rho * Z1 + sqrt(1 - rho * rho) * d(gen);

            S *= exp((r - 0.5 * V) * dt + sqrt(V * dt) * Z1);
            V += kappa * (theta - V) * dt + sigma * sqrt(V * dt) * Z2;
            V = max(V, 0.0);

            pricePaths[sim][step] = S;
        }
        finalPrice += S;
        finalVol += sqrt(V);
    }

    return {finalPrice / simulations, finalVol / simulations, pricePaths};
}