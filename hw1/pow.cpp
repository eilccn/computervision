/*
Eileen Chang

Program that does a timing comparison of using the C pow function to square a value versus using multiplication. 
Calculates the relative computational cost.
*/
#include <stdio.h>
#include <cmath>
#include <chrono>
#include <iostream>

using namespace std;

// class for timer using built-in chrono functions
class Timer {
  public:
    explicit Timer () : from (std::chrono::high_resolution_clock::now()) { }

    void start () {
      from = std::chrono::high_resolution_clock::now();
    }

    double elapsed() const {
      return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - from).count() * 1.0e-6;
    }

  private:
    std::chrono::high_resolution_clock::time_point from;
};

// main
int main (int argc, char* argv[])
{
  double total;
  Timer timer;

  total = 0.0;
  timer.start();
  for (double i = 0.0; i < 1.0; i += 1e-8)
    total += std::pow (i,2);
  std::cout << "std::pow(i,2): " << timer.elapsed() << "s (result = " << total << ")\n";

  total = 0.0;
  timer.start();
  for (double i = 0.0; i < 1.0; i += 1e-8)
    total += i*i;
  std::cout << "i*i: " << timer.elapsed() << "s (result = " << total << ")\n";

  std::cout << "\n";

  total = 0.0;
  timer.start();
  for (double i = 0.0; i < 1.0; i += 1e-8)
    total += std::pow (i,3);
  std::cout << "std::pow(i,3): " << timer.elapsed() << "s (result = " << total << ")\n";

  total = 0.0;
  timer.start();
  for (double i = 0.0; i < 1.0; i += 1e-8)
    total += i*i*i;
  std::cout << "i*i*i: " << timer.elapsed() << "s (result = " << total << ")\n";


  return 0;
}