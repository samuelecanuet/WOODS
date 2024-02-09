#include "Woods_MagnetField.hh"

WoodsMagnetField::WoodsMagnetField(const string filename, G4double value)
{
  ifstream file(filename); // Open the file for reading.

  // Storage space for the table
  // table dimensions
  nz = 1001;
  max = 50. * cm, min = -50. * cm;

  // Ignore first blank line
  char buffer[256];
  file.getline(buffer, 256);

  // Read in the data
  double z, bx, by, bz;
  for (int iz = 0; iz < nz; iz++)
  {
    file >> z >> bz >> by >> bx;
    zField[iz] = bz / 6 * value;
    yField[iz] = by * tesla;
    xField[iz] = bx * tesla;
    zval[iz] = z * meter;
  }
  file.close();

  // for (int iz=0; iz<nz; iz++) {
  //     G4cout << xField[iz] << " "  << yField[iz] << " "  << zField[iz] << endl;
  //}
}

void WoodsMagnetField::GetFieldValue(const double point[4],
                                      double *Bfield) const
{
  //  double x = point[0];
  //  double y = point[1];
  double z = point[2];

  // Check that the point is within the defined region
  if (z < min)
  {
    Bfield[0] = 0.0;
    Bfield[1] = 0.0;
    Bfield[2] = 0.0;
  }
  else if (z >= min && z <= max)
  {
    // find two grid point around point searched for
    for (int ix = 1; ix < nz; ix++)
    {
      if (zval[ix] > z)
      {
        Bfield[0] = 0.0;
        Bfield[1] = 0.0;
        Bfield[2] = (zField[ix] - zField[ix - 1]) / (zval[ix] - zval[ix - 1]) * (z - zval[ix - 1]) + zField[ix - 1];
        break;
      }
    }
  }
  else
  {
    Bfield[0] = 0.0;
    Bfield[1] = 0.0;
    Bfield[2] = 0.0;
  }

  /* for testing...
      Bfield[0] = 0.0;
      Bfield[1] = 0.0;
      Bfield[2] = 0.5*tesla;
  */
}
