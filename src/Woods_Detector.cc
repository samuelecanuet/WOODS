#include "Woods_Detector.hh"
#include "Woods_Sensor.hh"
#include "Woods_MagnetField.hh"

#include "G4VisAttributes.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4TransportationManager.hh"
#include "G4ChordFinder.hh"
#include "G4FieldManager.hh"
#include "G4UniformMagField.hh"

#include "G4Trap.hh"

#include <unordered_map>
#include <utility>

//----------------------------------------------------------------------

Woods_Detector::Woods_Detector(Woods_RunManager *mgr)
{
  cout << "Constructor Woods_Detectors" << endl;
  manager_ptr = mgr;
}

// destructor
Woods_Detector::~Woods_Detector()
{
  cout << "Destructor Woods_Detectors" << endl;
}

//----------------------------------------------------------------------

// Function that builds all the simulation geometry
G4VPhysicalVolume *Woods_Detector::Construct()
{

  static G4bool fieldIsInitialized = false;

  if (!fieldIsInitialized)
  {
    G4FieldManager *pFieldMgr;
    // G4MagneticField *WoodsMagField = new WoodsMagnetField("MAGNETIC_FIELD_data/woods_field_complete.txt", 0.004); /// NON UNIFORM MAG FIELD GETFIELDVALUE method

    G4MagneticField *WoodsMagField = new G4UniformMagField(G4ThreeVector(0., 0., 4. * tesla));
    pFieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    G4ChordFinder *pChordFinder = new G4ChordFinder(WoodsMagField);
    pChordFinder->SetDeltaChord(1 * um);
    pFieldMgr->SetChordFinder(pChordFinder);
    pFieldMgr->SetDetectorField(WoodsMagField);
    fieldIsInitialized = true;
  }

  // #endif

  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  //--------------------------------------------------------------------------------------
  //------------------------------------ World -------------------------------------------
  //--------------------------------------------------------------------------------------
  G4double innerRadius = 0 * cm;
  G4double outerRadius = 6.5 * cm; // réduit au raypon du Bore pour opti6.5
  G4double length = 21. * cm;      // réduit pour opti21cm
  G4double theta1 = 90.0 * deg;
  G4double phi = 360.0 * deg;

  fSolidWorld = new G4Tubs("World", innerRadius, outerRadius, length / 2, theta1, phi);

  fLogicWorld = new G4LogicalVolume(fSolidWorld, // its solid
                                    vide,        // its material
                                    "World");    // its name

  fPhysiWorld = new G4PVPlacement(0,               // no rotation
                                  G4ThreeVector(), // at (0,0,0)
                                  fLogicWorld,     // its fLogical volume
                                  "World",         // its name
                                  0,               // its mother  volume
                                  false,           // no boolean operation
                                  0);              // copy number

  G4VisAttributes *visAtt_World = new G4VisAttributes(G4Colour(1, 1, 1)); // white
  visAtt_World->SetVisibility(false);
  fLogicWorld->SetVisAttributes(visAtt_World);

  //==================================================================================================
  //========================================  WISArD MAGNET =========================================
  //==================================================================================================
  G4Material *materialIron = G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");
  G4double radius_woods = 70.0 * cm;
  G4double radius_bore = 6.5 * cm;
  G4double length_bore = 80 * cm;
  G4double radius_wall = 0.3 * cm;

  G4Tubs *woods = new G4Tubs("Woods_magnet",
                              radius_bore + radius_wall,
                              radius_woods,
                              0.5L * (length_bore),
                              0.L, 360.L * deg);
  G4LogicalVolume *woods_logic = new G4LogicalVolume(woods,        // forme
                                                      materialIron,  // matiere
                                                      "WoodsLogic", // nom
                                                      0, 0, 0);
  // - physical volume
  G4VPhysicalVolume *woods_phys = new G4PVPlacement(0,
                                                     G4ThreeVector(0., 0., 0.),
                                                     woods_logic, // ptr sur vol. log.
                                                     "Woods",     // nom du volume
                                                     fLogicWorld,  // volume parent
                                                     false, true);

  // - make the bore volume "visible"
  G4VisAttributes *visAtt_WITCHMagnet = new G4VisAttributes(G4Colour(0.5, 0.7, 0.7, 0.2));
  visAtt_WITCHMagnet->SetVisibility(false);
  visAtt_WITCHMagnet->SetForceWireframe(false);
  visAtt_WITCHMagnet->SetForceSolid(true);
  woods_logic->SetVisAttributes(visAtt_WITCHMagnet);

  if (woods_phys == NULL)
  {
  }

  //==================================================================================================
  //=======================================  BORE - MAGNET ==========================================
  //==================================================================================================
  G4Material *materialAluminum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");

  G4Tubs *bore = new G4Tubs("Bore_Tube",
                            radius_bore,
                            radius_bore + radius_wall,
                            0.5L * (length_bore),
                            0.L, 360.L * deg);
  G4LogicalVolume *bore_logic = new G4LogicalVolume(bore,             // forme
                                                    materialAluminum, // matiere
                                                    "BoreLogic",      // nom
                                                    0, 0, 0);
  // - physical volume
  G4VPhysicalVolume *bore_phys = new G4PVPlacement(0,
                                                   G4ThreeVector(0., 0., 0.),
                                                   bore_logic,  // ptr sur vol. log.
                                                   "Bore",      // nom du volume
                                                   fLogicWorld, // volume parent
                                                   false, true);

  // - make the bore volume "visible"
  G4VisAttributes *visAttr_Bore = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.2));
  visAttr_Bore->SetVisibility(false);
  visAttr_Bore->SetForceWireframe(false);
  visAttr_Bore->SetForceSolid(true);
  bore_logic->SetVisAttributes(visAttr_Bore);

  if (bore_phys == NULL)
  {
  }

  //======================================================================================================
  //====================================== CYLINDER ======================================================
  //======================================================================================================
  innerRadius = 71. * cm;
  outerRadius = 72. * cm;
  length = 100. * cm;
  theta = 90.0 * deg;
  phi = 360.0 * deg;

  G4Material *fMaterial_Al = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
  G4Tubs *fSolidMYCILINDER = new G4Tubs("MYCILINDER", innerRadius, outerRadius, length / 2, theta, phi);

  G4LogicalVolume *fLogicMYCILINDER = new G4LogicalVolume(fSolidMYCILINDER, // its solid
                                                          fMaterial_Al,     // its material
                                                          "MYCILINDER");    // its name

  G4VPhysicalVolume *fPhysiMYCILINDER = new G4PVPlacement(0,                      // no rotation
                                                          G4ThreeVector(0, 0, 0), // at (0,0,0)
                                                          fLogicMYCILINDER,       // its fLogical volume
                                                          "MYCILINDER",           // its name
                                                          fLogicWorld,            // its mother  volume
                                                          false,                  // no boolean operation
                                                          0,
                                                          false); // copy number

  G4VisAttributes *visAtt_MYCILINDER = new G4VisAttributes(G4Colour(1, 1, 0)); // white
  visAtt_MYCILINDER->SetVisibility(false);
  fLogicMYCILINDER->SetVisAttributes(visAtt_MYCILINDER);

  if (fPhysiMYCILINDER == NULL)
  {
  }

  // BASE 1
  innerRadius = 0 * mm;
  outerRadius = 72 * cm;
  G4double lengthB = 1. * cm;
  theta = 90.0 * deg;
  phi = 360.0 * deg;

  G4Tubs *fSolidMYCILINDER_B1 = new G4Tubs("MYCILINDER_B1", innerRadius, outerRadius, lengthB / 2, theta, phi);

  G4LogicalVolume *fLogicMYCILINDER_B1 = new G4LogicalVolume(fSolidMYCILINDER_B1, // its solid
                                                             fMaterial_Al,        // its material
                                                             "MYCILINDER_B1");    // its name

  G4VPhysicalVolume *fPhysiMYCILINDER_B1 = new G4PVPlacement(0,                                                  // no rotation
                                                             G4ThreeVector(0, 0, -(length / 2) - (lengthB / 2)), // at (0,0,0)
                                                             fLogicMYCILINDER_B1,                                // its fLogical volume
                                                             "MYCILINDER_B1",                                    // its name
                                                             fLogicMYCILINDER,                                   // its mother  volume
                                                             false,                                              // no boolean operation
                                                             0,
                                                             false); // copy number

  G4VisAttributes *visAtt_MYCILINDER_B1 = new G4VisAttributes(G4Colour(1, 1, 0)); // white
  visAtt_MYCILINDER_B1->SetVisibility(false);
  fLogicMYCILINDER_B1->SetVisAttributes(visAtt_MYCILINDER_B1);

  if (fPhysiMYCILINDER_B1 == NULL)
  {
  }

  // BASE 2
  G4Tubs *fSolidMYCILINDER_B2 = new G4Tubs("MYCILINDER_B2", innerRadius, outerRadius, lengthB / 2, theta, phi);

  G4LogicalVolume *fLogicMYCILINDER_B2 = new G4LogicalVolume(fSolidMYCILINDER_B2, // its solid
                                                             fMaterial_Al,        // its material
                                                             "MYCILINDER_B2");    // its name

  G4VPhysicalVolume *fPhysiMYCILINDER_B2 = new G4PVPlacement(0,                                                 // no rotation
                                                             G4ThreeVector(0, 0, (length / 2) + (lengthB / 2)), // at (0,0,0)
                                                             fLogicMYCILINDER_B2,                               // its fLogical volume
                                                             "MYCILINDER_B2",                                   // its name
                                                             fLogicMYCILINDER,                                  // its mother  volume
                                                             false,                                             // no boolean operation
                                                             0,
                                                             false); // copy number

  G4VisAttributes *visAtt_MYCILINDER_B2 = new G4VisAttributes(G4Colour(1, 1, 0)); // white
  visAtt_MYCILINDER_B2->SetVisibility(false);
  fLogicMYCILINDER_B2->SetVisAttributes(visAtt_MYCILINDER_B2);

  if (fPhysiMYCILINDER_B2 == NULL)
  {
  }

  
  G4Material *Cu = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
  G4Material *Al = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
  G4VisAttributes *Copper = new G4VisAttributes(G4Colour(0.72, 0.45, 0.2)); 
  Copper->SetForceWireframe(false);
  Copper->SetForceSolid(true);    

  outerRadius = 6.5 * cm;

    // MOTHER CATCHER
    G4double Radius_Rotation = 50 * mm;
    G4Tubs *mother_catcher = new G4Tubs("mother", 0., outerRadius * 5, 5 * mm, 0., 360. * deg);
    G4LogicalVolume *logic_mother_catcher = new G4LogicalVolume(mother_catcher, vide, "logic_mother_catcher");
    phys_mother_catcher = new G4PVPlacement(0, // no rotation
                                            G4ThreeVector(0, -Radius_Rotation, 0.5*mm),
                                            logic_mother_catcher, // its fLogical volume
                                            "mothercatcher",      // its name
                                            fLogicWorld,          // its mother  volume
                                            false,                // no boolean operation
                                            0);

    G4double SuppCatcher_Tige_width = 20 * mm;
    G4double SuppCatcher_Tige_height = 31.2 * mm;
    G4double SuppCatcher_Catcher_radius_inner = 7.5 * mm;
    G4double SuppCatcher_Catcher_radius_outer = 9 * mm;
    G4double SuppCatcher_Source_radius_inner = 4.5 * mm;
    G4double SuppCatcher_Source_radius_outer = 5.5 * mm;
    PEEK_thikness = 1.5 * mm;

    /// Main Plate
    double L1 = 30 * mm;
    double L2 = 31 * mm;
    double H = 30 * mm;
    double c = 10 * mm;
    double cc = 4 * mm;
    double l = 20 * mm;
    double h = 9 * mm;
    double Hl = 11 * mm;

    double SuppCatcher_Plate_height = H;

    std::vector<G4TwoVector> polygon(10);

    polygon[0].set(-L2 + c, -H / 2);
    polygon[1].set(-L2, -H / 2 + c);
    polygon[2].set(-L2, H / 2 + -Hl - cc);
    polygon[3].set(-L2 + cc, H / 2 + -Hl);
    polygon[4].set(-L2 + l, -H / 2 + h + c);
    polygon[5].set(-L2 + l, H / 2);
    polygon[6].set(L1 - c, H / 2);
    polygon[7].set(L1, H / 2 - c);
    polygon[8].set(L1, -H / 2 + c);
    polygon[9].set(L1 - c, -H / 2);

    G4TwoVector offsetA(0, 0), offsetB(0, 0);
    G4double scaleA = 1, scaleB = 1;
    G4VSolid *SuppCatcher_Plate = new G4ExtrudedSolid("Extruded", polygon, SuppCatcher_thikness / 2, offsetA, scaleA, offsetB, scaleB);

    // PEEK ring
    G4Material *PEEK = new G4Material("PEEKS", 1.32 * g / cm3, 3);
    G4NistManager *man = G4NistManager::Instance();
    PEEK->AddElement(man->FindOrBuildElement("C"), 19);
    PEEK->AddElement(man->FindOrBuildElement("H"), 12);
    PEEK->AddElement(man->FindOrBuildElement("O"), 3);
    G4Tubs *PEEK_Ring = new G4Tubs("PEEK_ring", SuppCatcher_Catcher_radius_inner, SuppCatcher_Catcher_radius_outer, PEEK_thikness / 2, 0., 360 * deg);

    // Catcher Hole Inner
    G4Tubs *SuppCatcher_Catcher_inner = new G4Tubs("Catcher_inner", 0., SuppCatcher_Catcher_radius_inner, SuppCatcher_thikness, 0., 360. * deg);

    // Catcher Hole Outer
    G4Tubs *SuppCatcher_Catcher_outer = new G4Tubs("Catcher_outer", 0., SuppCatcher_Catcher_radius_outer, PEEK_thikness / 2, 0., 360. * deg);

    // Source Hole Inner
    G4Tubs *SuppCatcher_Source_inner = new G4Tubs("Catcher_inner", 0., SuppCatcher_Source_radius_inner, SuppCatcher_thikness, 0., 360. * deg);

    // Source Hole Outer
    G4Tubs *SuppCatcher_Source_outer = new G4Tubs("Catcher_outer", 0., SuppCatcher_Source_radius_outer, PEEK_thikness / 2, 0., 360. * deg);

    // Tige
    G4Box *SuppCatcher_Tige = new G4Box("SuppCatcher_Tige", SuppCatcher_Tige_width / 2, SuppCatcher_Tige_height / 2, SuppCatcher_thikness / 2);

    ///////////ASSEMBLY////////////
    Support_Position = G4ThreeVector(0, -Radius_Rotation * (1 - cos(22.5 * deg)) + Radius_Rotation, 0);
    // Central Catcher Hole
    Central_Hole_Position = G4ThreeVector(0, Radius_Rotation * (1 - cos(22.5 * deg)), 0);
    G4VSolid *SuppCatcher_Plate_centralCatcher_inner = new G4SubtractionSolid("SuppCatcher_Plate-centralCatcher", SuppCatcher_Plate, SuppCatcher_Catcher_inner, 0, Central_Hole_Position);
    G4VSolid *SuppCatcher_Plate_centralCatcher = new G4SubtractionSolid("SuppCatcher_Plate-centralCatcher", SuppCatcher_Plate_centralCatcher_inner, SuppCatcher_Catcher_outer, 0, Central_Hole_Position + G4ThreeVector(0, 0, SuppCatcher_thikness / 2 - PEEK_thikness / 2));

    G4LogicalVolume *logic_centralPEEK_Ring = new G4LogicalVolume(PEEK_Ring, PEEK, "logic_Supp_catcher");
    phys_centralPEEK_ring = new G4PVPlacement(0, // no rotation
                                              Support_Position + Central_Hole_Position + G4ThreeVector(0, 0, SuppCatcher_thikness / 2 - PEEK_thikness / 2),
                                              logic_centralPEEK_Ring, // its fLogical volume
                                              "centralPEEK_Ring",     // its name
                                              logic_mother_catcher,   // its mother  volume
                                              false,                  // no boolean operation
                                              0);

    G4VisAttributes *visAtt_PEEK = new G4VisAttributes(G4Colour(1., 1., 1.));
    visAtt_PEEK->SetVisibility(true);
    visAtt_PEEK->SetForceWireframe(false);
    visAtt_PEEK->SetForceSolid(true);
    logic_centralPEEK_Ring->SetVisAttributes(visAtt_PEEK);

    // Side Catcher Hole
    Side_Hole_Position = G4ThreeVector(Radius_Rotation * sin(22.5 * deg), 0, 0);
    G4VSolid *SuppCatcher_Plate_centralsideCatcher_inner = new G4SubtractionSolid("SuppCatcher_Plate-sideCatcher", SuppCatcher_Plate_centralCatcher, SuppCatcher_Catcher_inner, 0, Side_Hole_Position);
    G4VSolid *SuppCatcher_Plate_centralsideCatcher = new G4SubtractionSolid("SuppCatcher_Plate-sideCatcher", SuppCatcher_Plate_centralsideCatcher_inner, SuppCatcher_Catcher_outer, 0, Side_Hole_Position + G4ThreeVector(0, 0, SuppCatcher_thikness / 2 - PEEK_thikness / 2));

    G4LogicalVolume *logic_sidePEEK_Ring = new G4LogicalVolume(PEEK_Ring, PEEK, "logic_Supp_catcher");
    phys_sidePEEK_ring = new G4PVPlacement(0, // no rotation
                                           Support_Position + Side_Hole_Position + G4ThreeVector(0, 0, SuppCatcher_thikness / 2 - PEEK_thikness / 2),
                                           logic_sidePEEK_Ring,  // its fLogical volume
                                           "sidePEEK_Ring",      // its name
                                           logic_mother_catcher, // its mother  volume
                                           false,                // no boolean operation
                                           0);

    logic_sidePEEK_Ring->SetVisAttributes(visAtt_PEEK);

    // Source Hole
    G4ThreeVector Source_Hole_Position = G4ThreeVector(-Radius_Rotation * sin(30 * deg), Radius_Rotation * (cos(30 * deg) - cos(22.5 * deg)), 0);
    G4VSolid *SuppCatcher_Plate_centralsidesourceCatcher_inner = new G4SubtractionSolid("SuppCatcher_Plate-source", SuppCatcher_Plate_centralsideCatcher, SuppCatcher_Source_inner, 0, Source_Hole_Position);
    G4VSolid *SuppCatcher_Plate_centralsidesourceCatcher = new G4SubtractionSolid("SuppCatcher_Plate-source", SuppCatcher_Plate_centralsidesourceCatcher_inner, SuppCatcher_Source_outer, 0, Source_Hole_Position + G4ThreeVector(0, 0, SuppCatcher_thikness / 2 - PEEK_thikness / 2));

    // Tige
    G4ThreeVector Tige_Position = G4ThreeVector(0, -SuppCatcher_Tige_height / 2 - SuppCatcher_Plate_height / 2, 0);
    G4UnionSolid *SuppCatcher_Plate_Tige = new G4UnionSolid("SuppCatcher_Plate_Tige", SuppCatcher_Plate_centralsidesourceCatcher, SuppCatcher_Tige, 0, Tige_Position);

    // Plate
    G4LogicalVolume *logic_SuppCatcher_Plate = new G4LogicalVolume(SuppCatcher_Plate_Tige, Al, "logic_Supp_catcher");
    phys_Supp_catcher = new G4PVPlacement(0, // no rotation
                                          Support_Position,
                                          logic_SuppCatcher_Plate,  // its fLogical volume
                                          "phys_SuppCatcher_Plate", // its name
                                          logic_mother_catcher,     // its mother  volume
                                          false,                    // no boolean operation
                                          0);

    G4VisAttributes *visAtt_Supp_catcher = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
    visAtt_Supp_catcher->SetVisibility(true);
    visAtt_Supp_catcher->SetForceWireframe(false);
    visAtt_Supp_catcher->SetForceSolid(true);
    logic_SuppCatcher_Plate->SetVisAttributes(visAtt_Supp_catcher);

    //// CATCHER //////
    /// CENTRAL
    Catcher_central_Position = Support_Position + Central_Hole_Position + G4ThreeVector(0, 0, SuppCatcher_thikness / 2 - PEEK_thikness);
    G4Material *Mylar = G4NistManager::Instance()->FindOrBuildMaterial("G4_MYLAR");
    thicknessMylarSource_central = 0.9 * um;
    thicknessAlSource = 50. * nm;

    AlSource1 = new G4Tubs("AlSource1_central", 0., SuppCatcher_Catcher_radius_inner, thicknessAlSource / 2, 0., 360. * deg);
    G4LogicalVolume *Logic_AlSource1_central = new G4LogicalVolume(AlSource1, Al, "LogicAlSource1_central");               // solid, material, name
    Physics_AlSource1_central = new G4PVPlacement(0,                                                                       // no rotation
                                                  Catcher_central_Position + G4ThreeVector(0., 0., thicknessAlSource / 2), // position
                                                  Logic_AlSource1_central, "LogicAlSource1_central",                       // its fLogical volume
                                                  logic_mother_catcher,                                                    // its mother volume
                                                  false,                                                                   // no boolean op.
                                                  0);                                                                      // copy nb.

    MylarSource_central = new G4Tubs("MylarSource", 0., SuppCatcher_Catcher_radius_inner, thicknessMylarSource_central / 2, 0., 360. * deg);
    G4LogicalVolume *Logic_MylarSource_central = new G4LogicalVolume(MylarSource_central, Mylar, "LogicMylarSource_central");                               // solid, material, name
    Physics_MylarSource_central = new G4PVPlacement(0,                                                                                                      // no rotation
                                                    Catcher_central_Position + G4ThreeVector(0., 0., thicknessMylarSource_central / 2 + thicknessAlSource), // position
                                                    Logic_MylarSource_central, "Logic_MylarSource_central",                                                 // its fLogical volume
                                                    logic_mother_catcher,                                                                                   // its mother volume
                                                    false,                                                                                                  // no boolean op.
                                                    0);                                                                                                     // copy nb.

    AlSource2 = new G4Tubs("AlSource2_central", 0., SuppCatcher_Catcher_radius_inner, thicknessAlSource / 2, 0., 360. * deg);
    G4LogicalVolume *Logic_AlSource2_central = new G4LogicalVolume(AlSource2, Al, "LogicAlSource2_central");                                                                  // solid, material, name
    Physics_AlSource2_central = new G4PVPlacement(0,                                                                                                                          // no rotation
                                                  Catcher_central_Position + G4ThreeVector(0., 0., thicknessAlSource / 2 + thicknessMylarSource_central + thicknessAlSource), // position
                                                  Logic_AlSource2_central, "LogicAlSource2_central",                                                                          // its fLogical volume
                                                  logic_mother_catcher,                                                                                                       // its mother volume
                                                  false,                                                                                                                      // no boolean op.
                                                  0);                                                                                                                         // copy nb.

    G4VisAttributes *MylarSource_att = new G4VisAttributes(G4Colour(0.94, 0.5, 0.5)); // pink
    MylarSource_att->SetVisibility(true);
    MylarSource_att->SetForceWireframe(false);
    MylarSource_att->SetForceSolid(true);
    Logic_MylarSource_central->SetVisAttributes(MylarSource_att);

    G4VisAttributes *AlSource_att = new G4VisAttributes(G4Colour(0.94, 0.2, 0.5)); // pink
    AlSource_att->SetVisibility(true);
    AlSource_att->SetForceWireframe(false);
    AlSource_att->SetForceSolid(true);
    Logic_AlSource1_central->SetVisAttributes(AlSource_att);
    Logic_AlSource2_central->SetVisAttributes(AlSource_att);

    /// SIDE
    Catcher_side_Position = Support_Position + Side_Hole_Position + G4ThreeVector(0, 0, SuppCatcher_thikness / 2 - PEEK_thikness);
    thicknessMylarSource_side = 0.5 * um;

    G4LogicalVolume *Logic_AlSource1_side = new G4LogicalVolume(AlSource1, Al, "LogicAlSource1_side");               // solid, material, name
    Physics_AlSource1_side = new G4PVPlacement(0,                                                                    // no rotation
                                               Catcher_side_Position + G4ThreeVector(0., 0., thicknessAlSource / 2), // position
                                               Logic_AlSource1_side, "LogicAlSource1_side",                          // its fLogical volume
                                               logic_mother_catcher,                                                 // its mother volume
                                               false,                                                                // no boolean op.
                                               0);                                                                   // copy nb.

    MylarSource_side = new G4Tubs("MylarSource", 0., SuppCatcher_Catcher_radius_inner, thicknessMylarSource_side / 2, 0., 360. * deg);
    G4LogicalVolume *Logic_MylarSource_side = new G4LogicalVolume(MylarSource_side, Mylar, "LogicMylarSource_side");                               // solid, material, name
    Physics_MylarSource_side = new G4PVPlacement(0,                                                                                                // no rotation
                                                 Catcher_side_Position + G4ThreeVector(0., 0., thicknessMylarSource_side / 2 + thicknessAlSource), // position
                                                 Logic_MylarSource_side, "Logic_MylarSource_side",                                                 // its fLogical volume
                                                 logic_mother_catcher,                                                                             // its mother volume
                                                 false,                                                                                            // no boolean op.
                                                 0);                                                                                               // copy nb.

    G4LogicalVolume *Logic_AlSource2_side = new G4LogicalVolume(AlSource2, Al, "LogicAlSource2_side");                                                               // solid, material, name
    Physics_AlSource2_side = new G4PVPlacement(0,                                                                                                                    // no rotation
                                               Catcher_side_Position + G4ThreeVector(0., 0., thicknessAlSource / 2 + thicknessMylarSource_side + thicknessAlSource), // position
                                               Logic_AlSource2_side, "LogicAlSource2_side",                                                                          // its fLogical volume
                                               logic_mother_catcher,                                                                                                 // its mother volume
                                               false,                                                                                                                // no boolean op.
                                               0);                                                                                                                   // copy nb.

    Logic_MylarSource_side->SetVisAttributes(MylarSource_att);
    Logic_AlSource1_side->SetVisAttributes(AlSource_att);
    Logic_AlSource2_side->SetVisAttributes(AlSource_att);

    /// SOURCE
    G4double thicknessSource = 1.5 * mm;
    Source_Position = Support_Position + Source_Hole_Position + G4ThreeVector(0, 0, SuppCatcher_thikness / 2 - PEEK_thikness);

    Source = new G4Tubs("Source", 0., SuppCatcher_Source_radius_outer, thicknessSource / 2, 0., 360. * deg);
    G4LogicalVolume *Logic_Source = new G4LogicalVolume(Source, Cu, "Logic_Source");                 // solid, material, name
    Physics_Source = new G4PVPlacement(0,                                                            // no rotation
                                       Source_Position + G4ThreeVector(0., 0., thicknessSource / 2), // position
                                       Logic_Source, "Phys_Source",                                  // its fLogical volume
                                       logic_mother_catcher,                                         // its mother volume
                                       false,                                                        // no boolean op.
                                       0);

    Copper->SetForceWireframe(false);
    Copper->SetForceSolid(true);
    Logic_Source->SetVisAttributes(Copper);

    //// GARAGE ////
    G4double Garage_height = 40 * mm;
    G4double Garage_width = 32 * mm;
    G4double Garage_Thickness = 1 * mm;
    G4double Garage_Space1 = 5 * mm;
    G4double Garage_Space2 = 5 * mm;
    G4double Garage_X = -4.5 * cm;
    G4double Garage_Y = -4.5 * cm;
    G4double Garage_Z = 0 * mm;
    G4ThreeVector Garage_Position = G4ThreeVector(Garage_X, Garage_Y + 4 * mm, Garage_Z);
    G4VSolid *Garage1 = new G4Box("Garage1", Garage_width / 2, Garage_height / 2, Garage_Thickness / 2);
    G4LogicalVolume *Logic_Garage1 = new G4LogicalVolume(Garage1, Al, "Logic_Garage1");                                                          // solid, material, name
    G4PVPlacement *Physics_Garage1 = new G4PVPlacement(0,                                                                                        // no rotation
                                                       Garage_Position + G4ThreeVector(Garage_width / 2, Garage_height / 2, -Garage_Space1 / 2), // position
                                                       Logic_Garage1, "Phys_Garage1",                                                            // its fLogical volume
                                                       fLogicWorld,                                                                              // its mother volume
                                                       false,                                                                                    // no boolean op.
                                                       0);
    G4VSolid *Garage2 = new G4Box("Garage2", Garage_width / 2, Garage_height / 2, Garage_Thickness / 2);
    G4LogicalVolume *Logic_Garage2 = new G4LogicalVolume(Garage2, Al, "Logic_Garage2");                                                         // solid, material, name
    G4PVPlacement *Physics_Garage2 = new G4PVPlacement(0,                                                                                       // no rotation
                                                       Garage_Position + G4ThreeVector(Garage_width / 2, Garage_height / 2, Garage_Space2 / 2), // position
                                                       Logic_Garage2, "Phys_Garage2",                                                           // its fLogical volume
                                                       fLogicWorld,                                                                             // its mother volume
                                                       false,                                                                                   // no boolean op.
                                                       0);

    Logic_Garage2->SetVisAttributes(visAtt_Supp_catcher);
    Logic_Garage1->SetVisAttributes(visAtt_Supp_catcher);

    Logic_MylarSource_central->SetSensitiveDetector(manager_ptr->GetWoodsSensor_CatcherMylar_central());
    Logic_AlSource1_central->SetSensitiveDetector(manager_ptr->GetWoodsSensor_CatcherAl1_central());
    Logic_AlSource2_central->SetSensitiveDetector(manager_ptr->GetWoodsSensor_CatcherAl2_central());
    Logic_MylarSource_side->SetSensitiveDetector(manager_ptr->GetWoodsSensor_CatcherMylar_side());
    Logic_AlSource1_side->SetSensitiveDetector(manager_ptr->GetWoodsSensor_CatcherAl1_side());
    Logic_AlSource2_side->SetSensitiveDetector(manager_ptr->GetWoodsSensor_CatcherAl2_side());                              

  //===================================================================================================================================================================
  //===================================================================================================================================================================
  //==================================================   WISArD PLASTIC SCINTILLATOR - WITH SUPPORT ===================================================================
  //===================================================================================================================================================================
  //===================================================================================================================================================================
  // NB: per ogni dubbio sulle misure, consultare i disegni tecnici inviati da M. Roche (mail al mio indirizzo cenbg il 18/05/2021).


  ////////////////////UPPER///////////////////////////
  G4double distanza_tra_BaseInfScintillatore_e_BordoSuperioreDeiSiDetector = 25.575 * mm;
  G4double z_height_Source_biggerBaseSiDet_inVerticale = 24.92 * mm;
  G4double thickness_z_SupportoRame_SiDetector = 1.5 * mm;
  G4double delta = 0 * cm;
  G4Material *fMaterial_PlasticScintillator = G4NistManager::Instance()->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

  fSolid_PlasticScintillator = new G4Tubs("PlasticScintillator", 0., fRadius_PlasticScintillator, 0.5 * fLength_PlasticScintillator, 0., 360 * deg);   
                                                                                  // name, r : 0->1cm, L : 5cm, phi : 0->2pi
  G4LogicalVolume *fLogic_PlasticScintillatorUp = new G4LogicalVolume(fSolid_PlasticScintillator, fMaterial_PlasticScintillator, "PlasticScintillator");                                                                                         // solid, material, name
  fPhys_PlasticScintillatorUp = new G4PVPlacement(0,                                                                                                                                                                              // rotationMatrix
                                                               G4ThreeVector(0., 0., z_height_Source_biggerBaseSiDet_inVerticale + distanza_tra_BaseInfScintillatore_e_BordoSuperioreDeiSiDetector + fLength_PlasticScintillator / 2 + delta), // position
                                                               fLogic_PlasticScintillatorUp, "PlasticScintillatorUp",                                                                                                                              // its fLogical volume
                                                               fLogicWorld,                                                                                                                                                                    // its mother volume
                                                               false,                                                                                                                                                                          // no boolean op.
                                                               0);                                                                                                                                                                             // copy nb.
  G4VisAttributes *PlasticScintillator_att = new G4VisAttributes(G4Colour(0.6, 0.6, 0.6, 0.6));                                                                                                                                                // red
  PlasticScintillator_att->SetForceWireframe(false);
  PlasticScintillator_att->SetForceSolid(true);
  fLogic_PlasticScintillatorUp->SetVisAttributes(PlasticScintillator_att);

  if (fPhys_PlasticScintillatorUp == NULL)
  {
  }

  G4double length_x_SupportoRame_PlasticScint = 20.5 * mm - 2 * mm;
  G4double height_y_SupportoRame_PlasticScint = 20.5 * mm - 2 * mm;
  

  bigBox_material_SupportoRame_PlasticScint = new G4Box("bigBox_material_SupportoRame_PlasticScint", length_x_SupportoRame_PlasticScint, height_y_SupportoRame_PlasticScint, thickness_z_SupportoRame_PlasticScint / 2); // corretto non div per due x e y
  holeForPlasticScint = new G4Tubs("holeForPlasticScint", 0., fRadius_PlasticScintillator, thickness_z_SupportoRame_PlasticScint, 0., 360 * deg);

  Box_material_SupportoRame_PlasticScint = new G4SubtractionSolid("Box_material_SupportoRame_PlasticScint",
                                                                            bigBox_material_SupportoRame_PlasticScint, holeForPlasticScint,
                                                                            0,
                                                                            G4ThreeVector(0., 0., 0.));

  G4LogicalVolume *logic_Box_material_SupportoRame_PlasticScintUp = new G4LogicalVolume(Box_material_SupportoRame_PlasticScint, Cu, "logic_Box_material_SupportoRame_PlasticScintUp"); // solid, material, name
  physics_Box_material_SupportoRame_PlasticScintUp = new G4PVPlacement(0,                                                                                         // no rotation
                                                                                        G4ThreeVector(0., 0., delta + z_height_Source_biggerBaseSiDet_inVerticale + distanza_tra_BaseInfScintillatore_e_BordoSuperioreDeiSiDetector + thickness_z_SupportoRame_SiDetector / 2),
                                                                                        logic_Box_material_SupportoRame_PlasticScintUp, "logic_Box_material_SupportoRame_PlasticScintUp", // its fLogical volume
                                                                                        fLogicWorld,                                                                                  // its mother volume
                                                                                        false,                                                                                        // no boolean op.
                                                                                        0);                                                                                           // copy nb.

  logic_Box_material_SupportoRame_PlasticScintUp->SetVisAttributes(Copper);
  

  if (physics_Box_material_SupportoRame_PlasticScintUp == NULL)
  {
  }

  G4double lenghtPlaccaCilidrica = fLength_PlasticScintillator - thickness_z_SupportoRame_SiDetector;

  placcaRame_Cilindrica = new G4Tubs("placcaRame_Cilindrica", fRadius_PlasticScintillator, fRadius_PlasticScintillator + thickness_z_SupportoRame_PlasticScint, lenghtPlaccaCilidrica / 2, 0., 360 * deg);
  G4LogicalVolume *logic_placcaRame_CilindricaUp = new G4LogicalVolume(placcaRame_Cilindrica, Cu, "placcaRame_CilindricaUp"); // solid, material, name
  physics_placcaRame_CilindricaUp = new G4PVPlacement(0,                                                 // no rotation
                                                                       G4ThreeVector(0., 0., delta + z_height_Source_biggerBaseSiDet_inVerticale + distanza_tra_BaseInfScintillatore_e_BordoSuperioreDeiSiDetector + thickness_z_SupportoRame_SiDetector + lenghtPlaccaCilidrica / 2),
                                                                       logic_placcaRame_CilindricaUp, "logic_placcaRame_CilindricaUp", // its fLogical volume
                                                                       fLogicWorld,                                                // its mother volume
                                                                       false,                                                      // no boolean op.
                                                                       0);

  logic_placcaRame_CilindricaUp->SetVisAttributes(Copper);
  if (physics_placcaRame_CilindricaUp == NULL)
  {
  }


  ////////////////////LOWER///////////////////////////
  G4LogicalVolume *fLogic_PlasticScintillatorLow = new G4LogicalVolume(fSolid_PlasticScintillator, fMaterial_PlasticScintillator, "PlasticScintillatorLow");                                                                                         // solid, material, name
  fPhys_PlasticScintillatorLow = new G4PVPlacement(0,                                                                                                                                                                              // rotationMatrix
                                                               G4ThreeVector(0., 0., -(z_height_Source_biggerBaseSiDet_inVerticale + distanza_tra_BaseInfScintillatore_e_BordoSuperioreDeiSiDetector + fLength_PlasticScintillator / 2)), // position
                                                               fLogic_PlasticScintillatorLow, "PlasticScintillatorLow",                                                                                                                              // its fLogical volume
                                                               fLogicWorld,                                                                                                                                                                    // its mother volume
                                                               false,                                                                                                                                                                          // no boolean op.
                                                               0);                                                                                                                                                                             // copy nb.
  fLogic_PlasticScintillatorLow->SetVisAttributes(PlasticScintillator_att);

  if (fPhys_PlasticScintillatorLow == NULL)
  {
  }

  G4LogicalVolume *logic_Box_material_SupportoRame_PlasticScintLow = new G4LogicalVolume(Box_material_SupportoRame_PlasticScint, Cu, "logic_Box_material_SupportoRame_PlasticScintLow"); // solid, material, name
  physics_Box_material_SupportoRame_PlasticScintLow = new G4PVPlacement(0,                                                                                         // no rotation
                                                                                        G4ThreeVector(0., 0., -(z_height_Source_biggerBaseSiDet_inVerticale + distanza_tra_BaseInfScintillatore_e_BordoSuperioreDeiSiDetector + thickness_z_SupportoRame_SiDetector / 2)),
                                                                                        logic_Box_material_SupportoRame_PlasticScintLow, "logic_Box_material_SupportoRame_PlasticScintLow", // its fLogical volume
                                                                                        fLogicWorld,                                                                                  // its mother volume
                                                                                        false,                                                                                        // no boolean op.
                                                                                        0);                                                                                           // copy nb.

  logic_Box_material_SupportoRame_PlasticScintLow->SetVisAttributes(Copper);
  

  if (physics_Box_material_SupportoRame_PlasticScintLow == NULL)
  {
  }

  G4LogicalVolume *logic_placcaRame_CilindricaLow = new G4LogicalVolume(placcaRame_Cilindrica, Cu, "placcaRame_CilindricaLow"); // solid, material, name
  physics_placcaRame_CilindricaLow = new G4PVPlacement(0,                                                 // no rotation
                                                                       G4ThreeVector(0., 0., -(z_height_Source_biggerBaseSiDet_inVerticale + distanza_tra_BaseInfScintillatore_e_BordoSuperioreDeiSiDetector + thickness_z_SupportoRame_SiDetector + lenghtPlaccaCilidrica / 2)),
                                                                       logic_placcaRame_CilindricaLow, "logic_placcaRame_CilindricaLow", // its fLogical volume
                                                                       fLogicWorld,                                                // its mother volume
                                                                       false,                                                      // no boolean op.
                                                                       0);

  logic_placcaRame_CilindricaLow->SetVisAttributes(Copper);
  if (physics_placcaRame_CilindricaLow == NULL)
  {
  }

  //////// SET SENSITIVE DETECTOR OTHER THAN SiDet and Catcher /////////
  fLogic_PlasticScintillatorLow->SetSensitiveDetector(manager_ptr->GetWoodsSensor_PlasticScintillatorLow());
  fLogic_PlasticScintillatorUp->SetSensitiveDetector(manager_ptr->GetWoodsSensor_PlasticScintillatorUp());




  return fPhysiWorld;
}

void Woods_Detector::SetBFieldValue(G4double value)
{
  G4FieldManager *pFieldMgr;
  // G4MagneticField *WoodsMagField = new WoodsMagnetField(GetInputNameB(), value); /// NON UNIFORM MAG FIELD GETFIELDVALUE method
  G4MagneticField *WoodsMagField = new G4UniformMagField(G4ThreeVector(0., 0., value));
  pFieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  G4ChordFinder *pChordFinder = new G4ChordFinder(WoodsMagField);
  pChordFinder->SetDeltaChord(1 * um);
  pFieldMgr->SetChordFinder(pChordFinder);
  pFieldMgr->SetDetectorField(WoodsMagField);
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void Woods_Detector::SetCatcherPosition_z(G4double value)
{
  phys_mother_catcher->SetTranslation(phys_mother_catcher->GetTranslation() + G4ThreeVector(0, 0, value));
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void Woods_Detector::SetCatcher_Thickness(G4double Al1_e, G4double Mylar_e, G4double Al2_e)
{
  G4int counter = 0;
  G4double shift = 0;
  G4double shift1 = 0;
  G4double shift2 = 0;

  if (Al1_e == -1.)
  {
    shift1 = 5*cm;
    Al1_e = DBL_MIN;
  }
  else
  {
    counter++;
    Physics_AlSource1_central->SetCopyNo(counter);
  }
  if (Mylar_e == -1.)
  {
    shift = 5*cm;
    Mylar_e = DBL_MIN;
  }
  else
  {
    counter++;
    Physics_MylarSource_central->SetCopyNo(counter);
  }
  if (Al2_e == -1.)
  {
    shift2 = 5*cm;
    Al2_e = DBL_MIN;
  }
  else
  {
    counter++;
    Physics_AlSource2_central->SetCopyNo(counter);
  }

    if (Catcher_Position == "catcher2")
    {
      Physics_AlSource1_central->SetTranslation(Physics_AlSource1_central->GetTranslation() + G4ThreeVector(0,shift1,-AlSource1 ->GetZHalfLength()+Al1_e/2));
      AlSource1 ->SetZHalfLength(Al1_e/2);
      G4RunManager::GetRunManager()->GeometryHasBeenModified();
      
      Physics_MylarSource_central->SetTranslation(Physics_AlSource1_central->GetTranslation() + G4ThreeVector(0,shift-shift1,Al1_e/2 + Mylar_e/2));
      MylarSource_central->SetZHalfLength(Mylar_e/2);
      G4RunManager::GetRunManager()->GeometryHasBeenModified();

      Physics_AlSource2_central->SetTranslation(Physics_MylarSource_central->GetTranslation() + G4ThreeVector(0,shift2-shift,Mylar_e/2 + Al2_e/2));
      AlSource2 ->SetZHalfLength(Al2_e/2);
    }

    else if (Catcher_Position == "catcher1")
    {
      Physics_AlSource1_side->SetTranslation(Physics_AlSource1_side->GetTranslation() + G4ThreeVector(0,shift1,-AlSource1 ->GetZHalfLength()+Al1_e/2));
      AlSource1 ->SetZHalfLength(Al1_e/2);
      G4RunManager::GetRunManager()->GeometryHasBeenModified();

      Physics_MylarSource_side->SetTranslation(Physics_AlSource1_side->GetTranslation() + G4ThreeVector(0,shift-shift1,Al1_e/2 + Mylar_e/2));
      MylarSource_side->SetZHalfLength(Mylar_e/2);
      G4RunManager::GetRunManager()->GeometryHasBeenModified();

      Physics_AlSource2_side->SetTranslation(Physics_MylarSource_side->GetTranslation() + G4ThreeVector(0,shift2-shift-shift1,Mylar_e/2 + Al2_e/2));
      AlSource2 ->SetZHalfLength(Al2_e/2);
    }

    else if (Catcher_Position == "source")
    {
      Physics_Source->SetTranslation(Physics_Source->GetTranslation() + G4ThreeVector(0, 0, 0));
    }
  
}

void Woods_Detector::SetCatcherPosition_theta(G4String position, G4double value)
{
  G4RotationMatrix *myRotation = new G4RotationMatrix();
  myRotation->rotateX(0. * deg);
  myRotation->rotateY(0. * deg);
  Catcher_Position = position;

  if (position == "source")
  {
    myRotation->rotateZ(30 * deg + value);
  }
  else if (position == "catcher2")
  {
    myRotation->rotateZ(value);
  }
  else if (position == "catcher1")
  {
    myRotation->rotateZ(-22.5 * deg + value);
  }
  else
  {
    G4Exception("Woods_Detector", ("Wrong Catcher position : " + position).c_str(), JustWarning, "");
  }

  phys_mother_catcher->SetRotation(myRotation);
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void Woods_Detector::SetDetector_Radius(G4double value)
{
  fSolid_PlasticScintillator->SetOuterRadius(value);
  placcaRame_Cilindrica->SetInnerRadius(value);
  placcaRame_Cilindrica->SetOuterRadius(value+thickness_z_SupportoRame_PlasticScint/2);

  holeForPlasticScint->SetOuterRadius(value);
  Box_material_SupportoRame_PlasticScint = new G4SubtractionSolid("Box_material_SupportoRame_PlasticScint",
                                                                            bigBox_material_SupportoRame_PlasticScint, holeForPlasticScint,
                                                                            0,
                                                                            G4ThreeVector(0., 0., 0.));

  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void Woods_Detector::SetDetector_Thickness(G4double value)
{
  fSolid_PlasticScintillator->SetZHalfLength(value/2);
  fPhys_PlasticScintillatorUp->SetTranslation(fPhys_PlasticScintillatorUp->GetTranslation() + G4ThreeVector(0., 0., -(fLength_PlasticScintillator-value) / 2));
  fPhys_PlasticScintillatorLow->SetTranslation(fPhys_PlasticScintillatorLow->GetTranslation() + G4ThreeVector(0., 0., (fLength_PlasticScintillator-value) / 2));

  placcaRame_Cilindrica->SetZHalfLength(value/2);
  physics_placcaRame_CilindricaUp->SetTranslation(physics_placcaRame_CilindricaUp->GetTranslation() + G4ThreeVector(0., 0., -(fLength_PlasticScintillator-value) / 2));
  physics_placcaRame_CilindricaLow->SetTranslation(physics_placcaRame_CilindricaLow->GetTranslation() + G4ThreeVector(0., 0., (fLength_PlasticScintillator-value) / 2));


  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}
    

void Woods_Detector::SetDetector_Distance(G4double value)
{
  fPhys_PlasticScintillatorUp->SetTranslation(G4ThreeVector(0, 0, value+fSolid_PlasticScintillator->GetZHalfLength()));
  fPhys_PlasticScintillatorLow->SetTranslation(G4ThreeVector(0, 0, -value-fSolid_PlasticScintillator->GetZHalfLength()));

  physics_placcaRame_CilindricaUp->SetTranslation(G4ThreeVector(0, 0, value+placcaRame_Cilindrica->GetZHalfLength()));
  physics_placcaRame_CilindricaLow->SetTranslation(G4ThreeVector(0, 0, -value-placcaRame_Cilindrica->GetZHalfLength()));

  physics_Box_material_SupportoRame_PlasticScintLow->SetTranslation(G4ThreeVector(0, 0, -value-thickness_z_SupportoRame_PlasticScint/2));
  physics_Box_material_SupportoRame_PlasticScintUp->SetTranslation(G4ThreeVector(0, 0, value+thickness_z_SupportoRame_PlasticScint/2));


  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}