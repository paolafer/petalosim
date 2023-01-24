// ----------------------------------------------------------------------------
// petalosim | PetXenonProperties.cc
//
// Some useful functions to calculate relevant physical properties of xenon.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PetXenonProperties.h"

#include "nexus/Interpolation.h"

#include <G4SystemOfUnits.hh>
#include <fstream>


std::pair<G4int, G4int> MakeLXeDensityDataTable(std::vector<std::vector<G4double>>& data)
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
  G4int npressures = 0;
  G4int ntemps = 0;
  G4int count = 0;
  G4double thistemp = 0;
  G4double temp, press, dens;
  char comma;

  while (inFile>>temp>>comma>>press>>comma>>dens){
    std::vector<G4double> thisdata {temp*kelvin, press*bar, dens*(kg/m3)};
    data.push_back(thisdata);

    // Figure out how many temperature and pressures we have
    if (temp == thistemp){
      npressures++;
    }
    else {
      thistemp = temp;
      ntemps++;
      npressures = 1;
    }
    count++;
  }

  npressures = npressures-1;

  inFile.close();

  std::pair<G4int, G4int> nkeys = std::make_pair(npressures, ntemps);

  return nkeys;
}


G4double GetLXeDensity(G4double temperature, G4double pressure)
{
  // Interpolate to calculate the density
  // at a given pressure and temperature
  G4double density = 5.324 * kg/m3;
  std::vector<std::vector<G4double>> data;
  std::pair<G4int, G4int> nkeys = MakeLXeDensityDataTable(data);
  G4int npressures = nkeys.first;
  G4int ntemps     = nkeys.second;

  // Find correct interval and use bilinear interpolation
  G4bool found = false;
  G4int tcount = 0;
  G4int pcount = 0;
  G4int count = 0;
  G4double t1, t2, p1, p2, d11, d12, d21, d22, d1, d2;

  while (tcount < ntemps-1) {
    t1 = data[count][0];
    t2 = data[count+npressures][0];

    if (temperature >= t1 && temperature < t2) {
      while (pcount < npressures-1) {
        p1 = data[count][1];
        p2 = data[count+1][1];

        if (pressure >= p1 && pressure < p2) {
          d11 = data[count][2];
          d12 = data[count+1][2];
          d21 = data[count+npressures][2];
          d22 = data[count+npressures+1][2];

          density = nexus::BilinearInterpolation(temperature, t1, t2,
            pressure, p1, p2,
            d11, d12, d21, d22);
            found = true;
            break;
        }
        pcount++;
        count++;
      }

      if (!found) {
        if (pressure == data[count][1]) {
          d1 = data[count][2];
          d2 = data[count+npressures][2];
          density = nexus::LinearInterpolation(temperature, t1, t2, d1, d2);
          found = true;
        } else {
          throw "Unknown xenon density for this pressure!";
        }
      }

      break;
    }
    tcount++;
    pcount = 0;
    count += npressures;
  }

  if (!found) {
    if (temperature == data[count][0]) {
      while (pcount< npressures-1) {
        p1 = data[count][1];
        p2 = data[count+1][1];

        if (pressure >= p1 && pressure < p2){
          d1 = data[count][2];
          d2 = data[count+1][2];

          density = nexus::LinearInterpolation(pressure, p1, p2, d1, d2);
          found = true;
          break;
        }
        pcount++;
        count++;
      }
      if (!found) {
        if (pressure == data[count][1]) {
          density = data[count][2];
          found = true;
        } else {
          throw "Unknown xenon density for this pressure!";
        }
      }
    } else {
        throw "Unknown xenon density for this temperature";
    }
  }
  return density;
}
