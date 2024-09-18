#include <cmath>  

using namespace std;

double normalCDF(double x) {
    return 0.5 * (1.0 + erf(x / sqrt(2.0)));
}

double blackScholesOptions(double S, double K, double T, double r, double sigma, bool isCall){
    double d1 = (log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
    double d2 = d1 - sigma * sqrt(T);

    if (isCall) 
        return S * normalCDF(d1) - K * exp(-r * T) * normalCDF(d2);
    else 
        return K * exp(-r * T) * normalCDF(-d2) - S * normalCDF(-d1);
}

