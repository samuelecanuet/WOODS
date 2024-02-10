#ifndef WITCH_RUNMANAGER_HH
#define WITCH_RUNMANAGER_HH

#include "Woods_Global.hh"
#include "G4RunManager.hh"
#include "Woods_Sensor.hh"
#include "G4AnalysisManager.hh"

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TTreeReader.h"
#include <TDirectory.h>

#include <unordered_map>
#include <utility>
#include <vector>

//----------------------------------------------------------------------
// This class defines the simulation core (from the base class G4RunManager)
// including information to get the required information
class Woods_RunManager : public G4RunManager
{

  //------------------------------------------------------------
  // internal variables definition
protected:
  Woods_Sensor *woods_sensor_PlasticScintillatorLow;
  Woods_Sensor *woods_sensor_PlasticScintillatorUp;
  Woods_Sensor *woods_sensor_CatcherMylar_central;
  Woods_Sensor *woods_sensor_CatcherAl1_central;
  Woods_Sensor *woods_sensor_CatcherAl2_central;
  Woods_Sensor *woods_sensor_CatcherMylar_side;
  Woods_Sensor *woods_sensor_CatcherAl1_side;
  Woods_Sensor *woods_sensor_CatcherAl2_side;

  G4double x, y, z, px, py, pz, Initial_Kinetic_Energy, Catcher_Central_Deposit_Energy, Catcher_Side_Deposit_Energy, PlasticScintillatorUp_Deposit_Energy, PlasticScintillatorUp_Hit_Position_x, PlasticScintillatorUp_Hit_Position_y, PlasticScintillatorUp_Hit_Position_z, PlasticScintillatorUp_Hit_Angle, PlasticScintillatorLow_Deposit_Energy, PlasticScintillatorLow_Hit_Position_x, PlasticScintillatorLow_Hit_Position_y, PlasticScintillatorLow_Hit_Position_z, PlasticScintillatorLow_Hit_Angle;
  G4int PlasticScintillatorUp_BackScattering_Number, PlasticScintillatorLow_BackScattering_Number, PlasticScintillatorUp_AnnihilationCounter, PlasticScintillatorLow_AnnihilationCounter;
  vector<G4double> PlasticScintillatorUp_Hit_KineticEnergy, PlasticScintillatorLow_Hit_KineticEnergy;
  G4int Particle_PDG;
  std::vector<std::string> Silicon_Detector_Name;
  std::vector<int> Silicon_Detector_Code;
  G4int Event_Number;
  G4int count = 0;

  ifstream input_txt;
  string input_name;

  ifstream inputSRIM;
  string input_nameSRIM;

  G4double threshoold;
  G4String filename;

  //------------------------------------------------------------
  // class functions definition
public:
  // constructor and destructor
  Woods_RunManager();
  ~Woods_RunManager();

  TFile *f;

  TTree *Tree;     

  Woods_Sensor *GetWoodsSensor_PlasticScintillatorUp();
  Woods_Sensor *GetWoodsSensor_PlasticScintillatorLow();
  Woods_Sensor *GetWoodsSensor_CatcherMylar_central();
  Woods_Sensor *GetWoodsSensor_CatcherAl1_central();
  Woods_Sensor *GetWoodsSensor_CatcherAl2_central();
  Woods_Sensor *GetWoodsSensor_CatcherMylar_side();
  Woods_Sensor *GetWoodsSensor_CatcherAl1_side();
  Woods_Sensor *GetWoodsSensor_CatcherAl2_side();

  //----------------------------------------------------------
  // Commands definitions
  ////--------------------------------------------------
  //// U.I. commands creation function
  // void  DefineSimulationCommands ( );     // inline
  //----------------------------------------------------------
  // Functions for input file
  ////--------------------------------------------------
  //// added input file functions
  ////                  declaration
  int OpenInput(const string &fname);
  void CloseInput();                  // inline
  ifstream &GetInput_TXT();               // inline
  const string &GetInputName() const; // inline

  int OpenInputSRIM(const string &fname);
  void CloseInputSRIM();    // inline
  ifstream &GetInputSRIM(); // inline

  void SetThreshoold(G4double th);
  G4double GetThreshoold();

  void SetOutputFilename(G4String fn);
  G4String GetOutputFilename();

  //----------------------------------------------------------
  // Functions for events processing and output histogram

  // redefine the function from base class G4RunManager
  void AnalyzeEvent(G4Event *event);

  ////////////// Construct Histograms //////////////////////////////
    TDirectory *dirUp;
    TDirectory *dirLow;
    TH1D *EnergyDepositUp;
    TH1D *EnergyDepositLow;
    TH1D *EnergyDepositUp_Beta;
    TH1D *EnergyDepositLow_Beta;
    TH1D *EnergyDepositUp_Gamma;
    TH1D *EnergyDepositLow_Gamma;
    TH1I * BackScatteringMultiplicityUp;
    TH1I * BackScatteringMultiplicityLow;
    TH1I * AnnihilationMultiplicityUp;
    TH1I * AnnihilationMultiplicityLow;
  //////////////////////////////////////////////////////////////////
};

//----------------------------------------------------------------------
// inline functions for the class


inline Woods_Sensor *Woods_RunManager::GetWoodsSensor_PlasticScintillatorUp() { return (woods_sensor_PlasticScintillatorUp); }
inline Woods_Sensor *Woods_RunManager::GetWoodsSensor_PlasticScintillatorLow() { return (woods_sensor_PlasticScintillatorLow); }
inline Woods_Sensor *Woods_RunManager::GetWoodsSensor_CatcherMylar_central() { return (woods_sensor_CatcherMylar_central); }
inline Woods_Sensor *Woods_RunManager::GetWoodsSensor_CatcherAl1_central() { return (woods_sensor_CatcherAl1_central); }
inline Woods_Sensor *Woods_RunManager::GetWoodsSensor_CatcherAl2_central() { return (woods_sensor_CatcherAl2_central); }
inline Woods_Sensor *Woods_RunManager::GetWoodsSensor_CatcherMylar_side() { return (woods_sensor_CatcherMylar_side); }
inline Woods_Sensor *Woods_RunManager::GetWoodsSensor_CatcherAl1_side() { return (woods_sensor_CatcherAl1_side); }
inline Woods_Sensor *Woods_RunManager::GetWoodsSensor_CatcherAl2_side() { return (woods_sensor_CatcherAl2_side); }

// Close the input file
inline void Woods_RunManager::CloseInput()
{
  input_txt.close();
  input_name = "";
}

// Get the input file stream information
inline ifstream &Woods_RunManager::GetInput_TXT()
{
  return (input_txt);
}

// Get the input file name
inline const string &Woods_RunManager::GetInputName() const
{
  return (input_name);
}

// SRIM
inline void Woods_RunManager::CloseInputSRIM()
{
  inputSRIM.close();
  input_nameSRIM = "";
}
inline ifstream &Woods_RunManager::GetInputSRIM()
{
  return (inputSRIM);
}

// THRESHOOLD
inline void Woods_RunManager::SetThreshoold(G4double th) { threshoold = th; }
inline G4double Woods_RunManager::GetThreshoold() { return (threshoold); }

// OUPUT FILE
inline void Woods_RunManager::SetOutputFilename(G4String fn) { filename = fn; }
inline G4String Woods_RunManager::GetOutputFilename() { return filename; }
#endif
