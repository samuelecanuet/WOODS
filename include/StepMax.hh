//======================================================================
/*! \file StepMax.hh
 *
 *  Header file for the StepMax class.
 */
//======================================================================

#ifndef STEP_MAX_HH
#define STEP_MAX_HH

// simulation general include
#include "Woods_Global.hh"

// Geant4 includes
#include "globals.hh"
#include "G4VDiscreteProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4Step.hh"

//======================================================================
/*! \class StepMax
 *
 *  This class defines a process to set a maximum step length in
 *  the particles tracking in Geant4.
 *  Using such a maximum step gives better energy loss calculation
 *  along the particle tracks.
 *
 */
class StepMax : public G4VDiscreteProcess
{

private:
  G4double max_step; ///< Maximum step length

public:
  // constructor / destructor
  StepMax(const G4double step = 1 * km,
          const G4String &name = "UserMaxStep");
  StepMax(const StepMax &original);
  ~StepMax();

  G4bool IsApplicable(const G4ParticleDefinition &part);

  void SetMaxStep(G4double);
  G4double GetMaxStep() const; // inline

  G4double PostStepGetPhysicalInteractionLength(const G4Track &track, G4double,
                                                G4ForceCondition *condition);

  G4VParticleChange *PostStepDoIt(const G4Track &track, const G4Step &);

  // inline functions
  G4double GetMeanFreePath(const G4Track &, G4double, G4ForceCondition *);
};

//----------------------------------------------------------------------
//  Inline functions

/*! Returns the value of the maximum step length.*/
inline G4double StepMax::GetMaxStep() const
{
  return (max_step);
}

/*! Returns the mean free path for the process (no meaning in this process).*/
inline G4double StepMax::GetMeanFreePath(const G4Track &, G4double, G4ForceCondition *)
{
  return DBL_MAX;
}

//======================================================================

#endif
