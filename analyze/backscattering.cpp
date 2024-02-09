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
#include <TGraphErrors.h>

using namespace std;

int main(int argc, char **argv)
{
    string filename_raw = "../../../../../../../mnt/hgfs/shared-2/save/32Ar_a1_b0_";
    string f = "32Ar_a1_b0_";
    //string material[2] = {"Pl", "Si"};
    string material[4] = {"1T", "2T", "3T", "4T"};

    std::unordered_map<std::string, vector<float>> DATA;
    vector<float> xx;
    TApplication *app = new TApplication("app", &argc, argv);
    TCanvas *c1 = new TCanvas("c1", "Graph Example", 800, 600);

    double energy[2] = {1,2};

    for (const string &mat : material)
    {
        for (int i = 1; i <= 2; i++)
        {
            TFile *outfile = new TFile((f+ i + "_" + mat + ".root"), "RECREATE");
            TFile *file = new TFile((filename_raw + i + "_" + mat + ".root"), "READ");
            cout<<filename_raw + i + "_" + mat + ".root"<<endl;

            int counter;

            /////////////////////////////////////////
            /////////////  HISTOGRAMS  //////////////
            /////////////////////////////////////////

            // ##### SOURCE #####//
            //////////////////////
            TH1D H1D_E0_positron = TH1D("h1d_E0_positron", "Initial Positron Kinetic Energy; E0 (keV); Counts/keV", 10000, 0, 10000);
            TH1D H1D_pz_positron = TH1D("h1d_pz_positron", "Initial z Momentum ; pz; Counts/keV", 2000, -1, 1);

            // #### DETECTORS ####//
            // Plastic Scintillator
            TH1D H1D_PL_Edep_positron = TH1D("h1d_PL_Edep_positron", "Plastic Scintillator Positron Energy Deposit ; Edep (keV); Counts/keV", 15000, 0, 15000);
            TH1D H1D_PL_x_positron = TH1D("h1d_PL_x_positron", "Plastic Scintillator Positron Hit x ; x (mm); Counts/0.1mm", 300, -15, 15);
            TH1D H1D_PL_y_positron = TH1D("h1d_PL_y_positron", "Plastic Scintillator Positron Hit y ; y (mm); Counts/0.1mm", 300, -15, 15);
            TH1D H1D_PL_z_positron = TH1D("h1d_PL_z_positron", "Plastic Scintillator Positron Hit z ; z (mm); Counts/0.1mm", 300, 0, 100);
            TH1D H1D_PL_Angle_positron = TH1D("h1d_PL_Angle_positron", "Plastic Scintillator Positron Hit Angle ; Angle (deg) ; Counts/deg", 180, 0, 180);

            TH2D H2D_PL_xy_positron = TH2D("h2d_PL_xy_positron", "Plastic Scintillator xy Hit; x (mm); y (mm)", 300, -15, 15, 300, -15, 15);
            TH2D H2D_PL_pzAngle_positron = TH2D("h2d_PL_pzAngle_positron", "Initial z Momentum vs Plastic Scintillator Hit Angle; pz; Angle (deg)", 2000, -1, 1, 180, 0, 180);
            TH2D H2D_PL_E0Angle_positron = TH2D("h2d_PL_E0Angle_positron", "Initial Positron Kinetic Energy vs Plastic Scintillator Hit Angle; E0 (keV); Angle (deg)", 15000, 0, 15000, 180, 0, 180);
            TH2D H2D_PL_E0EdepAngle_positron = TH2D("h2d_PL_E0EdepAngle_positron", "Energy Not Deposit vs Plastic Scintillator Hit Angle; Energy (keV); Angle (deg)", 15000, 0, 15000, 180, 0, 180);
            TH2D H2D_PL_E0_Edep_positron = TH2D("h2d_PL_E0_Edep_positron", "Initial Positron Kinetic Energy vs Deposit energy in Plastic Scintillator (catcher corrected); E0 (keV); Edep (keV)", 15000, 0, 15000, 10000, 0, 10000);

            ///////////// TREE READER ///////////////
            TTreeReader Reader("Tree", file);
            TTreeReaderValue<int> EventNumber(Reader, "Event_Number");
            TTreeReaderValue<string> Particle_PDG(Reader, "Particle_PDG");
            TTreeReaderValue<double> x(Reader, "x");
            TTreeReaderValue<double> y(Reader, "y");
            TTreeReaderValue<double> z(Reader, "z");
            TTreeReaderValue<double> px(Reader, "px");
            TTreeReaderValue<double> py(Reader, "py");
            TTreeReaderValue<double> pz(Reader, "pz");
            TTreeReaderValue<double> E0(Reader, "Initial_Kinetic_Energy");
            TTreeReaderValue<double> Catcher_Edep(Reader, "Catcher_Deposit_Energy");
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
            TTreeReaderValue<vector<int>> Si_Hit_Name(Reader, "Silicon_Detector_Name");
            TTreeReaderValue<vector<double>> Si_DL_Edep(Reader, "Silicon_Detector_DL_Deposit_Energy");
            int j = 0;

            

            while (Reader.Next())
            {
                // PROGRESS BAR
                int progress = (j++ * 100) / Reader.GetEntries();
                cout << "[" << std::string(progress / 4, '=') << ">" << progress << "%]\r";
                cout.flush();

                // POSITRON
                if (*Particle_PDG == -11)
                {
                    H1D_E0_positron.Fill(*E0);
                    H1D_pz_positron.Fill(*pz);
                    if (*Pl_Edep != 0.)
                    {
                        H1D_PL_Edep_positron.Fill(*Pl_Edep);
                        H1D_PL_Angle_positron.Fill(*Pl_Hit_Angle);
                        H2D_PL_xy_positron.Fill(*Pl_Hit_x, *Pl_Hit_y);
                        H2D_PL_E0Angle_positron.Fill(*E0, *Pl_Hit_Angle);
                        H2D_PL_pzAngle_positron.Fill(*pz, *Pl_Hit_Angle);
                    }
                }
            }

            outfile->cd();
            H1D_E0_positron.Write();
            H1D_PL_Edep_positron.Write();
            H1D_PL_Angle_positron.Write();
            H2D_PL_xy_positron.Write();
            H2D_PL_E0Angle_positron.Write();

            file->Close();
            outfile->Close();
            
        }
    }


    
    return 0;
}
