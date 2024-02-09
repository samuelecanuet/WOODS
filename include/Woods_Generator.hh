#ifndef WITCH_GENERATOR_HH
#define WITCH_GENERATOR_HH

#include "Woods_Global.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "Woods_RunManager.hh"
#include "Woods_Tracking.hh"

#include "G4ParticleGun.hh"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include <unordered_map>
#include <tuple>
#include <functional>

#include "Randomize.hh"

<
//----------------------------------------------------------------------
struct Histogram
{
    G4double value_x;
    G4double value_y;
    G4double value_z;
    G4int count;
};

struct TupleHash {
    template <class T1, class T2, class T3>
    std::size_t operator () (const std::tuple<T1, T2, T3>& t) const {
        auto h1 = std::hash<T1>{}(std::get<0>(t));
        auto h2 = std::hash<T2>{}(std::get<1>(t));
        auto h3 = std::hash<T3>{}(std::get<2>(t));
        return h1 ^ h2 ^ h3;
    }
};

class Woods_Generator : public G4VUserPrimaryGeneratorAction
{
    //------------------------------------------------------------
    // internal variables definition
protected:
    Woods_RunManager *manager_ptr;
    double energy;
    int ievent, iev_len = 0, isubevent, isubev_len = 0;

    G4ParticleTable *particle_table = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *part_electron;
    G4ParticleDefinition *part_positron;
    G4ParticleDefinition *part_gamma;
    G4ParticleDefinition *part_proton;
    G4ParticleDefinition *part_enubar;
    G4ParticleDefinition *part_alpha;
    G4ParticleDefinition *part_geantino;

    G4double radius;
    G4double radius_std;
    G4double x_beam;
    G4double y_beam;
    G4double position_catcher_z;
    
    std::tuple<G4double, G4double, G4double> tuple;
    G4double x_SRIM, y_SRIM, z_SRIM;

    G4ParticleDefinition *particle;
    G4ThreeVector dir;
    G4ParticleGun gun;

    std::pair<std::vector<Histogram>, G4double> res;

    Long64_t current_entry = 0;

    //------------------------------------------------------------
    // class functions definition
private:
    TFile* root_file;
    unique_ptr<TTreeReader> Reader;

    unique_ptr<TTreeReaderValue<Int_t>> code;
    unique_ptr<TTreeReaderValue<Int_t>> eventid;
    unique_ptr<TTreeReaderValue<double>> ekin_;
    unique_ptr<TTreeReaderValue<double>> px;
    unique_ptr<TTreeReaderValue<double>> py;
    unique_ptr<TTreeReaderValue<double>> pz;



public:
    // constructor and destructor
    Woods_Generator(Woods_RunManager *mgr);

    // destructor
    ~Woods_Generator();

    // function generating the event primary vertex with emitted particles
    void GeneratePrimaries(G4Event *event);
    void TXT_GENERATOR(G4Event *event);
    void ROOT_GENERATOR(G4Event *event);
    std::function<void(G4Event*)> GENERATOR;

    std::pair<std::vector<Histogram>, G4double>GetSRIM_hist();
    std::tuple<G4double, G4double, G4double> GetSRIM_data(std::vector<Histogram>, G4double);

    void SetBeamSize(G4double r, G4double r_std);
    void SetBeamPosition(G4double x, G4double y);
    void SetCatcherPosition_z(G4double catcher_z);

};

inline std::pair<std::vector<Histogram>, G4double> Woods_Generator::GetSRIM_hist()
{
    if (manager_ptr->GetInputSRIM().fail())
    {
        G4Exception("Woods_Generator::GetSRIM_hist", "SRIM file wrong", JustWarning, "");
    }

    std::unordered_map<std::tuple<int, int, int>, int, TupleHash> value_counts;
    G4double sum = 0;
    
    
    std::string ligne;
    while (std::getline(manager_ptr->GetInputSRIM(), ligne)) {
        
        std::istringstream iss(ligne);
        std::string x_str, y_str, z_str;
        double num;
        
        if (iss >> num >> z_str >> y_str >> x_str) {
            
            double x = std::stod(x_str.replace(x_str.find(','), 1, ".")); 
            double y = std::stod(y_str.replace(y_str.find(','), 1, "."));
            double z = std::stod(z_str.replace(z_str.find(','), 1, "."));
        
            int rounded_x = static_cast<int>(std::round(x));
            int rounded_y = static_cast<int>(std::round(y));
            int rounded_z = static_cast<int>(std::round(z));
            
            std::tuple<int, int, int> key(rounded_x, rounded_y, rounded_z);
            value_counts[key]++;
        }
    }
    std::vector<Histogram> histogram_vector;
    
    for (const auto& pair : value_counts) {
        Histogram histogram;
        histogram.value_x = static_cast<G4double>(std::get<0>(pair.first));
        histogram.value_y = static_cast<G4double>(std::get<1>(pair.first));
        histogram.value_z = static_cast<G4double>(std::get<2>(pair.first));
        histogram.count = pair.second;
        sum += pair.second;
        
        
        histogram_vector.push_back(histogram);
    }

    return std::make_pair(histogram_vector, sum);
}

inline std::tuple<G4double, G4double, G4double> Woods_Generator::GetSRIM_data(std::vector<Histogram> histogram, double totalWeight)
{
    G4double randomValue = G4RandFlat::shoot(0.0, totalWeight);
    G4double cumulativeWeight = 0.0;
    for (const Histogram &bin : histogram)
    {
        cumulativeWeight += bin.count;
        if (randomValue <= cumulativeWeight)
        {
            return std::make_tuple(bin.value_x, bin.value_y, bin.value_z);
        }
    }
}


inline void Woods_Generator::SetBeamSize(G4double r, G4double r_std)
{
    radius = r;
    radius_std = r_std;
}

inline void Woods_Generator::SetBeamPosition(G4double x, G4double y)
{
    x_beam = x;
    y_beam = y;
}

inline void Woods_Generator::SetCatcherPosition_z(G4double catcher_z)
{
    position_catcher_z = catcher_z; 
}
#endif
