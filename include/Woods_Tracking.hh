#include "G4UserTrackingAction.hh"

class Woods_Tracking : public G4UserTrackingAction
{
public:
  Woods_Tracking();

  virtual ~Woods_Tracking();

  virtual void PreUserTrackingAction(const G4Track *);
  virtual void PostUserTrackingAction(const G4Track *);
};
