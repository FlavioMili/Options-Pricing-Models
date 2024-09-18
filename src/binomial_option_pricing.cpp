#include <cmath>
#include <vector>

using namespace std;

double binomialOptionPricing(double S, double K, double T, double r, double sigma, int steps, bool isCall) {
    double dt = T / steps;
    double u = exp(sigma * sqrt(dt));
    double d = 1 / u;
    double p = (exp(r * dt) - d) / (u - d);

    std::vector<double> prices(steps + 1);
    for (int i = 0; i <= steps; i++) {
        prices[i] = S * pow(u, steps - i) * pow(d, i);
    }

    for (int i = 0; i <= steps; i++) {
        prices[i] = isCall ? max(0.0, prices[i] - K) : max(0.0, K - prices[i]);
    }

    for (int step = steps - 1; step >= 0; step--) {
        for (int i = 0; i <= step; i++) {
            prices[i] = exp(-r * dt) * (p * prices[i] + (1 - p) * prices[i + 1]);
        }
    }

    return prices[0];
}

vector<vector<double>> binomialTreePaths(double S, double K, double T, double r, double sigma, int steps)
{
    double dt = T / steps;
    double u = exp(sigma * sqrt(dt));
    double d = 1 / u;

    vector<vector<double>> paths(steps + 1, vector<double>(steps + 1, 0.0));

    for (int i = 0; i <= steps; ++i)
    {
        for (int j = 0; j <= i; ++j)
        {
            paths[i][j] = S * pow(u, j) * pow(d, i - j);
        }
    }

    return paths;
}

