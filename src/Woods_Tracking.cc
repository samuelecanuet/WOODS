#include "Woods_Tracking.hh"

#include "G4Track.hh"
#include "G4TrackVector.hh"
#include "G4TrackingManager.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

Woods_Tracking::Woods_Tracking() : G4UserTrackingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

Woods_Tracking::~Woods_Tracking()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void Woods_Tracking::PreUserTrackingAction(const G4Track *)
{
}

void Woods_Tracking::PostUserTrackingAction(const G4Track *track)
{
    G4TrackVector *childrens = fpTrackingManager->GimmeSecondaries();
    if (track->GetParentID() == 0)
    {
        for (unsigned int index = 0; index < childrens->size(); ++index)
        {
            G4Track *tr = (*childrens)[index];
            tr->SetParentID(track->GetTrackID());
        }
    }
    else
    {
        for (unsigned int index = 0; index < childrens->size(); ++index)
        {
            G4Track *tr = (*childrens)[index];
            tr->SetParentID(track->GetParentID());
        }
    }
}
