#include <cmath>
#include <random>
#include <vector>

using namespace std;

vector<vector<double>> garchModel(double omega, double alpha, double beta, double initial_variance, int n, int simulations = 100) {
    vector<vector<double>> multipleOutputs(simulations, vector<double>(n));
    
    for (int sim = 0; sim < simulations; ++sim) {
        random_device rd;
        mt19937 gen(rd());
        normal_distribution<> d(0, 1);

        vector<double> variances(n);
        variances[0] = initial_variance;
        multipleOutputs[sim][0] = sqrt(variances[0]) * d(gen);

        for (int i = 1; i < n; ++i) {
            variances[i] = omega + alpha * pow(multipleOutputs[sim][i-1], 2) + beta * variances[i-1];
            multipleOutputs[sim][i] = sqrt(variances[i]) * d(gen);
        }
    }

    return multipleOutputs;
}