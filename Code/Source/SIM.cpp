#include "Classes/Lattice/Open1DLattice.cpp"
#include "Classes/Lattice/Periodic1DLattice.cpp"
#include "Classes/Lattice/Periodic2DLattice.cpp"

#include "Classes/Algorithm/SSEAlgorithm.cpp"
#include "Classes/Algorithm/EDAlgorithm.cpp"
#include "Classes/Algorithm/ISINGAlgorithm.cpp"

#include <cstdlib>
#include <iostream>

int main(int argc, char *argv[]) {

  try {

    if(argc != 8) throw "[SIM] Error: Please specify 7 parameters (Size, Lattice-Index, Measure-Count, Algorithm-Index, Start-Temperature, End-Temperature, Temperature-Step)\n              - Size can be any positive integer\n              - Lattice-Index:\n                 0: Open-1D\n                 1: Periodic-1D\n                 2: Periodic-2D\n              - Measure-Count should be a positive long integer - ED of course needs no Measure-Count (=0)\n              - Algorithm-Index:\n                 0: ED  - Exact Diagonalization\n                 1: ISING  - Classical Ising simulation\n                 2: SSE - Stochastic Series Expansion\n              - Temperatures should be positive floats (use the dot as delimiter)";

    int size                = atoi(argv[1]);
    int latticeIndex        = atoi(argv[2]);
    long measureCount       = atoi(argv[3]);
    int algorithmIndex      = atoi(argv[4]);
    double startTemperature = atof(argv[5]);
    double endTemperature   = atof(argv[6]);
    double temperatureStep  = atof(argv[7]);

    if(size <= 0) throw "[SIM] Error: The Size has to be positive";
    
    AbstractLattice* lattice;
    const char* latticeLabel;
    
    switch(latticeIndex) {
    
      case 0:
        lattice = new Open1DLattice(size);
        latticeLabel = "Open-1D";
        break;
    
      case 1:
        lattice = new Periodic1DLattice(size);
        latticeLabel = "Periodic-1D";
        break;
    
      case 2:
        lattice = new Periodic2DLattice(size);
        latticeLabel = "Periodic-2D";
        break;
    
      default:
        throw "[SIM] Error: The Lattice-Index has to be within [0;2]";
    
    }
    
    if(measureCount <= 0) throw "[SIM] Error: The Measure-Count has to be positive";
    
    AbstractAlgorithm* algorithm;
    const char* algorithmLabel;

    switch(algorithmIndex) {
    
      case 0:
        algorithm = new EDAlgorithm(lattice, measureCount);
        algorithmLabel = "ED";
        break;
    
      case 1:
        algorithm = new ISINGAlgorithm(lattice, measureCount);
        algorithmLabel = "ISING";
        break;
    
      case 2:
        algorithm = new SSEAlgorithm(lattice, measureCount);
        algorithmLabel = "SSE";
        break;
    
      default:
        throw "[SIM] Error: The Algorithm-Index has to be within [0;2]";
    
    }
    
    if(startTemperature <  0) throw "[SIM] Error: The Start-Temperature has to be positive or zero";
    if(endTemperature   <= 0) throw "[SIM] Error: The End-Temperature has to be positive";
    if(temperatureStep  <= 0) throw "[SIM] Error: The Temperature-Step has to be positive";
    if(startTemperature > endTemperature) std::swap(startTemperature, endTemperature);
    
    printf("#\n");
    printf("# SIM - DATA\n");
    printf("#\n");
    printf("# ----------------------------------------\n");
    printf("#\n");
    printf("# Size                 = %+29.29i\n", size);
    printf("# Lattice              = %s\n", latticeLabel);
    printf("# Measure-Count        = %+29.29li\n", measureCount);
    printf("# Algorithm            = %s\n", algorithmLabel);
    printf("# Start-Temperature    = %+30.23e\n", startTemperature);
    printf("# End-Temperature      = %+30.23e\n", endTemperature);
    printf("# Temperature-Step     = %+30.23e\n", temperatureStep);
    printf("#\n");
    printf("# %-27s | %-28s | %-28s | %-28s | %-28s | %-28s | %-28s | %-28s | %-28s | %-28s | %-28s | %-28s | %-28s | %-28s | %-28s | %-28s | %-28s | %-28s | %-28s | %-28s | %-28s | %-28s | %-28s\n",
           "Size",
           "Lattice-Index",
           "Measure-Count",
           "Algorithm-Index",
           "Temperature",
           "Average Energy",
           "Error Of Energy",
           "ACorTime Of Energy",
           "Average Heat Capacity",
           "Error Of Heat Capacity",
           "ACorTime Of Heat Capacity",
           "Average Magnetisation",
           "Error Of Magnetisation",
           "ACorTime Of Magnetisation",
           "Average Susceptibility",
           "Error Of Susceptibility",
           "ACorTime Of Susceptibility",
           "Average A Magnetisation",
           "Error Of A Magnetisation",
           "ACorTime Of A Magnetisation",
           "Average A Susceptibility",
           "Error Of A Susceptibility",
           "ACorTime Of A Susceptibility");

    for(double temperature = endTemperature; temperature > startTemperature + (temperatureStep / 2); temperature -= temperatureStep) {
    
      algorithm->runTemperatureRound(temperature);
      
      printf("%+29.29i|%+29.29i|%+29.29li|%+29.29i|%+30.23e|%+30.23e|%+30.23e|%+29.29li|%+30.23e|%+30.23e|%+29.29li|%+30.23e|%+30.23e|%+29.29li|%+30.23e|%+30.23e|%+29.29li|%+30.23e|%+30.23e|%+29.29li|%+30.23e|%+30.23e|%+29.29li\n",
             size,
             latticeIndex,
             measureCount,
             algorithmIndex,
             temperature,
             algorithm->getAverageEnergy(),
             algorithm->getErrorOfEnergy(),
             algorithm->getAutoCorrelationTimeOfEnergy(),
             algorithm->getAverageHeatCapacity(),
             algorithm->getErrorOfHeatCapacity(),
             algorithm->getAutoCorrelationTimeOfHeatCapacity(),
             algorithm->getAverageMagnetisation(),
             algorithm->getErrorOfMagnetisation(),
             algorithm->getAutoCorrelationTimeOfMagnetisation(),
             algorithm->getAverageSusceptibility(),
             algorithm->getErrorOfSusceptibility(),
             algorithm->getAutoCorrelationTimeOfSusceptibility(),
             algorithm->getAverageAbsoluteMagnetisation(),
             algorithm->getErrorOfAbsoluteMagnetisation(),
             algorithm->getAutoCorrelationTimeOfAbsoluteMagnetisation(),
             algorithm->getAverageAbsoluteSusceptibility(),
             algorithm->getErrorOfAbsoluteSusceptibility(),
             algorithm->getAutoCorrelationTimeOfAbsoluteSusceptibility());
      
      std::cerr << "[SIM] Info: Finished, Size=" << size << ", Lattice=" << latticeLabel << ", Measure-Count=" << measureCount << ", Algorithm=" << algorithmLabel << ", Temperature=" << temperature << std::endl;

    }
    
    delete algorithm;
    delete lattice;

    return EXIT_SUCCESS;
  
  } catch(const char *message) {
  
    std::cerr << message << std::endl;
    return EXIT_FAILURE;
  
  }

};
