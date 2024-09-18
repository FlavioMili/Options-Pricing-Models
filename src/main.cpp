#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

#include "../imgui/imgui.h"
#include "../imgui/backends/imgui_impl_glfw.h"
#include "../imgui/backends/imgui_impl_opengl3.h"

// Include your model headers here
#include "MC_option.cpp"
#include "binomial_option_pricing.cpp"
#include "black_scholes_model.cpp"
#include "crr.cpp"
#include "garch.cpp"
#include "heston.cpp"
#include "hullwhite.cpp"
#include "merton_JD.cpp"
#include "vasicek.cpp"

#include "../implot/implot.h"

int main()
{
    // Initialize GLFW and OpenGL
    if (!glfwInit()) return 1;

    // Set OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Get primary monitor resolution
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primary);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Financial Models GUI", NULL, NULL);
    if (!window) { glfwTerminate(); return 1; }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Print OpenGL version info
    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    std::cout << "OpenGL Version: " << version << std::endl;
    std::cout << "GLSL Version: " << glslVersion << std::endl;

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);

    // Configure ImGui style
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

    // Set GLSL version
    const char* glsl_version = "#version 150";
    #ifdef __APPLE__
        // GL 3.2 + GLSL 150
        glsl_version = "#version 150";
    #else
        // GL 3.0 + GLSL 130
        glsl_version = "#version 130";
    #endif
    ImGui_ImplOpenGL3_Init(glsl_version);

    // State variables
    static int currentModel = 0;
    static double S0 = 100.0, K = 100.0, T = 1.0, r = 0.05, sigma = 0.2;
    static double param1 = 0.0, param2 = 0.0, param3 = 0.0, param4 = 0.0;
    static std::vector<double> output;
    static bool isCall = true;
    static int steps = 100;
    static std::vector<std::vector<double>> multipleOutputs;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Get the current window size
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);

        // Create a full-window ImGui window
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(display_w, display_h));
        ImGui::Begin("Financial Models", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus);

        // Create columns for layout
        ImGui::Columns(3, "MainColumns", false);
        ImGui::SetColumnWidth(0, 200); // Model selection column
        ImGui::SetColumnWidth(1, 300); // Parameters column 1
        ImGui::SetColumnWidth(2, 300); // Parameters column 2

        // Model selection column
        ImGui::Text("Select Model:");
        const char* models[] = {"Monte Carlo Option", "Binomial Option Pricing", "Black-Scholes", "CRR", "GARCH", "Heston", "Hull-White", "Merton Jump Diffusion", "Vasicek"};
        for (int i = 0; i < IM_ARRAYSIZE(models); i++)
        {
            if (ImGui::RadioButton(models[i], &currentModel, i))
            {
                param1 = param2 = param3 = param4 = 0.0;
            }
        }

        ImGui::NextColumn();

        // Parameters column 1
        ImGui::Text("Model Inputs:");
        ImGui::InputDouble("Initial Stock Price (S0)", &S0, 0.0, 0.0, "%.3f");
        ImGui::InputDouble("Strike Price (K)", &K, 0.0, 0.0, "%.3f");
        ImGui::InputDouble("Time to Maturity (T)", &T, 0.0, 0.0, "%.3f");
        ImGui::InputDouble("Risk-free Rate (r)", &r, 0.0, 0.0, "%.3f");
        ImGui::InputDouble("Volatility (sigma)", &sigma, 0.0, 0.0, "%.3f");
        if (currentModel != 2) { // If not Black-Scholes
            ImGui::InputInt("Steps", &steps);
        }
        ImGui::Checkbox("Call Option", &isCall);

        ImGui::NextColumn();

        ImGui::Text("Additional Parameters:");
        switch (currentModel)
        {
            case 4: // GARCH
                ImGui::InputDouble("Omega", &param1, 0.0, 0.0, "%.3f");
                ImGui::InputDouble("Alpha", &param2, 0.0, 0.0, "%.3f");
                ImGui::InputDouble("Beta", &param3, 0.0, 0.0, "%.3f");
                ImGui::InputDouble("Initial Variance", &param4, 0.0, 0.0, "%.3f");
                break;
            case 5: // Heston
                ImGui::InputDouble("Initial Volatility (V0)", &param1, 0.0, 0.0, "%.3f");
                ImGui::InputDouble("Mean Reversion Speed (kappa)", &param2, 0.0, 0.0, "%.3f");
                ImGui::InputDouble("Long-term Variance (theta)", &param3, 0.0, 0.0, "%.3f");
                ImGui::InputDouble("Volatility of Volatility (sigma)", &param4, 0.0, 0.0, "%.3f");
                break;
            case 7: // Merton Jump Diffusion
                ImGui::InputDouble("Jump Intensity (lambda)", &param1, 0.0, 0.0, "%.3f");
                ImGui::InputDouble("Jump Size Mean (mu_j)", &param2, 0.0, 0.0, "%.3f");
                ImGui::InputDouble("Jump Size Volatility (sigma_j)", &param3, 0.0, 0.0, "%.3f");
                break;
        }

        ImGui::Columns(1);

        // Run button
        if (ImGui::Button("Run Model"))
        {
            output.clear();
            multipleOutputs.clear();
            switch (currentModel)
            {
                case 0: // Monte Carlo Option
                    multipleOutputs = monteCarloOption(S0, K, T, r, sigma, steps, isCall, 1000);
                    break;
                case 1: // Binomial Option Pricing
                    output.push_back(binomialOptionPricing(S0, K, T, r, sigma, steps, isCall));
                    multipleOutputs = binomialTreePaths(S0, K, T, r, sigma, steps);
                    break;
                case 2: // Black-Scholes
                    output.push_back(blackScholesOptions(S0, K, T, r, sigma, isCall));
                    break;
                case 3: // CRR
                    output.push_back(crrModel(S0, K, T, r, sigma, steps, isCall));
                    multipleOutputs = crrTreePaths(S0, K, T, r, sigma, steps);
                    break;
                case 4: // GARCH
                    multipleOutputs = garchModel(param1, param2, param3, param4, steps);
                    break;
                case 5: // Heston
                {
                    HestonResult result = hestonModel(S0, param1, param2, param3, param4, 0.5, r, T, steps);
                    output.push_back(result.price);
                    output.push_back(result.volatility);
                    multipleOutputs = result.pricePaths;
                    break;
                }
                case 6: // Hull-White
                    multipleOutputs = hullWhiteModel(r, param1, sigma, T, steps);
                    break;
                case 7: // Merton Jump Diffusion
                    multipleOutputs = mertonJumpDiffusionModel(S0, r, sigma, param1, param2, param3, T, steps);
                    break;
                case 8: // Vasicek
                    multipleOutputs = vasicekModel(r, param1, param2, sigma, T, steps);
                    break;
            }
        }

        // Output display
        if (!output.empty() || !multipleOutputs.empty())
        {
            ImGui::Text("Output:");
            if (!output.empty())
            {
                for (size_t i = 0; i < output.size(); ++i)
                {
                    ImGui::Text("Result %zu: %.4f", i + 1, output[i]);
                }
            }
            else if (!multipleOutputs.empty())
            {
                // Calculate some statistics for models without explicit output
                double mean = 0.0, min = std::numeric_limits<double>::max(), max = std::numeric_limits<double>::lowest();
                for (const auto& path : multipleOutputs)
                {
                    double lastValue = path.back();
                    mean += lastValue;
                    min = std::min(min, lastValue);
                    max = std::max(max, lastValue);
                }
                mean /= multipleOutputs.size();

                ImGui::Text("Mean final value: %.4f", mean);
                ImGui::Text("Min final value: %.4f", min);
                ImGui::Text("Max final value: %.4f", max);
            }
            
            // Graph selection
            static int currentGraph = 0;
            const char* graphOptions[] = {"Model Output", "Price Distribution"};
            ImGui::Combo("Select Graph", &currentGraph, graphOptions, IM_ARRAYSIZE(graphOptions));

            // Calculate appropriate y-axis limits
            float yMin = std::numeric_limits<float>::max();
            float yMax = std::numeric_limits<float>::lowest();
            if (!multipleOutputs.empty())
            {
                for (const auto& path : multipleOutputs)
                {
                    for (const auto& value : path)
                    {
                        yMin = std::min(yMin, static_cast<float>(value));
                        yMax = std::max(yMax, static_cast<float>(value));
                    }
                }
            }
            else if (!output.empty())
            {
                for (const auto& value : output)
                {
                    yMin = std::min(yMin, static_cast<float>(value));
                    yMax = std::max(yMax, static_cast<float>(value));
                }
            }

            // Add some padding to y-axis limits
            float yRange = yMax - yMin;
            yMin -= yRange * 0.1f;
            yMax += yRange * 0.1f;

            // Use ImPlot to create plots
            ImPlotFlags flags = ImPlotFlags_NoBoxSelect;
            if (currentGraph == 0)
            {
                // Calculate the full range of the data
                int maxSteps = steps;
                if (!multipleOutputs.empty())
                {
                    maxSteps = multipleOutputs[0].size() - 1;
                }
                else if (!output.empty())
                {
                    maxSteps = output.size() - 1;
                }

                // Static variable to keep track of the zoom level
                static float zoom = 1.0f;

                // Allow zooming with mouse wheel
                if (ImGui::IsWindowHovered())
                {
                    zoom *= (1 + ImGui::GetIO().MouseWheel * 0.1f);
                    zoom = std::max(0.1f, std::min(zoom, 1.0f)); // Clamp zoom between 0.1 and 1.0
                }

                // Calculate the visible range based on zoom level
                double visible_range = maxSteps / zoom;
                double center = maxSteps / 2.0;
                double x_min = std::max(0.0, center - visible_range / 2.0);
                double x_max = std::min(static_cast<double>(maxSteps), center + visible_range / 2.0);

                if (ImPlot::BeginPlot("Model Output", ImVec2(-1, 400), flags))
                {
                    // Set up axes before plotting
                    ImPlot::SetupAxes("Steps", "Price", ImPlotAxisFlags_None, ImPlotAxisFlags_None);
                    ImPlot::SetupAxisLimits(ImAxis_X1, x_min, x_max, ImGuiCond_Always);
                    ImPlot::SetupAxisLimits(ImAxis_Y1, yMin, yMax, ImGuiCond_Always);
                    
                    // Now plot the data
                    if (!multipleOutputs.empty())
                    {
                        for (size_t i = 0; i < std::min(multipleOutputs.size(), size_t(10)); ++i)
                        {
                            std::vector<double> x(multipleOutputs[i].size());
                            std::iota(x.begin(), x.end(), 0);
                            ImPlot::PlotLine(("Path " + std::to_string(i)).c_str(), x.data(), multipleOutputs[i].data(), multipleOutputs[i].size());
                        }
                    }
                    else if (!output.empty())
                    {
                        std::vector<double> x(output.size());
                        std::iota(x.begin(), x.end(), 0);
                        ImPlot::PlotLine("Output", x.data(), output.data(), output.size());
                    }
                    ImPlot::EndPlot();
                }

                // Display zoom level
                ImGui::Text("Zoom: %.2f%%", zoom * 100);
            }

            // Add histogram for Monte Carlo simulations
            if (currentGraph == 1 && currentModel == 0 && !multipleOutputs.empty())
            {
                if (ImPlot::BeginPlot("Price Distribution", ImVec2(-1, 400), flags))
                {
                    std::vector<double> finalPrices;
                    for (const auto& path : multipleOutputs)
                    {
                        finalPrices.push_back(path.back());
                    }
                    ImPlot::SetupAxes("Price", "Frequency", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
                    ImPlot::PlotHistogram("Distribution", finalPrices.data(), finalPrices.size());
                    ImPlot::EndPlot();
                }
            }
        }

        ImGui::End();
        ImGui::Render();
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImPlot::DestroyContext();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}