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
  Woods_Sensor *woods_sensor_PlasticScintillator;
  Woods_Sensor *woods_sensor_CatcherMylar_central;
  Woods_Sensor *woods_sensor_CatcherAl1_central;
  Woods_Sensor *woods_sensor_CatcherAl2_central;
  Woods_Sensor *woods_sensor_CatcherMylar_side;
  Woods_Sensor *woods_sensor_CatcherAl1_side;
  Woods_Sensor *woods_sensor_CatcherAl2_side;

  G4double x, y, z, px, py, pz, Initial_Kinetic_Energy, Catcher_Central_Deposit_Energy, Catcher_Side_Deposit_Energy, PlasticScintillator_Deposit_Energy, PlasticScintillator_Hit_Position_x, PlasticScintillator_Hit_Position_y, PlasticScintillator_Hit_Position_z, PlasticScintillator_Hit_Angle;
  std::vector<double> Silicon_Detector_Deposit_Energy, Silicon_Detector_Hit_Position_x, Silicon_Detector_Hit_Position_y, Silicon_Detector_Hit_Position_z, Silicon_Detector_Hit_Angle, Silicon_Detector_DL_Deposit_Energy;
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

  static const int nb_det = 40;

  TFile *f;

  TTree *Tree;

  TH1D *histos_coinc[nb_det];
  TH1D *histos_nocoinc[nb_det];
  TH1D *histos_single[nb_det];

  std::string Detector_Name[nb_det] = {
      "1Up_Strip_1", "1Up_Strip_2", "1Up_Strip_3", "1Up_Strip_4", "1Up_Strip_5",
      "2Up_Strip_1", "2Up_Strip_2", "2Up_Strip_3", "2Up_Strip_4", "2Up_Strip_5",
      "3Up_Strip_1", "3Up_Strip_2", "3Up_Strip_3", "3Up_Strip_4", "3Up_Strip_5",
      "4Up_Strip_1", "4Up_Strip_2", "4Up_Strip_3", "4Up_Strip_4", "4Up_Strip_5",
      "1Down_Strip_1", "1Down_Strip_2", "1Down_Strip_3", "1Down_Strip_4", "1Down_Strip_5",
      "2Down_Strip_1", "2Down_Strip_2", "2Down_Strip_3", "2Down_Strip_4", "2Down_Strip_5",
      "3Down_Strip_1", "3Down_Strip_2", "3Down_Strip_3", "3Down_Strip_4", "3Down_Strip_5",
      "4Down_Strip_1", "4Down_Strip_2", "4Down_Strip_3", "4Down_Strip_4", "4Down_Strip_5"};

  int Detector_Code[nb_det] = {
      11, 12, 13, 14, 15,
      21, 22, 23, 24, 25, 
      31, 32, 33, 34, 35,
      41, 42, 43, 44, 45,
      -11, -12, -13, -14, -15,
      -21, -22, -23, -24, -25,
      -31, -32, -33, -34, -35,
      -41, -42, -43, -44, -45};
      
  std::unordered_map<int, std::pair<Woods_Sensor *, Woods_Sensor *>> dic_detector;

  Woods_Sensor *GetWoodsSensor_PlasticScintillator();
  Woods_Sensor *GetWoodsSensor_Detector(string name);
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
};

//----------------------------------------------------------------------
// inline functions for the class

inline Woods_Sensor *Woods_RunManager::GetWoodsSensor_Detector(string name)
{

  if (name.substr(name.length() - 2) == "dl")
  {
    if (dic_detector.find( stoi( name.substr(0, name.length() - 3)) ) != dic_detector.end())
    {
      return (dic_detector[ stoi( name.substr(0, name.length() - 3))].second);
    }
    else
    {
      G4Exception("GetWoods_Sensor_GetDetector", ("Detector Name not found : " + name).c_str(), JustWarning, "");
      return nullptr;
    }
  }

  else
  {
    if (dic_detector.find( stoi( name)) != dic_detector.end())
    {
      return (dic_detector[ stoi( name)].first);
    }
    else
    {
      G4Exception("GetWoods_Sensor_GetDetector", ("Detector Name not found : " + name).c_str(), JustWarning, "");
      return nullptr;
    }
  }
}

inline Woods_Sensor *Woods_RunManager::GetWoodsSensor_PlasticScintillator() { return (woods_sensor_PlasticScintillator); }
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

// inline DATA &Woods_RunManager::GetInput_ROOT()
// {
  
//   root_file = new TFile(input_name.c_str(), "READ");

//   datatree.reader = TTreeReader("ParticleTree", root_file);;
//   datatree.event = TTreeReaderValue<int>(Reader, "event");
//   return datatree;
// }

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
