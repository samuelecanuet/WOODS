#include "Woods_RunManager.hh"
#include "Woods_Sensor.hh"

#include "G4UImanager.hh"

#include "TBranch.h"
#include "TH1D.h"
#include "TObjString.h"

#include <iostream>

//----------------------------------------------------------------------

// constructor
Woods_RunManager::Woods_RunManager()
{
  cout << "Constructor Woods_RunManager" << endl;

  ////////////// Construct all sensors //////////////////////////////
  cout << "Constructor Woods_Sensor" << endl;
  woods_sensor_PlasticScintillatorUp = new Woods_Sensor;
  woods_sensor_PlasticScintillatorLow = new Woods_Sensor;

  woods_sensor_CatcherMylar_central = new Woods_Sensor;
  woods_sensor_CatcherAl1_central = new Woods_Sensor;
  woods_sensor_CatcherAl2_central = new Woods_Sensor;
  woods_sensor_CatcherMylar_side = new Woods_Sensor;
  woods_sensor_CatcherAl1_side = new Woods_Sensor;
  woods_sensor_CatcherAl2_side = new Woods_Sensor;
  ///////////////////////////////////////////////////////////////////
}

// destructor
Woods_RunManager::~Woods_RunManager()
{
  cout << "Destructor Woods_RunManager" << endl;

  f->Close();
  delete f;

  cout << "Destructor Woods_Sensor" << endl;
  delete woods_sensor_PlasticScintillatorLow;
  delete woods_sensor_PlasticScintillatorUp;

  delete woods_sensor_CatcherMylar_central;
  delete woods_sensor_CatcherAl1_central;
  delete woods_sensor_CatcherAl2_central;
  delete woods_sensor_CatcherMylar_side;
  delete woods_sensor_CatcherAl1_side;
  delete woods_sensor_CatcherAl2_side;

  CloseInput();
}

//----------------------------------------------------------------------
void Woods_RunManager::AnalyzeEvent(G4Event *event)
{
  if (event->GetEventID() == 0)
  {
    f = new TFile(GetOutputFilename(), "recreate");
    ////////////// Construct Log /////////////////////////////////////
    TDirectory *dir = f->mkdir("Log");
    dir->cd();
    G4UImanager *uiManager = G4UImanager::GetUIpointer();
    for (int i = 0; i < uiManager->GetNumberOfHistory(); i++)
    {
      string command = uiManager->GetPreviousCommand(i).substr(1);
      if (command.find("run") != 0 && command.find("event") != 0 && command.find("tracking") != 0 && command.find("input") != 0 && command.find("output") != 0)
      {
        char const *num_char = command.c_str();
        dir->WriteObject(&command, num_char);
      }
      f->cd();
    }
    //////////////////////////////////////////////////////////////////

    ////////////// Construct the TREE ////////////////////////////////
    Tree = new TTree("Tree", "Informations");
    Tree->Branch("Event_Number", &Event_Number, "Event_Number/I");
    Tree->Branch("Particle_PDG", &Particle_PDG, "Particle_PDG/I");
    Tree->Branch("x", &x, "x/D");
    Tree->Branch("y", &y, "y/D");
    Tree->Branch("z", &z, "z/D");
    Tree->Branch("px", &px, "px/D");
    Tree->Branch("py", &py, "py/D");
    Tree->Branch("pz", &pz, "pz/D");
    Tree->Branch("Initial_Kinetic_Energy", &Initial_Kinetic_Energy, "Initial_Kinetic_Energy/D");
    Tree->Branch("Catcher_Central_Deposit_Energy", &Catcher_Central_Deposit_Energy, "Catcher_Central_Deposit_Energy/D");
    Tree->Branch("Catcher_Side_Deposit_Energy", &Catcher_Side_Deposit_Energy, "Catcher_Side_Deposit_Energy/D");
    Tree->Branch("PlasticScintillatorUp_Deposit_Energy", &PlasticScintillatorUp_Deposit_Energy, "PlasticScintillatorUp_Deposit_Energy/D");
    Tree->Branch("PlasticScintillatorUp_Hit_Position_x", &PlasticScintillatorUp_Hit_Position_x, "PlasticScintillatorUp_Hit_Position_x/D");
    Tree->Branch("PlasticScintillatorUp_Hit_Position_y", &PlasticScintillatorUp_Hit_Position_y, "PlasticScintillatorUp_Hit_Position_y/D");
    Tree->Branch("PlasticScintillatorUp_Hit_Position_z", &PlasticScintillatorUp_Hit_Position_z, "PlasticScintillatorUp_Hit_Position_z/D");
    Tree->Branch("PlasticScintillatorUp_Hit_Angle", &PlasticScintillatorUp_Hit_Angle, "PlasticScintillatorUp_Hit_Angle/D");
    Tree->Branch("PlasticScintillatorLow_Deposit_Energy", &PlasticScintillatorLow_Deposit_Energy, "PlasticScintillatorLow_Deposit_Energy/D");
    Tree->Branch("PlasticScintillatorLow_Hit_Position_x", &PlasticScintillatorLow_Hit_Position_x, "PlasticScintillatorLow_Hit_Position_x/D");
    Tree->Branch("PlasticScintillatorLow_Hit_Position_y", &PlasticScintillatorLow_Hit_Position_y, "PlasticScintillatorLow_Hit_Position_y/D");
    Tree->Branch("PlasticScintillatorLow_Hit_Position_z", &PlasticScintillatorLow_Hit_Position_z, "PlasticScintillatorLow_Hit_Position_z/D");
    Tree->Branch("PlasticScintillatorLow_Hit_Angle", &PlasticScintillatorLow_Hit_Angle, "PlasticScintillatorLow_Hit_Angle/D");
    ///////////////////////////////////////////////////////////////////
  }
  // call the base class function (whatever it is supposed to do)
  G4RunManager::AnalyzeEvent(event);

  G4PrimaryVertex *PrimaryVertex = event->GetPrimaryVertex();
  if (PrimaryVertex != 0)
  {
    count++;
    for (int part = 1; part <= event->GetNumberOfPrimaryVertex(); part++)
    {
      
      G4PrimaryParticle *Primary = event->GetPrimaryVertex(part - 1)->GetPrimary();
      G4ThreeVector Momentum = Primary->GetMomentumDirection();

      Event_Number = event->GetEventID();
      Particle_PDG = Primary->GetG4code()->GetPDGEncoding();
      x = PrimaryVertex->GetX0() / um;
      y = PrimaryVertex->GetY0() / um;
      z = PrimaryVertex->GetZ0() / nm; 
      px = Momentum.x();
      py = Momentum.y();
      pz = Momentum.z();
      Initial_Kinetic_Energy = Primary->GetKineticEnergy() / keV;
      Catcher_Central_Deposit_Energy = woods_sensor_CatcherMylar_central->GetDictionnary()[part].DepositEnergy + woods_sensor_CatcherAl1_central->GetDictionnary()[part].DepositEnergy + woods_sensor_CatcherAl2_central->GetDictionnary()[part].DepositEnergy;
      Catcher_Side_Deposit_Energy = woods_sensor_CatcherMylar_side->GetDictionnary()[part].DepositEnergy + woods_sensor_CatcherAl1_side->GetDictionnary()[part].DepositEnergy + woods_sensor_CatcherAl2_side->GetDictionnary()[part].DepositEnergy;

      PrimaryInfo PlasticScintillatorUp = woods_sensor_PlasticScintillatorUp->GetDictionnary()[part];
      PlasticScintillatorUp_Deposit_Energy = PlasticScintillatorUp.DepositEnergy;
      PlasticScintillatorUp_Hit_Position_x = PlasticScintillatorUp.HitPosition.x();
      PlasticScintillatorUp_Hit_Position_y = PlasticScintillatorUp.HitPosition.y();
      PlasticScintillatorUp_Hit_Position_z = PlasticScintillatorUp.HitPosition.z();
      PlasticScintillatorUp_Hit_Angle = PlasticScintillatorUp.HitAngle;

      PrimaryInfo PlasticScintillatorLow = woods_sensor_PlasticScintillatorLow->GetDictionnary()[part];
      PlasticScintillatorLow_Deposit_Energy = PlasticScintillatorLow.DepositEnergy;
      PlasticScintillatorLow_Hit_Position_x = PlasticScintillatorLow.HitPosition.x();
      PlasticScintillatorLow_Hit_Position_y = PlasticScintillatorLow.HitPosition.y();
      PlasticScintillatorLow_Hit_Position_z = PlasticScintillatorLow.HitPosition.z();
      PlasticScintillatorLow_Hit_Angle = PlasticScintillatorLow.HitAngle;
      Tree->Fill();
    }
    
    
    ///// Reset all variables of detectors pour the next primary particle /////////////////
    Particle_PDG = 0;
    x = 0;
    y = 0;
    z = 0;
    px = 0;
    py = 0;
    pz = 0;
    Initial_Kinetic_Energy = 0;
    Catcher_Central_Deposit_Energy = 0;
    Catcher_Side_Deposit_Energy = 0;
    PlasticScintillatorUp_Deposit_Energy = 0;
    PlasticScintillatorUp_Hit_Position_x = 0;
    PlasticScintillatorUp_Hit_Position_y = 0;
    PlasticScintillatorUp_Hit_Position_z = 0;
    PlasticScintillatorUp_Hit_Angle = 0;
    PlasticScintillatorLow_Deposit_Energy = 0;
    PlasticScintillatorLow_Hit_Position_x = 0;
    PlasticScintillatorLow_Hit_Position_y = 0;
    PlasticScintillatorLow_Hit_Position_z = 0;
    PlasticScintillatorLow_Hit_Angle = 0;
    ////////////////////////////////////////////////////////
  }

  int divi = 10000;

  ///// Writing all trees ///////////////////////////////////////////
  if (count % divi == 0)
  {
    Tree->AutoSave("FlushBaskets");
  }

  ///// Reset all dictionnaries of detectors ///////////////////////
  woods_sensor_PlasticScintillatorLow->ResetDictionnary();
  woods_sensor_PlasticScintillatorUp->ResetDictionnary();
  woods_sensor_CatcherMylar_central->ResetDictionnary();
  woods_sensor_CatcherAl1_central->ResetDictionnary();
  woods_sensor_CatcherAl2_central->ResetDictionnary();
  woods_sensor_CatcherMylar_side->ResetDictionnary();
  woods_sensor_CatcherAl1_side->ResetDictionnary();
  woods_sensor_CatcherAl2_side->ResetDictionnary();
  ///////////////////////////////////////////////////////////////////
}

int Woods_RunManager::OpenInput(const std::string &fname) {
    int error = 0; // Valeur de retour

    if (fname != "") {
        // Fermer la sortie précédente au cas où
        CloseInput();

        // Vérifier l'extension du fichier
        size_t dotPosition = fname.find_last_of('.');
        if (dotPosition != std::string::npos) {
            std::string extension = fname.substr(dotPosition + 1);
            input_name = fname;
            if (extension == "root") {
                TFile * file = new TFile(fname.c_str(), "READ");
                if (file->IsOpen()){std::cout << "<I> Open input file : " << fname << std::endl; file->Close();}
                 else {std::cerr <<"<W> Unable to open CRADLE input file" << std::endl; error = 2;}
            } else if (extension == "txt") {
                input_txt.open(fname.c_str());
                if (!input_txt.fail()){std::cout << "<I> Open input file : " << fname << std::endl;}
                else {std::cerr <<"<W> Unable to open CRADLE input file" << std::endl; error = 2;}
            } else {
                error = 3;
                std::cerr << "<W> Unrecognized input file extension : " << extension << std::endl;
            }
        } else {
            error = 4; // Aucune extension de fichier trouvée
            std::cerr << "<W> No file extension found in input file name : " << fname << std::endl;
        }
    } else {
        error = 1; // Nom de fichier vide
        std::cerr << "<W> Empty input file name" << std::endl;
    }

    return error;
}

int Woods_RunManager::OpenInputSRIM(const string &fname)
{
  int error = 0; // return value

  if (fname != "")
  {
    // close previous output... just in case
    CloseInputSRIM();

    // try to open the new file
    inputSRIM.open(fname.c_str());

    if (inputSRIM.fail())
    {
      error = 2;
      cerr << "<W> OpenInput : error opening file " << fname << endl;
    }
    else
    {
      input_nameSRIM = fname;
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