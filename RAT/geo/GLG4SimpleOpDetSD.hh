// This file is part of the RAT software library.
// $Id: GLG4SimpleOpDetSD.hh,v 1.0 2015/03/22 12:00:00 tmw Exp $
//
//  GLG4SimpleOpDetSD.hh
//
//   Copy of GLG4PMTSD. Used to bypass PMT physics.  
//   Operates in normal way: it looks for optical photons and creates a hit.
//
//   Author:  Glenn Horton-Smith, 2000/01/28 
//   Copied and modified: T. Wongjirad, 2015/03/22
//

#ifndef GLG4SimpleOpDetSD_h
#define GLG4SimpleOpDetSD_h 1

#include "G4VSensitiveDetector.hh"
#include "G4VPhysicalVolume.hh"
#include <map>
#include <vector>

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class GLG4SimpleOpDetSD : public G4VSensitiveDetector
{
  protected:
      int max_opdets;
      int opdet_no_offset;
      int my_id_opdet_size;
      // enum { max_waveform_ns= 200 };

  std::map< int, G4VPhysicalVolume* > channelmap; ///< channel number to physical volume
  std::map< G4VPhysicalVolume*, int > pv_to_channelid_map; ///< physical volume to channel number
  std::map< int, int > channelid_to_opdetindex; /// need to go from ID number (arbitrary) to opdetindex;
  std::map< std::vector<int>, int > copynoindex_to_opdetindex; ///< go from copy no of relevant touchables to opdet ID
  std::vector< std::string > _opdet_pvname_v; ///< hierarchy of physical volumes used to deterime channel ID
  
  public:
  
  std::map<int,int> hit_sum;
  //typedef G4int waveform_t[max_waveform_ns];
  //waveform_t *hit_waveform; /* indexed by opdet number */
  
  G4int n_opdet_hits;   /* # of hits,       calculated at EndOfEvent */
  G4int n_hit_opdets;   /* # of OPDETs hit,   calculated at EndOfEvent */

  public:
  // member functions
      GLG4SimpleOpDetSD(G4String name,
			int max_opdets=1000000,
			int opdet_no_offset=0 );
      GLG4SimpleOpDetSD(G4String name,
			std::vector<std::string>& opdet_pvname_v );
      virtual ~GLG4SimpleOpDetSD();

      virtual void Initialize(G4HCofThisEvent*HCE);
      virtual void EndOfEvent(G4HCofThisEvent*HCE);
      virtual void clear();
      virtual void DrawAll();
      virtual void PrintAll();

      void SimpleHit( G4int iopdet,
		      G4double time,
		      G4double kineticEnergy,
		      const G4ThreeVector & position,
		      const G4ThreeVector & momentum,
		      const G4ThreeVector & polarization,
		      G4int iHitPhotonCount,
		      G4int origin_flag,
		      G4int trackID=-1,
		      G4bool prepulse=false );
  
  void AddOpDetChannel( int idnum, std::vector<int>& copyno_index_v, G4VPhysicalVolume* pv ) { 
    channelmap[idnum] = pv; 
    pv_to_channelid_map[pv] = idnum; 
    channelid_to_opdetindex[idnum] = my_id_opdet_size;
    copynoindex_to_opdetindex[copyno_index_v] = idnum;
    my_id_opdet_size++;
  };
  protected:
      virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
};

#endif
