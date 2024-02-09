#include <iostream>
#include <vector>
#include <unordered_map>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <TROOT.h>
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

using namespace std;



int main(int argc, char **argv)
{
    

    cout << argv[1] << endl;

    // Ouvrir le fichier ROOT
    TFile *file = new TFile(argv[1], "READ");
    if (!file->IsOpen())
    {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier ROOT du tree." << std::endl;
        return 1;
    }

    TFile *outfile = new TFile("test.root", "RECREATE");
    if (!outfile->IsOpen())
    {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier ROOT des histos." << std::endl;
        return 1;
    }

    /////////////////////////////////////////
    ///////////// VARIABLES /////////////////
    /////////////////////////////////////////
    int nb_det = 40;
    vector<string> Detector_Name = {
      "1Up_Strip_1", "1Up_Strip_2", "1Up_Strip_3", "1Up_Strip_4", "1Up_Strip_5",
      "2Up_Strip_1", "2Up_Strip_2", "2Up_Strip_3", "2Up_Strip_4", "2Up_Strip_5",
      "3Up_Strip_1", "3Up_Strip_2", "3Up_Strip_3", "3Up_Strip_4", "3Up_Strip_5",
      "4Up_Strip_1", "4Up_Strip_2", "4Up_Strip_3", "4Up_Strip_4", "4Up_Strip_5",
      "1Down_Strip_1", "1Down_Strip_2", "1Down_Strip_3", "1Down_Strip_4", "1Down_Strip_5",
      "2Down_Strip_1", "2Down_Strip_2", "2Down_Strip_3", "2Down_Strip_4", "2Down_Strip_5",
      "3Down_Strip_1", "3Down_Strip_2", "3Down_Strip_3", "3Down_Strip_4", "3Down_Strip_5",
      "4Down_Strip_1", "4Down_Strip_2", "4Down_Strip_3", "4Down_Strip_4", "4Down_Strip_5"};

  vector<int> Detector_Code = {
      11, 12, 13, 14, 15,
      21, 22, 23, 24, 25, 
      31, 32, 33, 34, 35,
      41, 42, 43, 44, 45,
      -11, -12, -13, -14, -15,
      -21, -22, -23, -24, -25,
      -31, -32, -33, -34, -35,
      -41, -42, -43, -44, -45};

    double threshoold_SiPMs;
    if (strstr(argv[1], "Pl") != nullptr)
    {
        threshoold_SiPMs = 100.;
    }
    else
    {
        threshoold_SiPMs = 100.;
    }
    cout << threshoold_SiPMs << endl;

    int counter;

    /////////////////////////////////////////
    /////////////  HISTOGRAMS  //////////////
    /////////////////////////////////////////

    // ##### SOURCE #####//
    //////////////////////
    // POSITION
    TH1D H1D_x = TH1D("h1d_x", "Initial x Position; x (um); Counts/10um", 400, -2000, 2000);
    TH1D H1D_y = TH1D("h1d_y", "Initial y Position; y (um); Counts/10um", 400, -2000, 2000);
    TH1D H1D_z = TH1D("h1d_z", "Initial z Position; z (nm); Counts/nm", 1000, 0, 1000);
    TH2D H2D_xy = TH2D("h2d_xy", "Initial xy Position; x (um); y (um)", 400, -2000, 2000, 400, -2000, 2000);
    // MOMENTUM
    TH1D H1D_px_proton = TH1D("h1d_px_proton", "Initial Proton Momentum Direction x; px; Counts", 200, -1, 1);
    TH1D H1D_py_proton = TH1D("h1d_py_proton", "Initial Proton Momentum Direction y; py; Counts", 200, -1, 1);
    TH1D H1D_pz_proton = TH1D("h1d_pz_proton", "Initial Proton Momentum Direction z; pz; Counts", 200, -1, 1);

    TH1D H1D_px_positron = TH1D("h1d_px_positron", "Initial Positron Momentum Direction x; px; Counts", 200, -1, 1);
    TH1D H1D_py_positron = TH1D("h1d_py_positron", "Initial Positron Momentum Direction y; py; Counts", 200, -1, 1);
    TH1D H1D_pz_positron = TH1D("h1d_pz_positron", "Initial Positron Momentum Direction z; pz; Counts", 200, -1, 1);
    // Kinetic Energy
    TH1D H1D_E0_proton = TH1D("h1d_E0_proton", "Initial Proton Kinetic Energy; E0 (keV); Counts/keV", 10000, 0, 10000);
    TH1D H1D_E0_positron = TH1D("h1d_E0_positron", "Initial Positron Kinetic Energy; E0 (keV); Counts/keV", 10000, 0, 10000);

    // #### DETECTORS ####//
    // Plastic Scintillator
    TH1D H1D_PL_Edep_proton = TH1D("h1d_PL_Edep_proton", "Plastic Scintillator Proton Energy Deposit ; Edep (keV); Counts/keV", 10000, 0, 10000);
    TH1D H1D_PL_x_proton = TH1D("h1d_PL_x_proton", "Plastic Scintillator Proton Hit x ; x (mm); Counts/0.1mm", 300, -15, 15);
    TH1D H1D_PL_y_proton = TH1D("h1d_PL_y_proton", "Plastic Scintillator Proton Hit y ; y (mm); Counts/0.1mm", 300, -15, 15);
    TH1D H1D_PL_z_proton = TH1D("h1d_PL_z_proton", "Plastic Scintillator Proton Hit z ; z (mm); Counts/0.1mm", 100, 0, 100);
    TH1D H1D_PL_Angle_proton = TH1D("h1d_PL_Angle_proton", "Plastic Scintillator Proton Hit Angle ; Angle (deg) ; Counts/deg", 180, 0, 180);

    TH1D H1D_PL_Edep_positron = TH1D("h1d_PL_Edep_positron", "Plastic Scintillator Positron Energy Deposit ; Edep (keV); Counts/keV", 10000, 0, 10000);
    TH1D H1D_PL_x_positron = TH1D("h1d_PL_x_positron", "Plastic Scintillator Positron Hit x ; x (mm); Counts/0.1mm", 300, -15, 15);
    TH1D H1D_PL_y_positron = TH1D("h1d_PL_y_positron", "Plastic Scintillator Positron Hit y ; y (mm); Counts/0.1mm", 300, -15, 15);
    TH1D H1D_PL_z_positron = TH1D("h1d_PL_z_positron", "Plastic Scintillator Positron Hit z ; z (mm); Counts/0.1mm", 300, 0, 100);
    TH1D H1D_PL_Angle_positron = TH1D("h1d_PL_Angle_positron", "Plastic Scintillator Positron Hit Angle ; Angle (deg) ; Counts/deg", 180, 0, 180);

    TH1D H1D_PL_Edep = TH1D("h1d_PL_Edep", "Plastic Scintillator Energy Deposit ; Edep (keV); Counts/keV", 10000, 0, 10000);
    TH1D H1D_PL_x = TH1D("h1d_PL_x", "Plastic Scintillator Hit x ; x (mm); Counts/mm", 300, -15, 15);
    TH1D H1D_PL_y = TH1D("h1d_PL_y", "Plastic Scintillator Hit y ; y (mm); Counts/mm", 300, -15, 15);
    TH1D H1D_PL_z = TH1D("h1d_PL_z", "Plastic Scintillator Hit z ; z (mm); Counts/mm", 300, -15, 15);
    TH1D H1D_PL_Angle = TH1D("h1d_PL_Angle", "Plastic Scintillator Hit Angle ; Angle (deg) ; Counts/deg", 180, 0, 180);

    TH2D H2D_PL_xy_positron = TH2D("h2d_PL_xy_positron", "Plastic Scintillator xy Hit; x (mm); y (mm)", 300, -15, 15, 300, -15, 15);
    TH2D H2D_PL_E0Angle_positron = TH2D("h2d_PL_E0Angle_positron", "Initial Positron Kinetic Energy vs Plastic Scintillator Hit Angle; E0 (keV); Angle (deg)", 10000, 0, 10000, 180, 0, 180);
    TH2D H2D_PL_E0EdepAngle_positron = TH2D("h2d_PL_E0EdepAngle_positron", "Energy Not Deposit vs Plastic Scintillator Hit Angle; Energy (keV); Angle (deg)", 10000, 0, 10000, 180, 0, 180);
    TH2D H2D_PL_E0_Edep_positron = TH2D("h2d_PL_E0_Edep_positron", "Initial Positron Kinetic Energy vs Deposit energy in Plastic Scintillator (catcher corrected); E0 (keV); Edep (keV)", 10000, 0, 10000, 10000, 0, 10000);

    // Silicon Detectors
    TH1D H1D_Si_Edep_proton = TH1D("h1d_Si_Edep_proton", "Silicon Detectors Proton Energy Deposit ; Edep (keV); Counts/keV", 10000, 0, 10000);
    TH1D H1D_Si_x_proton = TH1D("h1d_Si_x_proton", "Silicon Detectors Proton Hit x ; x (mm); Counts/0.1mm", 1000, -50, 50);
    TH1D H1D_Si_y_proton = TH1D("h1d_Si_y_proton", "Silicon Detectors Proton Hit y ; y (mm); Counts/0.1mm", 1000, -50, 50);
    TH1D H1D_Si_z_proton = TH1D("h1d_Si_z_proton", "Silicon Detectors Proton Hit z ; z (mm); Counts/0.1mm", 1000, -50, 50);
    TH1D H1D_Si_Angle_proton = TH1D("h1d_Si_Angle_proton", "Silicon Detectors Proton Hit Angle ; Angle (deg) ; Counts/deg", 180, 0, 180);
    TH1D H1D_Si_DL_Edep_proton = TH1D("h1d_Si_DL_Edep_proton", "Silicon Detectors Dead Layer Energy Deposit ; Edep (keV) ; Counts/keV", 10000, 0, 10000);

    // Catcher
    TH1D H1D_Catcher_Edep_proton = TH1D("h1d_Catcher_Edep_proton", "Catcher Proton Energy Deposit ; Edep (keV); Counts/keV", 10000, 0, 10000);
    TH1D H1D_Catcher_Edep_positron = TH1D("h1d_Catcher_Edep_positron", "Catcher Positron Energy Deposit ; Edep (keV); Counts/keV", 10000, 0, 10000);
    TH1D H1D_Catcher_Edep = TH1D("h1d_Catcher_Edep", "Catcher Energy Deposit ; Edep (keV); Counts/keV", 10000, 0, 10000);

    // ### Beta-P DETECTION ###//
    std::unordered_map<std::string, TH1D *> H1D_coinc;
    std::unordered_map<std::string, TH1D *> H1D_nocoinc;

    for (const string &name : Detector_Name)
    {
        H1D_coinc[(name + "coinc").c_str()] = new TH1D(("h1d_" + name + "_coinc").c_str(), (name + "Energy Deposit in Coincidence").c_str(), 100000, 0.0, 10000.0);
        H1D_nocoinc[(name + "no_coinc").c_str()] = new TH1D(("h1d_" + name + "_nocoinc").c_str(), (name + "Energy Deposit in Anti-Coincidence").c_str(), 100000, 0.0, 10000.0);
    }


    ///////////// TREE READER ///////////////
    TTreeReader Reader("Tree", file);
    TTreeReaderValue<int> EventNumber(Reader, "Event_Number");
    TTreeReaderValue<int> Particle_PDG(Reader, "Particle_PDG");
    TTreeReaderValue<double> x(Reader, "x");
    TTreeReaderValue<double> y(Reader, "y");
    TTreeReaderValue<double> z(Reader, "z");
    TTreeReaderValue<double> px(Reader, "px");
    TTreeReaderValue<double> py(Reader, "py");
    TTreeReaderValue<double> pz(Reader, "pz");
    TTreeReaderValue<double> E0(Reader, "Initial_Kinetic_Energy");
    TTreeReaderValue<double> Catcher_Edep(Reader, "Catcher_Central_Deposit_Energy");
    TTreeReaderValue<double> Pl_Edep(Reader, "PlasticScintillator_Deposit_Energy");
    TTreeReaderValue<double> Pl_Hit_x(Reader, "PlasticScintillator_Hit_Position_x");
    TTreeReaderValue<double> Pl_Hit_y(Reader, "PlasticScintillator_Hit_Position_y");
    TTreeReaderValue<double> Pl_Hit_z(Reader, "PlasticScintillator_Hit_Position_z");
    TTreeReaderValue<double> Pl_Hit_Angle(Reader, "PlasticScintillator_Hit_Angle");
    TTreeReaderValue<vector<double>> Si_Edep(Reader, "Silicon_Detector_Deposit_Energy");
    TTreeReaderValue<vector<double>> Si_Hit_x(Reader, "Silicon_Detector_Hit_Position_x");
    TTreeReaderValue<vector<double>> Si_Hit_y(Reader, "Silicon_Detector_Hit_Position_y");
    TTreeReaderValue<vector<double>> Si_Hit_z(Reader, "Silicon_Detector_Hit_Position_z");
    TTreeReaderValue<vector<double>> Si_Hit_Angle(Reader, "Silicon_Detector_Hit_Angle");
    TTreeReaderValue<vector<int>> Si_Hit_Code(Reader, "Silicon_Detector_Code");
    TTreeReaderValue<vector<double>> Si_DL_Edep(Reader, "Silicon_Detector_DL_Deposit_Energy");
    int count = 0;
    int j = 0;
    double Trigger=0;
    while (Reader.Next())
    {
        //PROGRESS BAR
        int progress = (j++ * 100) / Reader.GetEntries();
        cout << "[" << std::string(progress / 4, '=') << ">" << progress << "%]\r";
        cout.flush();


        // COMMON
        if (*EventNumber == count)
        {
            H1D_x.Fill(*x);
            H1D_y.Fill(*y);
            H1D_z.Fill(*z);
            H2D_xy.Fill(*x, *y);
            count++;
        }

        if (*Catcher_Edep != 0.)
        {
            H1D_Catcher_Edep.Fill(*Catcher_Edep);
        }
        if (*Pl_Edep != 0)
        {
            H1D_PL_Edep.Fill(*Pl_Edep);
            H1D_PL_x.Fill(*Pl_Hit_x);
            H1D_PL_y.Fill(*Pl_Hit_y);
            H1D_PL_z.Fill(*Pl_Hit_z);
            H1D_PL_Angle.Fill(*Pl_Hit_Angle);
        }

        // PROTON
        if (*Particle_PDG == 2212)
        {
            double proton_edep = *Pl_Edep;
            H1D_px_proton.Fill(*px);
            H1D_py_proton.Fill(*py);
            H1D_pz_proton.Fill(*pz);
            H1D_E0_proton.Fill(*E0);

            if (*Catcher_Edep != 0.)
            {
                H1D_Catcher_Edep_proton.Fill(*Catcher_Edep);
            }
            if (*Pl_Edep != 0.)
            {
                H1D_PL_Edep_proton.Fill(*Pl_Edep);
                H1D_PL_x_proton.Fill(*Pl_Hit_x);
                H1D_PL_y_proton.Fill(*Pl_Hit_y);
                H1D_PL_z_proton.Fill(*Pl_Hit_z);
                H1D_PL_Angle_proton.Fill(*Pl_Hit_Angle);
            }

            for (size_t hit = 0; hit < (*Si_Edep).size(); hit++)
            {
                if ((*Si_Edep)[hit] != 0.)
                {
                    H1D_Si_Edep_proton.Fill((*Si_Edep)[hit]);
                    H1D_Si_x_proton.Fill((*Si_Hit_x)[hit]);
                    H1D_Si_y_proton.Fill((*Si_Hit_y)[hit]);
                    H1D_Si_z_proton.Fill((*Si_Hit_z)[hit]);
                    H1D_Si_Angle_proton.Fill((*Si_Hit_Angle)[hit]);
                    H1D_Si_DL_Edep_proton.Fill((*Si_DL_Edep)[hit]);
                }
            }
        }

        // POSITRON
        if (*Particle_PDG == -11)
        {
            H1D_px_positron.Fill(*px);
            H1D_py_positron.Fill(*py);
            H1D_pz_positron.Fill(*pz);
            H1D_E0_positron.Fill(*E0);

            if (*Catcher_Edep != 0.)
            {
                H1D_Catcher_Edep_positron.Fill(*Catcher_Edep);
            }
            if (*Pl_Edep != 0.)
            {

                H1D_PL_Edep_positron.Fill(*Pl_Edep);
                H2D_PL_E0_Edep_positron.Fill(*E0 - *Catcher_Edep, *Pl_Edep);
                H1D_PL_x_positron.Fill(*Pl_Hit_x);
                H1D_PL_y_positron.Fill(*Pl_Hit_y);
                H1D_PL_z_positron.Fill(*Pl_Hit_z);
                H1D_PL_Angle_positron.Fill(*Pl_Hit_Angle);

                H2D_PL_xy_positron.Fill(*Pl_Hit_x, *Pl_Hit_y);
                H2D_PL_E0Angle_positron.Fill(*E0, *Pl_Hit_Angle);
                H2D_PL_E0EdepAngle_positron.Fill(*E0 - *Catcher_Edep - *Pl_Edep, *Pl_Hit_Angle);
            }
        }

        // DETECTION
        //double Trigger=0;
        // if (*EventNumber == counter)
        // {
            if (*Particle_PDG == -11)
            {
                Trigger = *Pl_Edep;
            }
            
        // /}
        // else
        // {
        //     Trigger = 0.;
        //     counter++;
        //     if (*Particle_Name == "e+")
        //     {
        //         Trigger = *Pl_Edep;
        //     }
        // }

        for (size_t hit = 0; hit < (*Si_Edep).size(); hit++)
        {
            if ((*Si_Edep)[hit] != 0.)
            {
                auto it = std::find(Detector_Code.begin(), Detector_Code.end(), (*Si_Hit_Code)[hit]);
                int index = std::distance(Detector_Code.begin(), it);
                string Name = Detector_Name[index];
                cout<<Name<<endl;
                if (Trigger >= threshoold_SiPMs)
                {
                    H1D_coinc[(Name + "coinc").c_str()]->Fill((*Si_Edep)[hit]);
                }
                else
                {
                    H1D_nocoinc[(Name + "no_coinc").c_str()]->Fill((*Si_Edep)[hit]);
                }
            }
        }
    }



    H1D_x.Write();
    H1D_y.Write();
    H1D_z.Write();
    H2D_xy.Write();
    H1D_px_proton.Write();
    H1D_py_proton.Write();
    H1D_pz_proton.Write();
    H1D_px_positron.Write();
    H1D_py_positron.Write();
    H1D_pz_positron.Write();
    H1D_E0_proton.Write();
    H1D_E0_positron.Write();
    H1D_PL_Edep_proton.Write();
    H1D_PL_x_proton.Write();
    H1D_PL_y_proton.Write();
    H1D_PL_z_proton.Write();
    H1D_PL_Angle_proton.Write();
    H1D_PL_Edep_positron.Write();
    H2D_PL_E0_Edep_positron.Write();
    H1D_PL_x_positron.Write();
    H1D_PL_y_positron.Write();
    H1D_PL_z_positron.Write();
    H1D_PL_Angle_positron.Write();
    H1D_PL_Edep.Write();
    H1D_PL_x.Write();
    H1D_PL_y.Write();
    H1D_PL_z.Write();
    H1D_PL_Angle.Write();
    H2D_PL_xy_positron.Write();
    H2D_PL_E0Angle_positron.Write();
    H2D_PL_E0EdepAngle_positron.Write();
    H1D_Si_Edep_proton.Write();
    H1D_Si_x_proton.Write();
    H1D_Si_y_proton.Write();
    H1D_Si_z_proton.Write();
    H1D_Si_Angle_proton.Write();
    H1D_Si_DL_Edep_proton.Write();
    H1D_Catcher_Edep_proton.Write();
    H1D_Catcher_Edep_positron.Write();
    H1D_Catcher_Edep.Write();

    for (const string &name : Detector_Name)
    {
        H1D_coinc[(name + "coinc").c_str()]->Write();
        H1D_nocoinc[(name + "no_coinc").c_str()]->Write();
    }

    file->Close();
    outfile->Close();

    return 0;
}
