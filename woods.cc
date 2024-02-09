// declaration of the classes used in the main function
#include "Woods_RunManager.hh"
#include "Woods_Detector.hh"
#include "Woods_PhysList.hh"
#include "Woods_Generator.hh"
#include "Woods_Messenger.hh"

// include files for interactive sessions and for display
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

//----------------------------------------------------------------------
// program main function

int main(int argc, char **argv)
{
  system("Logo/logo.ans");
  G4Random::setTheSeed(123456789);

  // get run time
  clock_t t1, t2;
  t1 = clock();

  //------------------------------------------------------------
  //  Simulation customisation
  // define the core simulation object
  Woods_RunManager run;

  // create the detector definition
  Woods_Detector *ptr_det = new Woods_Detector(&run);
  run.SetUserInitialization(ptr_det);

  // create the physics list
  Woods_PhysList *ptr_phys = new Woods_PhysList;
  run.SetUserInitialization(ptr_phys);

  Woods_Tracking *ptr_track = new Woods_Tracking();
  run.SetUserAction(ptr_track);

  // create the generator for events
  Woods_Generator *ptr_gene = new Woods_Generator(&run);
  run.SetUserAction(ptr_gene);

  new Woods_Messenger(&run, ptr_det, ptr_gene);


  run.Initialize();

  //------------------------------------------------------------
  //  Session start

  // the process must be added after initialization (so that particles
  // are defined)

  ptr_phys->AddStepMax(1 * mm, 0x2);

  // set the details of Geant4 messages
  G4EventManager::GetEventManager()->SetVerboseLevel(0);
  G4EventManager::GetEventManager()->GetTrackingManager()->SetVerboseLevel(0);

  // visualisation manager (required for displays)
  G4VisManager *visu_manager = new G4VisExecutive;
  visu_manager->SetVerboseLevel(0);
  visu_manager->Initialize();

  // get the command interpreter pointer
  G4UImanager *UI = G4UImanager::GetUIpointer();

  // execution of a macro given as command-line argument
  if (argc > 1)
  {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    G4cout << G4endl << "Reading macro file: " << fileName << G4endl;
    UI->ApplyCommand(command + fileName);
  }
  else
  // start of an interactive session
  {
    G4cout << "Starting a tcsh interactive session" << G4endl;
    G4UIExecutive *session = new G4UIExecutive(argc, argv);

    // start default viewer from macro
    UI->ApplyCommand("/control/execute visu.mac");
    G4cout << "--" << G4endl
           << "Use the following command for a display test" << G4endl
           << "->        /control/execute visu.mac" << G4endl
           << "--" << G4endl;

    // start the interactive session
    session->SessionStart();

    //  	G4cout << "End of interactive session" << G4endl << G4endl;
    delete session;
  }

  //------------------------------------------------------------

  // analysisManager->CloseFile();
  // get run tiime
  t2 = clock();
  float diff((float)t2 - (float)t1);
  cout << " " << endl;
  cout << "<I>: Run time: " << diff / 1.e6 << " s" << endl;
  cout << " " << endl;

  delete visu_manager;

  return (0);
}
