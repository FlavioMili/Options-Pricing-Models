# Financial Models for C++ Simulations

## 0. [Black-Scholes Model](https://en.wikipedia.org/wiki/Black%E2%80%93Scholes_model)
- **Type**: Analytical (Closed-form solution).
- **Description**: Calculates the price of European call/put options using a formula involving the stock price, volatility, time to maturity, and risk-free rate.
- **Difficulty**: Easy.

## 1. [Binomial Option Pricing Model](https://en.wikipedia.org/wiki/Binomial_options_pricing_model)
- **Type**: Discrete, tree-based.
- **Description**: Uses a binomial tree to estimate the price of options by simulating possible price paths over time.
- **Difficulty**: Easy.

## 2. [Monte Carlo Simulation (Option Pricing)](https://en.wikipedia.org/wiki/Monte_Carlo_methods_for_option_pricing)
- **Type**: Stochastic, random sampling.
- **Description**: Simulates multiple random paths for asset prices to estimate the average payoff of options.
- **Difficulty**: Easy.

## 3. [Vasicek Model](https://en.wikipedia.org/wiki/Vasicek_model)
- **Type**: Stochastic process (Interest rates).
- **Description**: Simulates interest rates using a mean-reverting process, useful for bond pricing.
- **Difficulty**: Moderate.

## 4. [Cox-Ross-Rubinstein (CRR) Model](https://en.wikipedia.org/wiki/Binomial_options_pricing_model#The_Cox,_Ross,_Rubinstein_Model)
- **Type**: Discrete, tree-based.
- **Description**: Generalized binomial model, handling complex conditions like time-varying volatility and interest rates.
- **Difficulty**: Moderate.

## 5. [Hull-White Model](https://en.wikipedia.org/wiki/Hull%E2%80%93White_model)
- **Type**: Stochastic process (Interest rates).
- **Description**: Models interest rates using a Gaussian process with mean-reversion, useful for bond and derivative pricing.
- **Difficulty**: Moderate.

## 6. [Heston Model](https://en.wikipedia.org/wiki/Heston_model)
- **Type**: Stochastic process (Volatility).
- **Description**: Simulates both asset prices and their stochastic volatility, extending Black-Scholes for more realistic pricing.
- **Difficulty**: Advanced.

## 7. [Merton Jump-Diffusion Model](https://en.wikipedia.org/wiki/Jump_diffusion)
- **Type**: Stochastic process with jumps.
- **Description**: Extends Black-Scholes by adding random jumps in asset prices for sudden market changes.
- **Difficulty**: Advanced.

## 8. [GARCH Model](https://en.wikipedia.org/wiki/Autoregressive_conditional_heteroskedasticity)
- **Type**: Time-series volatility modeling.
- **Description**: Models financial time series with changing volatility, commonly used in risk management.
- **Difficulty**: Advanced.
