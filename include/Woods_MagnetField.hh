#include "globals.hh"
#include "Woods_Global.hh"
#include "G4MagneticField.hh"
#include "G4ios.hh"

#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

class WoodsMagnetField
    // #ifndef STANDALONE
    : public G4MagneticField
// #endif
{

  int nz;
  double max, min;
  double xField[1001], yField[1001], zField[1001], zval[1001];

public:
  WoodsMagnetField(const string filename, G4double value);

  void GetFieldValue(const double Point[3],
                     double *Bfield) const;
};
