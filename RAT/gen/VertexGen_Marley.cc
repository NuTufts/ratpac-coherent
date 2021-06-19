#include "VertexGen_Marley.hh"

#include "G4PrimaryVertex.hh"
#include "G4Event.hh"

#include "RAT/base/Log.hh"

#include "marley/marley_utils.hh"
#include "marley/JSON.hh"
#include "marley/JSONConfig.hh"
#include "marley/Generator.hh"
#include "marley/Logger.hh"

//#include "nlohmann/json.hpp"

#include "G4SystemOfUnits.hh"

#include <cstdlib>
#include <chrono>
#include <sstream>

namespace RAT {

VertexGen_Marley::VertexGen_Marley(const char* arg_dbname)
  : GLG4VertexGen(arg_dbname),
    _MarleyGen(nullptr),
    _MarleyNumGen(0)
{
  // marley generator
  _MarleyJSON_filepath = std::string(std::getenv("GLG4DATA")) + "/marley_dar.json"; // default
  _MarleyJSON = new marley::JSON;
  _MarleyConfig = nullptr;
  
  _json_event_file  = "__not_specified__";
  _json_nevents_gen = 0;
  
}

VertexGen_Marley::~VertexGen_Marley()
{}

void VertexGen_Marley::GeneratePrimaryVertex(G4Event *argEvent,
                                             G4ThreeVector &dx,
                                             G4double dt)
{

  info << "[VertexGen_Marley::GeneratePrimaryVertex] start" << newline;
  
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

  // get the volume info from the same json file
  marley::JSON jvolume = _MarleyJSON->at("volume");
  
  // use passed in position and time
  G4PrimaryVertex* vertex= new G4PrimaryVertex(dx, dt);
  
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
    
    G4PrimaryParticle* genparticle
      = new G4PrimaryParticle( pdg,
                               pparticle->px()*MeV,
                               pparticle->py()*MeV,
                               pparticle->pz()*MeV );
    genparticle->SetMass( pparticle->mass()*MeV );
    vertex->SetPrimary( genparticle );
    
    // if ( pdg>1000000000 ) {
    //   // nuclei
    //   int A = pdg/10%1000;
    //   int Z = pdg/10000%1000;
    //   G4ParticleDefinition* recoil_nucleus = G4IonTable::GetIonTable()->GetIon(Z, A, 0.0*keV);
    //   fPartGenerator->SetParticleDefinition(recoil_nucleus);
    // }
    // else {
    //   // particles
    //   std::stringstream msg;
    //   switch ( pdg ) {
    //   case 11:
    //     fPartGenerator->SetParticleDefinition( G4Electron::Electron() );
    //     break;
    //   case -11:
    //     fPartGenerator->SetParticleDefinition( G4Positron::Positron() );
    //     break;
    //   case 22:
    //     fPartGenerator->SetParticleDefinition( G4Gamma::Gamma() );
    //     break;
    //   case 2212:
    //     fPartGenerator->SetParticleDefinition( G4Proton::Proton() );
    //     break;        
    //   case 2112:
    //     fPartGenerator->SetParticleDefinition( G4Neutron::Neutron() );
    //     break;        
    //   default:
    //     msg << "[CENNSPrimaryGeneratorAction::GenerateMarley] unrecongnized PDG = " << pdg << std::endl;
    //     throw std::runtime_error( msg.str() );
    //     break;
    //   }
    // }//end of if nucleus

    // fPartGenerator->SetParticleEnergy( ke );

    // fPartGenerator->SetParticleMomentumDirection( mom );
    // fPartGenerator->SetParticleCharge( pparticle->charge() );
    // fPartGenerator->SetParticlePosition( genpos );

    // add particle to event
    //fPartGenerator->GeneratePrimaryVertex(event);
  }//loop over final state particles

  argEvent->AddPrimaryVertex( vertex );
  
  // Print status messages about simulation progress after every 100
  // events have been generated
  if (_MarleyNumGen % 100 == 1 ) {
    // Print a status message showing the current number of events
    std::cout << "Marley Event Count = " << _MarleyNumGen << std::endl;
  }
  
}


// void CENNSPrimaryGeneratorAction::GenerateFromJSON(G4Event* event) {

//   std::cout << "[CENNSPrimaryGeneratorAction::GenerateFromJSON]" << std::endl;
//   std::cout << "inputfile: " << _json_event_file << std::endl;

//   // load json file
//   std::ifstream i( _json_event_file.c_str() );

//   // load json object
//   // why do we have a second implementation of json?
//   // This one (nlohmann) has a very standard library-like interface, which is nice.
//   // Marley had their own. oh well.
//   nlohmann::json j;
//   try {
//     i >> j;
//   }
//   catch (const std::exception& e ) {
//     std::stringstream msg;
//     msg << "[" << __FUNCTION__ << ".L" << __LINE__ << "] error parsing file: " << e.what() << std::endl;
//     throw std::runtime_error(msg.str());
//   }

//   // get event list
//   if ( j.find("events")==j.end() ) {
//     std::string msg = "Did not find 'events' key in JSON file";
//     throw std::runtime_error(msg);
//   }

//   int nevents_in_file = j["events"].size();

//   // get the event
//   int ievent = _json_nevents_gen;

//   if ( ievent>=nevents_in_file ) {
//     std::stringstream msg;
//     msg << "Ran out of events. Attempting to load eventid=" << ievent << " but only " << nevents_in_file << std::endl;
//     throw std::runtime_error(msg.str());
//   }

//   nlohmann::json jevent = j["events"].at(ievent);


//   // get event list
//   if ( j.find("volume")==j.end() ) {
//     std::string msg = "Did not find 'volume' key in JSON file";
//     throw std::runtime_error(msg);
//   }
  
//   nlohmann::json jvolume = j["volume"];

//   G4ThreeVector genpos;
//   if ( jvolume.find("r")!=jvolume.end() ) {
//     // radius defined in 
//     // cylindrical throw
//     G4cout << "  Gen in cylinder (R,Z)=(" << jvolume["r"].get<double>() << "," << jvolume["z"].get<double>() << G4endl;
//     GetPositionInLAr( genpos, jvolume["r"].get<double>(), jvolume["r"].get<double>(), jvolume["z"].get<double>(), true );
//   }
//   else if ( jvolume.find("x")!=jvolume.end() ) {
//     G4cout << "  Gen in box (X,Y,Z)=(" << jvolume["x"].get<double>() << "," << jvolume["y"].get<double>() << "," << jvolume["z"].get<double>() << ")" << G4endl;    
//     GetPositionInLAr( genpos, jvolume["x"].get<double>(), jvolume["y"].get<double>(), jvolume["z"].get<double>(), false );
//   }
//   else {
//     std::string msg = "did not find ('r','z') nor ('x','y','z') keys in 'volume' block. needed to generate position";
//     throw std::runtime_error(msg);
//   }
  
//   G4cout << "[CENNSPrimaryGeneratorAction::" << __FUNCTION__<< "] Generated position: " << genpos << G4endl;
//   _json_nevents_gen++;

//   // Get Pariticle List
//   if ( jevent.find("final_particles")==jevent.end() ) {
//     std::string msg = "did not find key 'final_particles' in 'event' block";
//     throw std::runtime_error(msg);
//   }

//   size_t num_particles = jevent["final_particles"].size();
//   G4cout << "[CENNSPrimaryGeneratorAction::" << __FUNCTION__<< "]  Number of particles: " << num_particles << G4endl;
  

//   for ( size_t iparticle=0; iparticle<num_particles; iparticle++ ) {

//     nlohmann::json jparticle = jevent["final_particles"].at(iparticle);
    
//     // shoot the particle gun for each final state particle
    
//     int pdg = jparticle["pdg"].get<int>();
//     G4double energy = jparticle["E"].get<double>()*MeV;
//     G4double mass   = jparticle["mass"].get<double>()*MeV;
//     G4double px     = jparticle["px"].get<double>()*MeV;
//     G4double py     = jparticle["py"].get<double>()*MeV;
//     G4double pz     = jparticle["pz"].get<double>()*MeV;

//     G4double ke = (energy-mass);
//     std::cout << " [" << iparticle << "] pdg=" << pdg << " KE=" << ke << std::endl;
    
//     if ( abs(pdg)==12 || abs(pdg)==14 || abs(pdg)==16 ) {
//       // skip the final-state neutrino (if there is one)
//       continue;
//     }

//     if ( pdg>1000000000 ) {
//       // nuclei
//       int A = pdg/10%1000;
//       int Z = pdg/10000%1000;
//       G4ParticleDefinition* recoil_nucleus = G4IonTable::GetIonTable()->GetIon(Z, A, 0.0*keV);
//       fPartGenerator->SetParticleDefinition(recoil_nucleus);
//     }
//     else {
//       // particles
//       std::stringstream msg;
//       switch ( pdg ) {
//       case 11:
//         fPartGenerator->SetParticleDefinition( G4Electron::Electron() );
//         break;
//       case -11:
//         fPartGenerator->SetParticleDefinition( G4Positron::Positron() );
//         break;
//       case 22:
//         fPartGenerator->SetParticleDefinition( G4Gamma::Gamma() );
//         break;
//       case 2212:
//         fPartGenerator->SetParticleDefinition( G4Proton::Proton() );
//         break;        
//       case 2112:
//         fPartGenerator->SetParticleDefinition( G4Neutron::Neutron() );
//         break;        
//       default:
//         msg << "[CENNSPrimaryGeneratorAction::GenerateMarley] unrecongnized PDG = " << pdg << std::endl;
//         throw std::runtime_error( msg.str() );
//         break;
//       }
//     }//end of if nucleus

//     fPartGenerator->SetParticleEnergy( ke );
//     G4ParticleMomentum mom( px, py, pz );
//     fPartGenerator->SetParticleMomentumDirection( mom );
//     fPartGenerator->SetParticlePosition( genpos );

//     // add particle to event
//     fPartGenerator->GeneratePrimaryVertex(event);
//   }
  
//   // Print status messages about simulation progress after every 100
//   // events have been generated
//   if (_json_nevents_gen % 100 == 1 ) {
//     // Print a status message showing the current number of events
//     G4cout << "[CENNSPrimaryGeneratorAction::" << __FUNCTION__ << "] Event Count = " << _json_nevents_gen << G4endl;
//   }

void VertexGen_Marley::SetState( G4String newValues ){
}
  
G4String VertexGen_Marley::GetState() {
  return "";
}
  
}//end of RAT namespace
