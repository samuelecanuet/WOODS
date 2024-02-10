#include "Woods_Sensor.hh"
#include "G4VProcess.hh"
#include "G4TrackingManager.hh"
#include "G4Event.hh"
#include <G4Field.hh>
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4LogicalVolume.hh"
#include "G4TouchableHandle.hh"
#include "G4Navigator.hh"
//----------------------------------------------------------------------

// constructor of the sensitive detector associated to the Silicon Detector
Woods_Sensor::Woods_Sensor()
    : G4VSensitiveDetector("WoodsSensor")
{
}

// destructor
Woods_Sensor::~Woods_Sensor()
{
}

//----------------------------------------------------------------------

// Function called at the beginning of the event processing
// (to initialise the list of "hits" associated to the detector)
void Woods_Sensor::Initialize(G4HCofThisEvent *)
{
  cerr << "Woods_Sensor Initialisation" << endl;

  PrimaryInfo_init.DepositEnergy = 0;
  PrimaryInfo_init.HitPosition = G4ThreeVector(0, 0, 0);
  PrimaryInfo_init.HitAngle = 0;
  PrimaryInfo_init.ParticleName = "Unknown";
  PrimaryInfo_init.BackScatteringNumber = 0;
  PrimaryInfo_init.HitKineticEnergy.clear();
  PrimaryInfo_init.AnnihilationCounter = 0;
}

// Function called when the track step occurs in the detector
G4bool Woods_Sensor::ProcessHits(G4Step *step, G4TouchableHistory *)
{
  G4Track *track = step->GetTrack();

  // Getting the primary track id
  if (track->GetParentID() == 0)
  {
    index = track->GetTrackID();
  }
  else
  {
    index = track->GetParentID();
  }


  //////Init dic
  if (PrimaryDictionnary.find(index) == PrimaryDictionnary.end())
  {

    PrimaryDictionnary[index] = PrimaryInfo_init;
    PrimaryDictionnary[index].ParticleName = G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->GetPrimaryVertex(index - 1)->GetPrimary()->GetG4code()->GetParticleName();
    if (track->GetParentID() == 0)
    {

      PrimaryDictionnary[index].HitPosition = step->GetPreStepPoint()->GetPosition() / mm;

      int copy_no = step->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo();
      if (copy_no == 98 || copy_no == 99)
      {
        PrimaryDictionnary[index].HitAngle = std::acos(G4ThreeVector(0, 0, 1) * track->GetMomentumDirection()) / deg;
      }
    }
  }

  
  ////Fill dic
  if (track->GetParentID() == 0 && track->GetTrackStatus() == fAlive && (step->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo() == 98 || step->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo() == 99))
  {
    if (step->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo() == -1)
    {
      PrimaryDictionnary[index].HitKineticEnergy.push_back(step->GetPostStepPoint()->GetKineticEnergy() / keV);
    }

    if (step->GetPostStepPoint()->GetPhysicalVolume()->GetCopyNo() == -1 )
    {
      PrimaryDictionnary[index].BackScatteringNumber++;
    }

    if (step->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessName() == "annil")
    {
      PrimaryDictionnary[index].AnnihilationCounter++;
    }
  }

  PrimaryDictionnary[index].DepositEnergy += step->GetTotalEnergyDeposit() / keV;
  return (true);
}
