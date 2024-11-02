#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

//Define a structure to hold city coordinates
struct City {
    int id;
    double x, y;
};

//Calculate Euclidean distance between two cities
double euclideanDistance(const City &a, const City &b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

//Calculate the total cost of a tour
double calculateTourCost(const vector<City> &cities, const vector<int> &tour) {
    double cost = 0.0;
    for (size_t i = 0; i < tour.size() - 1; i++) {
        cost += euclideanDistance(cities[tour[i]], cities[tour[i + 1]]);
    }
    cost += euclideanDistance(cities[tour.back()], cities[tour.front()]); // Return to starting city
    return cost;
}

//Swap two cities in the tour to create a neighboring solution
void swapCities(vector<int> &tour) {
    int i = rand() % tour.size();
    int j = rand() % tour.size();
    swap(tour[i], tour[j]);
}

//Simulated Annealing algorithm for TSP
pair<double, vector<int>> simulatedAnnealing(vector<City> &cities, int iterations = 10000) {
    double T = 10000.0;          // Initial temperature
    double coolingRate = 0.995;  // Cooling rate
    int n = cities.size();
    vector<int> currentTour(n);
    iota(currentTour.begin(), currentTour.end(), 0); // Initialize tour with 0, 1, 2, ..., n-1
    double currentCost = calculateTourCost(cities, currentTour);
    
    vector<int> bestTour = currentTour;
    double bestCost = currentCost;

    for (int iter = 0; iter < iterations; iter++) {
        vector<int> newTour = currentTour;
        swapCities(newTour);
        double newCost = calculateTourCost(cities, newTour);

        if (newCost < currentCost || exp((currentCost - newCost) / T) > (rand() / (double)RAND_MAX)) {
            currentTour = newTour;
            currentCost = newCost;

            if (currentCost < bestCost) {
                bestTour = currentTour;
                bestCost = currentCost;
            }
        }
        T *= coolingRate;  // Decrease temperature
    }
    return {bestCost, bestTour};
}

//Read cities from a TSP file (sno, x, y)
vector<City> readCitiesFromFile(const string &filename) {
    vector<City> cities;
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        City city;
        ss >> city.id >> city.x >> city.y;
        cities.push_back(city);
    }
    return cities;
}

int main() {
    srand(time(0));  //Initialize random seed

    string folderPath = "E:/iit test/project1-tsp/vlsi";  // Folder containing TSP files
    string fileName = "E:/iit test/project1-tsp/vlsi/ara238025.tsp";  //Example file path for TSP data

    vector<City> cities = readCitiesFromFile(fileName);
    
    //Simulated Annealing for TSP
    auto result = simulatedAnnealing(cities, 10000);
    double bestCost = result.first;
    vector<int> bestTour = result.second;

    //Output final cost and tour
    cout << "Final achieved cost: " << bestCost << endl;
    cout << "Optimal tour (city sequence): ";
    for (int city : bestTour) {
        cout << cities[city].id << " ";
    }
    cout << endl;

    return 0;
}
