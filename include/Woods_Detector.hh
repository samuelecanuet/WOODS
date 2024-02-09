#ifndef GE_DETECTOR_HH
#define GE_DETECTOR_HH

#include "Woods_Global.hh"
#include "G4VUserDetectorConstruction.hh"
#include "Woods_RunManager.hh"
#include "G4ThreeVector.hh"
#include "G4UnionSolid.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4SubtractionSolid.hh"
#include "G4ExtrudedSolid.hh"
#include "G4MultiUnion.hh"
#include "G4VisAttributes.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4TransportationManager.hh"
#include "G4ChordFinder.hh"
#include "G4FieldManager.hh"
#include "G4Trap.hh"
#include "G4VSolid.hh"

#include <unordered_map>
#include <tuple>
#include <string>
#include <iostream>

class G4GenericMessenger;

//----------------------------------------------------------------------

class Woods_Detector : public G4VUserDetectorConstruction
{

  //------------------------------------------------------------
  // internal variables definition
protected:
  // pointer to the run manager
  Woods_RunManager *manager_ptr;
  G4GenericMessenger *fMessenger;
  // detectors
  double pos_detp1, pos_detp2, pos_dete;
  double radius1_detp, radius2_detp, radius1_dete, radius2_dete;
  double thickness_detp, thickness_dete;
  // catcher
  double pos_catcher, thickness_catcher, radius_catcher;

  //------------------------------------------------------------
  // class functions definition
public:
  // constructor with arguments
  Woods_Detector(Woods_RunManager *mgr);

  // destructor
  ~Woods_Detector();

  // setup construction (mandatory class function)
  G4VPhysicalVolume *Construct();


  void SetBFieldValue(G4double val);

  int OpenInputB(const string &fname);
  const string &GetInputNameB() const; // inline
  void CloseInputB();                  // inline
  ifstream inputB;
  string input_nameB;
  void SetCatcherPosition_theta(G4String position, G4double angle);
  void SetCatcherPosition_z(G4double catcher_z);
  void SetCatcher_Thickness(G4double Al1_e, G4double Mylar_e, G4double Al2_e);
  void SetDetector_Radius(G4double radius);
  void SetDetector_Thickness(G4double thickness);
  void SetDetector_Distance(G4double distance);
  G4Tubs *MylarSource;
  G4Tubs *AlSource1;
  G4Tubs *AlSource2;
  G4Tubs *MylarSource_central;
  G4Tubs *MylarSource_side;
  G4Tubs *Source;

  G4Tubs *fSolidWorld;
  G4LogicalVolume *fLogicWorld;
  G4VPhysicalVolume *fPhysiWorld;
  G4Material *fDefaultMaterial;

  G4double fLength_PlasticScintillator = 5 * cm;
  G4double fRadius_PlasticScintillator = 1.5 * cm;

  G4String Catcher_Position;

  G4bool event;

  G4Material *vide = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  std::vector<std::string> directions = {"Up", "Down"};
  G4double thicknessMylarSource;

  G4double thicknessMylarSource_central;
  G4double thicknessMylarSource_side;

  G4double thicknessAlSource;
  G4VPhysicalVolume *Physics_AlSource1;
  G4VPhysicalVolume *Physics_AlSource1_central;
  G4VPhysicalVolume *Physics_AlSource1_side;

  G4VPhysicalVolume *Physics_MylarSource;
  G4VPhysicalVolume *Physics_MylarSource_central;
  G4VPhysicalVolume *Physics_MylarSource_side;

  G4VPhysicalVolume *Physics_AlSource2;
  G4VPhysicalVolume *Physics_AlSource2_central;
  G4VPhysicalVolume *Physics_AlSource2_side;

  G4VPhysicalVolume *Physics_Source;


  G4VPhysicalVolume *phys_centralPEEK_ring;
  G4VPhysicalVolume *phys_sidePEEK_ring;

  G4ThreeVector Support_Position;
  G4ThreeVector Central_Hole_Position;
  G4ThreeVector Side_Hole_Position;
  G4ThreeVector Catcher_side_Position;
  G4ThreeVector Catcher_central_Position;
  G4ThreeVector Source_Position;
  G4double PEEK_thikness;
  G4double SuppCatcher_thikness = 2*mm;

  G4VPhysicalVolume *phys_Supp_catcher;
  G4VPhysicalVolume *phys_mother_catcher;
  G4double y_baseCatcher_Spessa_1;
  G4double y_baseCatcher_Sottile_1;
  G4double y_baseCatcher_2;
  G4double y_pos_Catcher_risp_munion;
  G4double thickness_baseCatcher_Spessa_1;
  G4double thickness_baseCatcher_Sottile_1;
  G4double thickness_supp_catcher;
  G4VPhysicalVolume *phys_munion_supportCatcher_Source;
  G4double x_pos_bucoCatcher;

  G4PVPlacement *fPhys_PlasticScintillatorUp;
  G4PVPlacement *fPhys_PlasticScintillatorLow;
  G4VPhysicalVolume *physics_placcaRame_CilindricaUp;
  G4VPhysicalVolume *physics_placcaRame_CilindricaLow;
  G4VPhysicalVolume *physics_Box_material_SupportoRame_PlasticScintUp;
  G4VPhysicalVolume *physics_Box_material_SupportoRame_PlasticScintLow;

  G4Tubs *holeForPlasticScint;
  G4VSolid *Box_material_SupportoRame_PlasticScint;
  G4VSolid *bigBox_material_SupportoRame_PlasticScint;
  G4double thickness_z_SupportoRame_PlasticScint = 1.5 * mm;


  G4Tubs *fSolid_PlasticScintillator;
  G4Tubs *placcaRame_Cilindrica;

  double theta;
};



inline int Woods_Detector::OpenInputB(const string &fname)
{
  int error = 0; // return value

  if (fname != "")
  {
    // close previous output... just in case
    CloseInputB();

    // try to open the new file
    inputB.open(fname.c_str());

    if (inputB.fail())
    {
      error = 2;
      cerr << "<W> OpenInput : error opening file " << fname << endl;
    }
    else
    {
      input_nameB = fname;
      cout << "<I> Open input file: " << fname << endl;
    }
  }
  else
  {
    error = 1;
    cerr << "<W> OpenInput : empty file name" << endl;
  }

  return (error);
}

// Get the input file name
inline const string &Woods_Detector::GetInputNameB() const
{
  return (input_nameB);
}

inline void Woods_Detector::CloseInputB()
{
  inputB.close();
  input_nameB = "";
}

#endif
