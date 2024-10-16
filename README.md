# Financial Models GUI

This project is a graphical user interface for various financial models. The formulas are sourced from Wikipedia and implemented using C++ with ImGui for the interface and ImPlot for visualizations. 

Screenshot from the Montecarlo simulation and its graph:
<img width="1183" alt="GUI reference" src="https://github.com/user-attachments/assets/5df6a3bc-a69f-4c81-bbbd-486ce3553006">


## Features

- Monte Carlo Option Pricing
- Binomial Option Pricing
- Black-Scholes Model
- Cox-Ross-Rubinstein (CRR) Model
- GARCH Model
- Heston Model
- Hull-White Model
- Merton Jump Diffusion Model
- Vasicek Model

## Prerequisites

- C++ compiler with C++17 support
- CMake (version 3.10 or higher)
- GLFW3
- OpenGL

## Building the Project

1. Clone the repository:
   ```
   git clone https://github.com/FlavioMili/Options-Pricing-Models.git
   cd Options-Pricing-Models
   ```

2. Create a build directory and navigate to it:
   ```
   mkdir build && cd build
   ```

3. Run CMake and build the project:
   ```
   cmake ..
   make
   ```

4. Run the executable:
   ```
   ./ModelGUI
   ```

## Usage

1. Select a financial model from the left panel.
2. Input the required parameters in the middle panel.
3. Click "Run Model" to execute the calculations.
4. View the results and graphs in the bottom panel.
