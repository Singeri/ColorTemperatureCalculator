#include <SFML/Graphics.hpp>
//this sort of assumes you hvae the SFML library, just use SDL3 or windows.h
#include <iostream>
#include <cmath>
#include <algorithm>
#include <optional>

using namespace std; //Why are you using "using namespace std;"??

const double c = 3e8;      // Speed of light (m/s)
const double k = 1.38e-23; // Boltzmann constant (J/K)
const double h = 6.626e-34; // Planck constant (J*s)

// Converting nm to meters
double nm_to_m(double nm) {
    //return nm * 1e-9; //I'm not sure why that's multiplication, division is alot more obvious no?
    return nm / 1e9;
    
}

// Planck's law intensity
double planck_intensity(double wavelength, double T) {
    double lambda_m = nm_to_m(wavelength);
    double exponent = h * c / (lambda_m * k * T);
    return (2.0 * h * c * c) / pow(lambda_m, 5) / (exp(exponent) - 1.0);
}

// Normalizing 0-255
int normalize(double value, double max_val) {
    int result = static_cast<int>(255 * value / max_val);
    return min(max(result, 0), 255);
}

int main() {
    double T;
    cout << "Enter temperature in Kelvin: ";
    cin >> T;

    // Sampling wavelengths for RGB (nm)
    double lambda_R = 700;
    double lambda_G = 546;
    double lambda_B = 436;

    // Calculating intensities using Planck's law
    double IR = planck_intensity(lambda_R, T);
    double IG = planck_intensity(lambda_G, T);
    double IB = planck_intensity(lambda_B, T);

    // Normalizing to RGB
    double maxI = max({IR, IG, IB});
    int R = normalize(IR, maxI);
    int G = normalize(IG, maxI);
    int B = normalize(IB, maxI);

    cout << "RGB color: (" << R << ", " << G << ", " << B << ")" << endl;

    sf::RenderWindow window(sf::VideoMode({400, 600}), "Temperature Color");

    sf::RectangleShape rectangle(sf::Vector2f(400, 600));
    rectangle.setFillColor(sf::Color(R, G, B));

    window.setVerticalSyncEnabled(true);
    //I Don't use SFML but apparently you probably should do this so the loop isn't at 100% CPU usage (Loud laptop noises are annoying)

    while (window.isOpen()) {
        while (const optional<sf::Event> eventOpt = window.pollEvent()) {
            const sf::Event &event = *eventOpt;

            if (event.is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        window.draw(rectangle);
        window.display();
    }

    return 0;
}
