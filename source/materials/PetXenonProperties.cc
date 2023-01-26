// ----------------------------------------------------------------------------
// petalosim | PetXenonProperties.cc
//
// Some useful functions to calculate relevant physical properties of xenon.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PetXenonProperties.h"

#include <G4SystemOfUnits.hh>
#include <fstream>


 G4int MakeLXeDensityDataTable(std::vector<std::vector<G4double>>& data)
{
  // Fills the data_ vector with temperature, pressure, and density data
  // Assumes the data file goes up in pressure then temperature
  // with the format: Temperature Pressure Density

  // Open file
  char* ps_path = std::getenv("PETALODIR");
  if (ps_path == nullptr) {
    G4Exception("[PetXenonProperties]", "MakeLXeDensityDataTable()",
                FatalException, "Environment variable PETALODIR not defined!");
  }

  G4String path(ps_path);
  G4String filename = path + "/data/lxe_density_data.txt";

  std::ifstream inFile;
  inFile.open(filename);
  if (!inFile) {
    throw "File could not be opened";
  }

  // Read lines in file
  G4String thisline;
  getline(inFile, thisline); // don't use first line
  G4int count = 0;
  G4double temp, press, dens;
  char comma;

  while (inFile>>temp>>comma>>press>>comma>>dens){
    std::vector<G4double> thisdata {temp*kelvin, press*bar, dens*(kg/m3)};
    data.push_back(thisdata);
    count++;
  }

  inFile.close();

  return count;
}


G4double GetLXeDensity(G4double pressure)
{
  // Interpolate to calculate the density at a given pressure.
  // The temperature is unique, given the pressure, and it follows the liquid-vapor
  // saturation curve of liquid xenon.
  G4double density = 5.324 * kg/m3;
  std::vector<std::vector<G4double>> data;
  G4int n_keys = MakeLXeDensityDataTable(data);

  // Use linear interpolation
  G4bool found = false;

  for (G4int i=0; i<n_keys-1; ++i) {
    if (pressure >= data[i][1] && pressure < data[i+1][1]) {
      G4double x1 = data[i][1];
      G4double x2 = data[i+1][1];
      G4double y1 = data[i][2];
      G4double y2 = data[i+1][2];
      density = y1 + (y2-y1)*(pressure-x1)/(x2-x1);
      found = true;
      break;
    }
  }

  if (!found) {
    if (pressure == data[n_keys-1][1]) {
      density = data[n_keys-1][2];
    }
    else {
      throw "Unknown xenon density for this pressure!";
    }
  }

  return density;
}
