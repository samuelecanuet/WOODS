#ifndef WITCH_SENSOR_HH
#define WITCH_SENSOR_HH

#include "Woods_Global.hh"
#include "G4VSensitiveDetector.hh"
#include "G4EventManager.hh"

struct PrimaryInfo
{
  G4double DepositEnergy;
  G4double HitAngle;
  G4ThreeVector HitPosition;
  G4String ParticleName;
  G4int BackScatteringNumber;
  vector<G4double> HitKineticEnergy;
  G4int AnnihilationCounter;
};

//----------------------------------------------------------------------
// This class is associated with a volume of the geometry to make this
// volume "sensitive"
// When a particle track produces a "hit" in the corresponding volume,
// the ProcessHits function is called
class Woods_Sensor : public G4VSensitiveDetector
{

  //------------------------------------------------------------
  // internal variables definition
protected:

  //------------------------------------------------------------
  // class functions definition
public:
  // constructor and destructor
  Woods_Sensor();
  ~Woods_Sensor();

  // hits processing
  void Initialize(G4HCofThisEvent *);
  G4bool ProcessHits(G4Step *, G4TouchableHistory *);

  std::unordered_map<G4int, PrimaryInfo> PrimaryDictionnary;

  void ResetDictionnary() { PrimaryDictionnary.clear(); }
  std::unordered_map<G4int, PrimaryInfo> GetDictionnary() { return PrimaryDictionnary; }

  inline void PrintDictionnary(PrimaryInfo dic);

  PrimaryInfo PrimaryInfo_init;

  int index;
  
};

void Woods_Sensor::PrintDictionnary(PrimaryInfo dic)
{
  G4cout<<G4endl;
  G4cout<<"Name : "<<dic.ParticleName << G4endl;
  G4cout<<setprecision(4)<<"Energy Deposit : "<< dic.DepositEnergy << " keV"<<G4endl;
  G4cout<<setprecision(4)<<"Hit Angle : "<< dic.HitAngle << " deg"<<G4endl;
  G4cout<<"Hit Position : "<< dic.HitPosition << "(mm)"<<G4endl;
  G4cout<<G4endl;
}
#endif
