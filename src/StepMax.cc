//======================================================================
/*! \file StepMax.cpp
 *
 *  Source file for the StepMax class.
 */
//======================================================================

#include "StepMax.hh"

//----------------------------------------------------------------------

/*! Constructor of the process.
 *  \param  step    maximum step value
 *  \param  name    process name
 */
StepMax::StepMax(const G4double step,
                 const G4String &name)
    : G4VDiscreteProcess(name)
{
  max_step = step;
}

/*! Copy constructor.
 *  \param  original    process to copy
 */
StepMax::StepMax(const StepMax &original)
    : G4VDiscreteProcess((StepMax &)original)
{
  cerr << "<W> StepMax: copy constructor should not be used" << endl;
}

/*! Destructor of the process.*/
StepMax::~StepMax()
{
}

//----------------------------------------------------------------------

/*! Returns true if the process can be applied to a particle.
 *  \param  part    particle definition
 */
G4bool StepMax::IsApplicable(const G4ParticleDefinition &part)
{
  return (part.GetPDGCharge() != 0. && !part.IsShortLived());
}

/*! Define a new value for the maximum step length.
 *  \param  step    maximum step value
 */
void StepMax::SetMaxStep(G4double step)
{
  max_step = step;
}

//----------------------------------------------------------------------

/*! Returns the interaction length.
 *  \param  track       current track
 *  \param  cond        force condition
 */
G4double StepMax::PostStepGetPhysicalInteractionLength(
    const G4Track &track,
    G4double,
    G4ForceCondition *cond)
{
  // condition is set to "Not Forced"
  *cond = NotForced;

  G4double proposed_step = DBL_MAX;

  if ((max_step > 0.) && (track.GetVolume() != NULL))
    proposed_step = max_step;

  return (proposed_step);
}

//----------------------------------------------------------------------
/*! Apply the process at post-step.
 *  \param  track     (not documented in Geant4)
 */
G4VParticleChange *StepMax::PostStepDoIt(const G4Track &track, const G4Step &)
{
  // do nothing
  aParticleChange.Initialize(track);
  return (&aParticleChange);
}

//----------------------------------------------------------------------
