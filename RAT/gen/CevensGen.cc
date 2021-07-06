#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Neutron.hh"
#include "G4Proton.hh"
#include "G4IonTable.hh"
#include "G4Navigator.hh"
#include "G4Material.hh"
#include "G4TransportationManager.hh"
#include "Randomize.hh"
#include "G4PhysicalConstants.hh"

#include "marley/marley_utils.hh"
#include "marley/JSON.hh"
#include "marley/JSONConfig.hh"
#include "marley/Generator.hh"
#include "marley/Logger.hh"

#include "nlohmann/json.hpp"

#include "CENNSPrimaryGeneratorAction.hh"
#include "CENNSPrimaryGeneratorMessenger.hh"

#include "G4SystemOfUnits.hh"

#include <cstdlib>
#include <chrono>
#include <sstream>

CENNSPrimaryGeneratorAction::CENNSPrimaryGeneratorAction():
  G4VUserPrimaryGeneratorAction(), fParticleGun()
{ 
  fParticleGun = new G4GeneralParticleSource();  
  fGunMessenger = new CENNSPrimaryGeneratorMessenger(this);
  fGenerator = " ";
  fNuFlavor = " ";
  fXCosmic = 0;
  fYCosmic = 0;
  fZCosmic = 0;
  fRadCosmic = 0;
  fPartGenerator = new G4ParticleGun();

  // marley generator
  _MarleyJSON_filepath = std::string(std::getenv("CENNS10_DATADIR")) + "/marley_dar.json"; // default
  _MarleyJSON = new marley::JSON;
  _MarleyConfig = nullptr;
  _MarleyGen = nullptr;  
  _MarleyNumGen = 0;
  
  _json_event_file  = "__not_specified__";
  _json_nevents_gen = 0;
  
}

CENNSPrimaryGeneratorAction::~CENNSPrimaryGeneratorAction()
{ 
  delete fParticleGun; 
}

void CENNSPrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{ 
  //Use standard Geant4 gps commands to create particle sources
  if(fGenerator == "gps" || fGenerator == "GeneralParticleSource")
    fParticleGun->GeneratePrimaryVertex(event); 
  //Use CEvNS generator I created
  if(fGenerator == "CEvNS")
    GenerateCEvNS(event);
  //Use cosmic muon generator I created
  if(fGenerator == "cosmicMuon")
    GenerateCosmics(event);
  if(fGenerator == "83mKr")
    Generate83mKr(event);
  //Use Marley interface
  if(fGenerator == "marley")
    GenerateMarley(event);
  if (fGenerator=="json")
    GenerateFromJSON(event);
}
      

void CENNSPrimaryGeneratorAction::SetGenerator(G4String generator)
{
  if(generator == "gps" || generator == "CEvNS" || generator == "cosmicMuon" || generator == "83mKr" || generator=="marley" || generator=="json" )
    fGenerator = generator;
}

void CENNSPrimaryGeneratorAction::SetNuFlavor(G4String flavor)
{
  fNuFlavor = flavor;
}

void CENNSPrimaryGeneratorAction::SetXCosmicPos(G4double xpos)
{
  fXCosmic = xpos;
}

void CENNSPrimaryGeneratorAction::SetYCosmicPos(G4double ypos)
{
  fYCosmic = ypos;
}

void CENNSPrimaryGeneratorAction::SetZCosmicPos(G4double zpos)
{
  fZCosmic = zpos;
}

void CENNSPrimaryGeneratorAction::SetRadCosmic(G4double radCosmic)
{
  fRadCosmic = radCosmic;
}

void CENNSPrimaryGeneratorAction::Generate83mKr(G4Event* event)
{
  G4double phi = 2*pi*G4UniformRand();
  G4double costhe = 1.0 - 2.0*G4UniformRand();
  G4double sinthe = sqrt(1.0 - costhe*costhe);
  //G4ParticleDefinition* krypton1 = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
  fPartGenerator->SetParticleDefinition(G4Gamma::Gamma());
  G4ThreeVector genPos, momentum;
  G4double xmom = sinthe*cos(phi);
  G4double ymom = sinthe*sin(phi);
  G4double zmom = costhe;
  //G4double nutime = SampleNuTime();
  GetPositionInLAr(genPos, 14.5*25.4, 14.5*25.4, 4.5*25.4);
  //SetParticleMomentum(momentum, ArAngle, phi);                               
  //if(fNuFlavor == "nu_e" || fNuFlavor == "anti_nu_mu")                       
  //fPartGenerator->SetParticleTime(-2.2*std::log(G4UniformRand())*microsecond);                                                                             
  //first gamma
  fPartGenerator->SetParticleEnergy(0.03215*MeV);
  fPartGenerator->SetParticleMomentumDirection(G4ThreeVector(xmom,ymom,zmom));
  fPartGenerator->SetParticlePosition(genPos);
  fPartGenerator->SetParticleTime(0.0*ns);
  fPartGenerator->GeneratePrimaryVertex(event);

  fPartGenerator->SetParticleDefinition(G4Gamma::Gamma());

  phi = 2*pi*G4UniformRand();
  costhe = 1.0 - 2.0*G4UniformRand();
  sinthe = sqrt(1.0 - costhe*costhe);
  G4double xmom2 = sinthe*cos(phi);
  G4double ymom2 = sinthe*sin(phi);
  G4double zmom2 = costhe;
  fPartGenerator->SetParticleEnergy(0.009406*MeV);
  fPartGenerator->SetParticleMomentumDirection(G4ThreeVector(xmom2,ymom2,zmom2));
  //fPartGenerator->SetParticlePosition(genPos);
  //fPartGenerator->SetParticleTime(-2.2*std::log(G4UniformRand())*microsecond);  
  fPartGenerator->SetParticleTime(-156.0*std::log(G4UniformRand())*ns);
  fPartGenerator->GeneratePrimaryVertex(event);
}

void CENNSPrimaryGeneratorAction::GenerateCEvNS(G4Event* event)
{
  G4double phi = 2*pi*G4UniformRand();
  G4double costhe = 1.0 - 2.0*G4UniformRand(); //choosing the CM angle of scattering process 
  G4double sinthe = sqrt(1 - costhe*costhe);
  fNuFlavor = "anti_nu_mu";
  //G4double random = G4UniformRand();
  //if(random <= 0.02)
  //fNuFlavor = "nu_mu";
  //if(random > 0.02 && random < 0.52)
  //fNuFlavor = "nu_e";
  //if(random > 0.52)
  //fNuFlavor = "anti_nu_mu";
  //G4double energy_nu = PullNuEnergy(fNuFlavor);  
  G4double energy_nu = SampleNuEnergy();
  G4int Z  = 18;
  G4int A = 40;
  G4ParticleDefinition* Argon = G4IonTable::GetIonTable()->GetIon(Z, A, 0.0*keV);
  fPartGenerator->SetParticleDefinition(Argon);

  //G4double nu_angle = atan(sinthe/(costhe + energy_nu/Argon->GetPDGMass()/MeV));
  //G4double ArAngle = atan(sinthe/(1-costhe));
  //G4double recoil_en = (2*energy_nu*energy_nu/Argon->GetPDGMass()/MeV)*cos(nu_angle)*cos(nu_angle);
  G4ThreeVector genPos, momentum;
  G4double xmom = sinthe*cos(phi);
  G4double ymom = sinthe*sin(phi);
  G4double zmom = costhe;
  G4double nutime = SampleNuTime();
  GetPositionInLAr(genPos, 14.5*25.4, 14.5*25.4, 14.5*25.4 );
  //SetParticleMomentum(momentum, ArAngle, phi);

  //if(fNuFlavor == "nu_e" || fNuFlavor == "anti_nu_mu")
    //fPartGenerator->SetParticleTime(-2.2*std::log(G4UniformRand())*microsecond);
  fPartGenerator->SetParticleEnergy(energy_nu*MeV);
  fPartGenerator->SetParticleMomentumDirection(G4ThreeVector(xmom,ymom,zmom));
  fPartGenerator->SetParticlePosition(genPos);
  fPartGenerator->SetParticleTime(nutime*ns);
  fPartGenerator->GeneratePrimaryVertex(event);
  
  
}

G4double CENNSPrimaryGeneratorAction::SampleNuEnergy()
{
  std::string recoilfilepath= std::string( std::getenv("CENNS10_DATADIR"))+"/Recoil_Spectra_BERT_Ar-40_noConv.root";
  TFile *recoilfile = new TFile(recoilfilepath.c_str());
  TH2D *energydist = (TH2D*)recoilfile->Get("h_dNdT_NuMuBar");
  TH1D *energyspect = energydist->ProjectionY();
  int nbins = energyspect->GetNbinsX();
  //G4cout << nbins << G4endl;
  double probSum = 0;
  //energyspect->SetBinContent(0,0.0);
  energyspect->Scale(1.0/energyspect->Integral());
  for(int j = 0; j < nbins; ++j)
  {
    probSum += energyspect->GetBinContent(j);
    if(energyspect->GetBinContent(j) > 0)
      energyspect->SetBinContent(j,probSum);
    //G4cout << probSum << " " << energyspect->GetBinContent(j) << G4endl;
  }
  double rand = G4UniformRand();
  for(int k = 1; k < nbins; ++k)
    if(rand >= energyspect->GetBinContent(k-1) && rand <= energyspect->GetBinContent(k))
    {
      G4double energyval = energyspect->GetXaxis()->GetBinCenter(k) + energyspect->GetXaxis()->GetBinWidth(k)*(G4UniformRand()- 0.5);
      //while(energyval < 0.0)
      //energyval = energyspect->GetXaxis()->GetBinCenter(k) + energyspect->GetXaxis()->GetBinWidth(k)*(G4UniformRand()- 0.5);
      recoilfile->Close();
      delete recoilfile;
      //G4cout << energyval << " " << k << G4endl;
      return energyval;
    }
  recoilfile->Close();
  delete recoilfile;
  return 0.0; //if get here something wrong     
}

G4double CENNSPrimaryGeneratorAction::SampleNuTime()
{
  TFile *recoilfile = new TFile( std::string( std::getenv("CENNS10_DATADIR")+std::string("/Recoil_Spectra_BERT_Ar-40_noConv.root") ).c_str() );
  TH2D *timedist = (TH2D*)recoilfile->Get("h_dNdT_NuMuBar");
  TH1D *timespect = timedist->ProjectionX();
  int nbins = timespect->GetNbinsX();
  //G4cout << nbins << G4endl;                                    
  double probSum = 0;                             
  timespect->Scale(1.0/timespect->Integral());
  for(int j = 0; j < nbins; ++j)
    {                                           
      probSum += timespect->GetBinContent(j);
      if(timespect->GetBinContent(j) > 0)
	timespect->SetBinContent(j,probSum);
      //G4cout << probSum << " " << timespect->GetBinContent(j) << G4endl;
    }
  double rand = G4UniformRand();
  //G4cout << rand << G4endl;
  for(int k = 1; k < nbins; ++k)
    if(rand >= timespect->GetBinContent(k-1) && rand <= timespect->GetBinContent(k))
      {
	G4double timeval = timespect->GetXaxis()->GetBinCenter(k) + timespect->GetXaxis()->GetBinWidth(k)*(G4UniformRand() - 0.5);
	//if(timeval < 0.0)
	//timeval = 0.0;
	recoilfile->Close();
	delete recoilfile;
	//G4cout << timeval << " " << k << G4endl;
	return timeval;
      }
  recoilfile->Close();
  delete recoilfile;
  return 0.0; //if get here something wrong                  
}

void CENNSPrimaryGeneratorAction::SetParticleMomentum(G4ThreeVector& momentum, G4double ar_angle, G4double phi)
{
  G4double xmom = sin(ar_angle)*cos(phi);
  G4double ymom = sin(ar_angle)*sin(phi);
  G4double zmom = cos(ar_angle);
  momentum.set(xmom,ymom,zmom);
}

void CENNSPrimaryGeneratorAction::GetPositionInLAr(G4ThreeVector& genpos, G4double xmax, G4double ymax, G4double zmax, bool cylindrical_vol )
{
  G4Navigator* Navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  G4Material* lAr = G4Material::GetMaterial("LArgon");
  if ( !lAr ) {
    throw std::runtime_error("[CENNSPrimaryGeneratorAction::GetPositionInLAr] No material with the name 'LArgon' is defined");
  }
  
  while(true)
  {
    G4double zpos = (2.0*G4UniformRand() - 1.0)*zmax;
    G4double xpos = (2.0*G4UniformRand() - 1.0)*xmax;
    G4double ypos = (2.0*G4UniformRand() - 1.0)*ymax;

    if ( cylindrical_vol && ( fabs(xpos)>xmax || fabs(ypos)>ymax ) ) {
      G4cout << " redraw not in cylidner. (x,y)>(xmax,ymax) = (" << xpos << "," << ypos << ") > (" << xmax << "," << ymax << ")" << G4endl;
      continue; // try again
    }
    
    genpos.set(xpos*mm,ypos*mm,zpos*mm);
   
    G4VPhysicalVolume* volume = Navigator->LocateGlobalPointAndSetup(genpos);

    G4String matname = volume->GetLogicalVolume()->GetMaterial()->GetName();
    if( matname == "LArgon") {
      return;
    }
    G4cout << "redraw not in LAr material "  << matname << G4endl;    
  }
  return;
  
}

G4double CENNSPrimaryGeneratorAction::PullNuEnergy(G4String nuFlavor)
{ 
  if(nuFlavor == "nu_mu")
    return 29.9; //100% prompt nu_mu at 29.9 MeV
  
  if(nuFlavor == "nu_e")
  {
    G4double rand = G4UniformRand();
    const G4int numEntries = 53;
    G4double nuEnergy[numEntries] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0,
				     6.0, 7.0, 8.0, 9.0, 10.0, 11.0,
				     12.0, 13.0, 14.0, 15.0, 16.0, 17.0,
				     18.0, 19.0, 20.0, 21.0, 22.0, 23.0,
				     24.0, 25.0, 26.0, 27.0, 28.0, 29.0,
				     30.0, 31.0, 32.0, 33.0, 34.0, 35.0,
				     36.0, 37.0, 38.0, 39.0, 40.0, 41.0,
				     42.0, 43.0, 44.0, 45.0, 46.0, 47.0,
				     48.0, 49.0, 50.0, 51.0, 52.0 };
    G4double fluxProb[numEntries] = {0.0, 0.0005, 0.001, 0.0015, 0.002, 0.0025,
				 0.003, 0.0035, 0.004, 0.005, 0.006,
				 0.007, 0.008, 0.0095, 0.011, 0.0125,
				 0.014, 0.016, 0.018, 0.0195, 0.021, 
				 0.0225, 0.024, 0.0255, 0.027, 0.028,
				 0.029, 0.0295, 0.03, 0.0305, 0.031, 
				 0.0315, 0.032, 0.0325, 0.033, 0.0325,
				 0.032, 0.0315, 0.031, 0.0305, 0.03,
				 0.029, 0.028, 0.0265, 0.025, 0.023,
				 0.021, 0.018, 0.015, 0.012, 0.009,
				 0.004, 0.0009 };
    G4double runningSum = 0.0;
    for(G4int i = 0; i < numEntries; ++i)
      runningSum += fluxProb[i];
    for(G4int i = 1; i < numEntries; ++i)
    {
      G4double val = fluxProb[i]/runningSum;
      fluxProb[i] = fluxProb[i-1] + val;
    }
    for(G4int i = 0; i < numEntries; ++i)
    {
      if(rand >= fluxProb[i] && rand <= fluxProb[i+1])
	return nuEnergy[i] + G4UniformRand();
    }
  }
  
  if(nuFlavor = "anti_nu_mu")
  {
    G4double rand = G4UniformRand();
    const G4int numEntries = 53;
    G4double nuEnergy[numEntries] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0,
                                     6.0, 7.0, 8.0, 9.0, 10.0, 11.0,
                                     12.0, 13.0, 14.0, 15.0, 16.0, 17.0,
                                     18.0, 19.0, 20.0, 21.0, 22.0, 23.0,
                                     24.0, 25.0, 26.0, 27.0, 28.0, 29.0,
                                     30.0, 31.0, 32.0, 33.0, 34.0, 35.0,
                                     36.0, 37.0, 38.0, 39.0, 40.0, 41.0,
                                     42.0, 43.0, 44.0, 45.0, 46.0, 47.0,
                                     48.0, 49.0, 50.0, 51.0, 52.0 };
    G4double fluxProb[numEntries] = {0.0, 0.00025, 0.0005, 0.00075, 0.001, 0.00125,
				     0.0015, 0.00175, 0.002, 0.0025, 0.003,
				     0.0035, 0.004, 0.005, 0.006, 0.007,
				     0.008, 0.009, 0.01, 0.011, 0.012,
				     0.013, 0.014, 0.015, 0.016, 0.017,
				     0.018, 0.019, 0.02, 0.021, 0.022,
				     0.023, 0.024, 0.025, 0.026, 0.0265,
				     0.027, 0.028, 0.029, 0.03, 0.031,
				     0.0315, 0.032, 0.0325, 0.033, 0.034,
				     0.035, 0.0355, 0.036, 0.0365, 0.037,
				     0.037, 0.037 };
    G4double runningSum = 0.0;
    for(G4int i = 0; i < numEntries; ++i)
      runningSum += fluxProb[i];
    for(G4int i = 0; i < numEntries; ++i)
    {
      G4double val = fluxProb[i]/runningSum;
      fluxProb[i] = fluxProb[i-1] + val;
    }
    for(G4int i = 0; i < numEntries; ++i)
    {
      if(rand >= fluxProb[i] && rand <= fluxProb[i+1])
	return nuEnergy[i] + G4UniformRand();
    }
  }
  
  return 0.0;
}

void CENNSPrimaryGeneratorAction::GenerateCosmics(G4Event* event)
{
  G4double phi = 2*pi*G4UniformRand();
  G4double costhe = G4UniformRand();
  //G4double costhe = PullAngDist(); //Miyake 8 m.w.e
  G4double sinthe = sqrt(1 - costhe*costhe);
  G4ParticleDefinition* cosmic = G4ParticleTable::GetParticleTable()->FindParticle("mu-");
  fPartGenerator->SetParticleDefinition(cosmic);
  
  G4double xmom = cos(phi)*sinthe;
  G4double ymom = sin(phi)*sinthe;
  G4double zmom = costhe;
  //G4double muEnergy = PullMiyake(); //Miyake w/ 8 m.w.e (until better)

  G4ThreeVector genPos;
  GetCosmicPosition(genPos);
  fPartGenerator->SetParticleEnergy(5.0*MeV);
  fPartGenerator->SetParticleMomentumDirection(G4ThreeVector(xmom,ymom,-zmom));
  fPartGenerator->SetParticlePosition(genPos);
  fPartGenerator->GeneratePrimaryVertex(event);
  
}

void CENNSPrimaryGeneratorAction::GetCosmicPosition(G4ThreeVector& genPos)
{
  G4double xpos, ypos, zpos;
  zpos = fZCosmic;
  while(true)
  {
    ypos = G4UniformRand()*fRadCosmic;
    xpos = G4UniformRand()*fRadCosmic;
    if(xpos*xpos + ypos*ypos < fXCosmic*fXCosmic + fYCosmic*fYCosmic)
    {
      genPos.set(xpos*cm,ypos*cm,zpos*cm);
      return;
    }
  }
  return;
}

void CENNSPrimaryGeneratorAction::GenerateMarley(G4Event* event) {

  std::cout << "[CENNSPrimaryGeneratorAction::GenerateMarleyEvent]" << std::endl;
  
  if ( !_MarleyGen ) {

    // Parse the objects from the JSON-based configuration file
    (*_MarleyJSON) = marley::JSON::load_file( _MarleyJSON_filepath );

    // Process them to get the generator configuration. 
    _MarleyConfig = new marley::JSONConfig( *_MarleyJSON );

    // initialize the logger for marley
    marley::Logger::Instance().add_stream(std::cout,
      marley::Logger::LogLevel::INFO);

    // This std::unique_ptr to a Generator object will be initialized below
    // with the proper configuration.
    _MarleyGen = new marley::Generator( _MarleyConfig->create_generator() );
  }

  // get the volume info from the same jason file
  marley::JSON jvolume = _MarleyJSON->at("volume");


  G4ThreeVector genpos;
  if ( jvolume.has_key("r") ) {
    // cylindrical throw
    G4cout << "  Gen in cylinder (R,Z)=(" << jvolume.at("r").to_double() << "," << jvolume.at("z").to_double() << G4endl;
    GetPositionInLAr( genpos, jvolume.at("r").to_double(), jvolume.at("r").to_double(), jvolume.at("z").to_double(), true );
  }
  else {
    G4cout << "  Gen in box (X,Y,Z)=(" << jvolume.at("x").to_double() << "," << jvolume.at("y").to_double() << "," << jvolume.at("z").to_double() << ")" << G4endl;    
    GetPositionInLAr( genpos, jvolume.at("x").to_double(), jvolume.at("y").to_double(), jvolume.at("z").to_double(), false );
  }
  std::cout << "  Gen position: " << genpos << std::endl;
  
  //marley::JSON ex_set = _MarleyJSON->get_object("executable_settings");

  // Generate all of the requested events. End the loop early
  // if the user interrupts execution (e.g., via ctrl+C)
  
  // Create an event using the generator object
  marley::Event marley_event = _MarleyGen->create_event();
  _MarleyNumGen++;

  // Get Pariticle List
  const std::vector<marley::Particle*>& finalstate = marley_event.get_final_particles();

  for ( auto const& pparticle : finalstate ) {
    // shoot the particle gun for each final state particle
    
    int pdg = pparticle->pdg_code();
    G4double ke = (pparticle->total_energy() - pparticle->mass())*MeV;
    std::cout << " [pdg " << pdg << "] KE=" << ke << std::endl;
    
    if ( abs(pdg)==12 || abs(pdg)==14 || abs(pdg)==16 ) {
      // skip the final-state neutrino (if there is one)
      continue;
    }

    //G4double energy = pparticle->total_energy()*MeV;
    // if ( energy>2.0 ) {
    //   continue;
    // }

    if ( pdg>1000000000 ) {
      // nuclei
      int A = pdg/10%1000;
      int Z = pdg/10000%1000;
      G4ParticleDefinition* recoil_nucleus = G4IonTable::GetIonTable()->GetIon(Z, A, 0.0*keV);
      fPartGenerator->SetParticleDefinition(recoil_nucleus);
    }
    else {
      // particles
      std::stringstream msg;
      switch ( pdg ) {
      case 11:
        fPartGenerator->SetParticleDefinition( G4Electron::Electron() );
        break;
      case -11:
        fPartGenerator->SetParticleDefinition( G4Positron::Positron() );
        break;
      case 22:
        fPartGenerator->SetParticleDefinition( G4Gamma::Gamma() );
        break;
      case 2212:
        fPartGenerator->SetParticleDefinition( G4Proton::Proton() );
        break;        
      case 2112:
        fPartGenerator->SetParticleDefinition( G4Neutron::Neutron() );
        break;        
      default:
        msg << "[CENNSPrimaryGeneratorAction::GenerateMarley] unrecongnized PDG = " << pdg << std::endl;
        throw std::runtime_error( msg.str() );
        break;
      }
    }//end of if nucleus

    fPartGenerator->SetParticleEnergy( ke );
    G4ParticleMomentum mom( pparticle->px()*MeV, pparticle->py()*MeV, pparticle->pz()*MeV );
    fPartGenerator->SetParticleMomentumDirection( mom );
    fPartGenerator->SetParticleCharge( pparticle->charge() );
    fPartGenerator->SetParticlePosition( genpos );

    // add particle to event
    fPartGenerator->GeneratePrimaryVertex(event);
  }
  
  // Print status messages about simulation progress after every 100
  // events have been generated
  if (_MarleyNumGen % 100 == 1 ) {
    // Print a status message showing the current number of events
    std::cout << "Marley Event Count = " << _MarleyNumGen << std::endl;
  }

}


void CENNSPrimaryGeneratorAction::GenerateFromJSON(G4Event* event) {

  std::cout << "[CENNSPrimaryGeneratorAction::GenerateFromJSON]" << std::endl;
  std::cout << "inputfile: " << _json_event_file << std::endl;

  // load json file
  std::ifstream i( _json_event_file.c_str() );

  // load json object
  // why do we have a second implementation of json?
  // This one (nlohmann) has a very standard library-like interface, which is nice.
  // Marley had their own. oh well.
  nlohmann::json j;
  try {
    i >> j;
  }
  catch (const std::exception& e ) {
    std::stringstream msg;
    msg << "[" << __FUNCTION__ << ".L" << __LINE__ << "] error parsing file: " << e.what() << std::endl;
    throw std::runtime_error(msg.str());
  }

  // get event list
  if ( j.find("events")==j.end() ) {
    std::string msg = "Did not find 'events' key in JSON file";
    throw std::runtime_error(msg);
  }

  int nevents_in_file = j["events"].size();

  // get the event
  int ievent = _json_nevents_gen;

  if ( ievent>=nevents_in_file ) {
    std::stringstream msg;
    msg << "Ran out of events. Attempting to load eventid=" << ievent << " but only " << nevents_in_file << std::endl;
    throw std::runtime_error(msg.str());
  }

  nlohmann::json jevent = j["events"].at(ievent);


  // get event list
  if ( j.find("volume")==j.end() ) {
    std::string msg = "Did not find 'volume' key in JSON file";
    throw std::runtime_error(msg);
  }
  
  nlohmann::json jvolume = j["volume"];

  G4ThreeVector genpos;
  if ( jvolume.find("r")!=jvolume.end() ) {
    // radius defined in 
    // cylindrical throw
    G4cout << "  Gen in cylinder (R,Z)=(" << jvolume["r"].get<double>() << "," << jvolume["z"].get<double>() << G4endl;
    GetPositionInLAr( genpos, jvolume["r"].get<double>(), jvolume["r"].get<double>(), jvolume["z"].get<double>(), true );
  }
  else if ( jvolume.find("x")!=jvolume.end() ) {
    G4cout << "  Gen in box (X,Y,Z)=(" << jvolume["x"].get<double>() << "," << jvolume["y"].get<double>() << "," << jvolume["z"].get<double>() << ")" << G4endl;    
    GetPositionInLAr( genpos, jvolume["x"].get<double>(), jvolume["y"].get<double>(), jvolume["z"].get<double>(), false );
  }
  else {
    std::string msg = "did not find ('r','z') nor ('x','y','z') keys in 'volume' block. needed to generate position";
    throw std::runtime_error(msg);
  }
  
  G4cout << "[CENNSPrimaryGeneratorAction::" << __FUNCTION__<< "] Generated position: " << genpos << G4endl;
  _json_nevents_gen++;

  // Get Pariticle List
  if ( jevent.find("final_particles")==jevent.end() ) {
    std::string msg = "did not find key 'final_particles' in 'event' block";
    throw std::runtime_error(msg);
  }

  size_t num_particles = jevent["final_particles"].size();
  G4cout << "[CENNSPrimaryGeneratorAction::" << __FUNCTION__<< "]  Number of particles: " << num_particles << G4endl;
  

  for ( size_t iparticle=0; iparticle<num_particles; iparticle++ ) {

    nlohmann::json jparticle = jevent["final_particles"].at(iparticle);
    
    // shoot the particle gun for each final state particle
    
    int pdg = jparticle["pdg"].get<int>();
    G4double energy = jparticle["E"].get<double>()*MeV;
    G4double mass   = jparticle["mass"].get<double>()*MeV;
    G4double px     = jparticle["px"].get<double>()*MeV;
    G4double py     = jparticle["py"].get<double>()*MeV;
    G4double pz     = jparticle["pz"].get<double>()*MeV;

    G4double ke = (energy-mass);
    std::cout << " [" << iparticle << "] pdg=" << pdg << " KE=" << ke << std::endl;
    
    if ( abs(pdg)==12 || abs(pdg)==14 || abs(pdg)==16 ) {
      // skip the final-state neutrino (if there is one)
      continue;
    }

    if ( pdg>1000000000 ) {
      // nuclei
      int A = pdg/10%1000;
      int Z = pdg/10000%1000;
      G4ParticleDefinition* recoil_nucleus = G4IonTable::GetIonTable()->GetIon(Z, A, 0.0*keV);
      fPartGenerator->SetParticleDefinition(recoil_nucleus);
    }
    else {
      // particles
      std::stringstream msg;
      switch ( pdg ) {
      case 11:
        fPartGenerator->SetParticleDefinition( G4Electron::Electron() );
        break;
      case -11:
        fPartGenerator->SetParticleDefinition( G4Positron::Positron() );
        break;
      case 22:
        fPartGenerator->SetParticleDefinition( G4Gamma::Gamma() );
        break;
      case 2212:
        fPartGenerator->SetParticleDefinition( G4Proton::Proton() );
        break;        
      case 2112:
        fPartGenerator->SetParticleDefinition( G4Neutron::Neutron() );
        break;        
      default:
        msg << "[CENNSPrimaryGeneratorAction::GenerateMarley] unrecongnized PDG = " << pdg << std::endl;
        throw std::runtime_error( msg.str() );
        break;
      }
    }//end of if nucleus

    fPartGenerator->SetParticleEnergy( ke );
    G4ParticleMomentum mom( px, py, pz );
    fPartGenerator->SetParticleMomentumDirection( mom );
    fPartGenerator->SetParticlePosition( genpos );

    // add particle to event
    fPartGenerator->GeneratePrimaryVertex(event);
  }
  
  // Print status messages about simulation progress after every 100
  // events have been generated
  if (_json_nevents_gen % 100 == 1 ) {
    // Print a status message showing the current number of events
    G4cout << "[CENNSPrimaryGeneratorAction::" << __FUNCTION__ << "] Event Count = " << _json_nevents_gen << G4endl;
  }

}
