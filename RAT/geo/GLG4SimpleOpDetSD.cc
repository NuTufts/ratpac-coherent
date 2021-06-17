// This file is part of the GenericLAND software library.
// $Id: GLG4SimpleOpDetSD.cc,v 1.1 2005/08/30 19:55:22 volsung Exp $
//
//  GLG4SimpleOpDetSD.cc
//
//   Records total number of hits on each SimpleOpDet.
//   Uses Geant4-style hit collection to record hit time, place, etc.
//
//  GLG4 version by Glenn Horton-Smith December, 2004.
//  Based on earlier work by O. Tajima and G. Horton-Smith
//

#include "GLG4SimpleOpDetSD.hh"
#include "RAT/core/GLG4VEventAction.hh"

#include "G4Track.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4TouchableHandle.hh"
#include "G4ios.hh"
#include "G4SDManager.hh"

#include "RAT/physics/GLG4Scint.hh"  // for doScintilllation and total energy deposition info
#include "G4VSolid.hh" // for access to solid store
#include "Randomize.hh"
#include <RAT/base/Log.hh>

#include <string.h>  // for memset

GLG4SimpleOpDetSD::GLG4SimpleOpDetSD(G4String name, int arg_max_opdets, int arg_opdet_no_offset )
:G4VSensitiveDetector(name)
{
  max_opdets= arg_max_opdets;
  opdet_no_offset= arg_opdet_no_offset;
  my_id_opdet_size= 0;  
  hit_sum.clear();
  _opdet_pvname_v.clear();
}

GLG4SimpleOpDetSD::GLG4SimpleOpDetSD(G4String name, std::vector<std::string>& opdet_pvname_v )
  :G4VSensitiveDetector(name),
   _opdet_pvname_v(opdet_pvname_v)
{
  max_opdets= 100000;
  opdet_no_offset= 0;
  my_id_opdet_size= 0;  
  hit_sum.clear();
}

GLG4SimpleOpDetSD::~GLG4SimpleOpDetSD()
{
  hit_sum.clear();
}


void GLG4SimpleOpDetSD::Initialize(G4HCofThisEvent*)
{
  hit_sum.clear();
  n_opdet_hits= n_hit_opdets= 0;
}


G4bool GLG4SimpleOpDetSD::ProcessHits(G4Step* aStep, G4TouchableHistory* hist)
{

  if (aStep->GetTrack()->GetDefinition()->GetParticleName() != "opticalphoton")
    return false;
  // things to do:
  // (1) get optical ID number (means we need system of indexing!)
  // (2) get track info
  // (3) call SimpleHit
  // (4) kill photon

  // get optical id
  G4StepPoint* prestep = aStep->GetPreStepPoint();

  // to get the channel id, we need to get a tuple based on a set of copy numbers
  // across the depth of the touchable volumes
  
  int channelid = -1;
  G4VPhysicalVolume* pv = NULL;
  std::vector<int> copyno_v( _opdet_pvname_v.size(), 0 );
  
  G4TouchableHandle touchable = aStep->GetPreStepPoint()->GetTouchableHandle();
  for ( int idepth=0; idepth<touchable->GetHistoryDepth(); idepth++ ) {
    G4VPhysicalVolume* pv  = touchable->GetVolume(idepth);        
    //RAT::info << "GLG4SimpleOpDetSD[" << GetName() << "]::ProcessHits] depth=" << idepth << " volume=" << pv->GetName() << newline;
    for (int i=0; i<(int)_opdet_pvname_v.size(); i++) {
      if ( pv->GetName().contains( _opdet_pvname_v[i] ) )
        copyno_v[i] = pv->GetCopyNo();
    }
  }

  auto it_chid = copynoindex_to_opdetindex.find( copyno_v );
  if (it_chid==copynoindex_to_opdetindex.end()) {
    RAT::Log::Die("GLG4SimpleOpDetSD.cc: Could not find channel from copyno vector");
  }
  channelid = it_chid->second;

  // RAT::info << "GLG4SimpleOpDetSD[" << GetName() << "] hit. copyno_v (";
  // for (int i=0; i<(int)copyno_v.size(); i++) {
  //   RAT::info << copyno_v[i];
  //   if ( i+1<(int)copyno_v.size() )
  //     RAT::info << ",";
  // }
  // RAT::info << ") --> id " << channelid << newline;
  
  G4double time = aStep->GetTrack()->GetGlobalTime();
  G4double ke = aStep->GetTrack()->GetKineticEnergy();
  G4ThreeVector pos = aStep->GetTrack()->GetPosition();
  G4ThreeVector mom = aStep->GetTrack()->GetMomentum();
  G4ThreeVector pol = aStep->GetTrack()->GetPolarization();
  G4int N_pe = 1;
  G4int trackid = aStep->GetTrack()->GetTrackID();
  G4bool prepulse = false;

  // RAT::info << "GLG4SimpleOpDetSD detects photon in OpDet Channel " << channelid << "!" << newline;
  // if (  aStep->GetTrack()->GetCreatorProcess() )
  //   RAT::info << " Photon origin=" << aStep->GetTrack()->GetCreatorProcess()->GetProcessName() << newline;
  // else
  //   RAT::info << " Photon origin=" << "not-specified" << newline;
              
  
  int origin_flag = -1;
  if ( aStep->GetTrack()->GetCreatorProcess() ) {
    if ( aStep->GetTrack()->GetCreatorProcess()->GetProcessName()=="Cerenkov" )
      origin_flag = 0;
    else if ( aStep->GetTrack()->GetCreatorProcess()->GetProcessName()=="Scintillation" )
      origin_flag = 1;
    else if ( aStep->GetTrack()->GetCreatorProcess()->GetProcessName()=="Reemission" )
      origin_flag = 2;
    else
      origin_flag = -1;
  }
  
  //RAT::info << "GLG4SimpleOpDetSD detects photon in OpDet Channel " << channelid << "!" << newline;

  SimpleHit( channelid, time, ke, pos, mom, pol, N_pe, trackid, origin_flag, prepulse );
  
  aStep->GetTrack()->SetTrackStatus(fStopAndKill);
  return true;
}


// Here is the real "hit" routine, used by GLG4SimpleOpDetOpticalModel and by ProcessHits
// It is more efficient in some ways.
void GLG4SimpleOpDetSD::SimpleHit( G4int iopdet,
				   G4double time,
				   G4double kineticEnergy,
				   const G4ThreeVector &hit_position,
				   const G4ThreeVector &hit_momentum,
				   const G4ThreeVector &hit_polarization,
				   G4int iHitPhotonCount,
				   G4int trackID,
				   G4int origin_flag,
				   G4bool prepulse )
{
  G4int opdet_index = channelid_to_opdetindex[iopdet]-opdet_no_offset;
  if (opdet_index < 0 || opdet_index >= my_id_opdet_size)
    {
      G4cerr << "Error: GLG4SimpleOpDetSD::SimpleHit [" << GetName() << "] passed iopdet="
	     << iopdet << ", but max_opdets=" << max_opdets
	     << " and offset=" << opdet_no_offset << " !" << G4endl;
      return;
    }

  if ( hit_sum.find(opdet_index)==hit_sum.end() )
    hit_sum[opdet_index] = 0;
  
  hit_sum[opdet_index] += iHitPhotonCount;

  // create new GLG4HitPhoton, the way of recording photo hits on SimpleOpDets
  GLG4HitPhoton* hit_photon = new GLG4HitPhoton();
  hit_photon->SetPMTID((int)iopdet);
  hit_photon->SetTime((double) time );
  hit_photon->SetKineticEnergy((double) kineticEnergy );
  hit_photon->SetPosition( 
			  (double)hit_position.x(),
			  (double)hit_position.y(),
			  (double)hit_position.z()
			  );
  hit_photon->SetMomentum( 
			  (double)hit_momentum.x(),
			  (double)hit_momentum.y(),
			  (double)hit_momentum.z()
			  );
  hit_photon->SetPolarization( 
			      (double)hit_polarization.x(),
			      (double)hit_polarization.y(),
			      (double)hit_polarization.z()
			      );
  hit_photon->SetCount( iHitPhotonCount );
  hit_photon->SetTrackID( trackID );
  hit_photon->SetPrepulse( prepulse );
  hit_photon->SetOriginFlag( origin_flag );
    
  GLG4VEventAction::GetTheHitPMTCollection()->DetectPhoton(hit_photon); // we still use the PMT hit collection.
}


void GLG4SimpleOpDetSD::EndOfEvent(G4HCofThisEvent*)
{
  int iopdet;
    
  n_opdet_hits=0;
  n_hit_opdets=0;
  for (iopdet=0; iopdet<max_opdets; iopdet++) {
    if (hit_sum[iopdet]) {
      n_opdet_hits+= hit_sum[iopdet];
      n_hit_opdets++;
    }
  }

}


void GLG4SimpleOpDetSD::clear()
{} 

void GLG4SimpleOpDetSD::DrawAll()
{}

void GLG4SimpleOpDetSD::PrintAll()
{
}
