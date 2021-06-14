////////////////////////////////////////////////////////////////////////
/// \class RAT::VertexFile_Gen
///
/// \brief Reads in RAT root trees and simulates the MC event in geant
///
/// \author R. Bonventre <rbonventre@lbl.gov> -- contact person
///
/// REVISION HISTORY:\n
///   27/02/2014 : R. Bonventre - First version
///   22/02/2015 : R. Bonventre - Modified for RAT-PAC
/// 
/// \details VertexFile_Gen is used to take event vertices generated by
///         RAT or any other program (for example GENIE atmospherics )
///         and simulate them in RAT. Unlike InRootProducer, which just
///         runs RAT processors on fully simulated data, this generator
///         lets you run Geant4 using just the MCParticle and MCParent
///         information. To use this generator the command is 
///       /generator/add vertexfile filename[:POSITION][:TIME][:NEVENTS][:NOFFSET]
///
///         Where the file is any RAT root file that at least has MC
///         particle information. If there is no POSITION or TIME or they
///         are set to 'default' it will use the position / time given in
///         the root file for that event. It will simulate NEVENTS of the events
///         in the file, starting with event NOFFSET. If it runs out of
///         events before you get through as many as specified with
///         /rat/run/start, it will end the simulation after the completion
///         of that event.
////////////////////////////////////////////////////////////////////////

#ifndef __RAT_VertexFile_Gen_
#define __RAT_VertexFile_Gen_

#include <RAT/gen/GLG4Gen.hh>

#include <TTimeStamp.h>

class TTree;

namespace RAT {

  namespace DS {
    class Root;
  }

class VertexFile_Gen: public GLG4Gen {
public:
  VertexFile_Gen() : fStateStr(""), fCurrentEvent(0), fNumEvents(0), fMaxEvent(0), fLastEventTime(0), fTimeGen(0), fPosGen(0) {};
  virtual ~VertexFile_Gen(){};

  void GenerateEvent(G4Event *event);
  bool IsRepeatable() const {return true;};

  /// @param[in] offset Time reset to this value
  void ResetTime(double offset=0.0);

  /// @param[in] state SetState state
  void SetState(G4String state);
  G4String GetState() const;

  /// specify/get parameters for time generator (eg from generator/rate/ commands) overriding ROOT file
  /// @param[in] state the time generator state
  void SetTimeState(G4String state);
  G4String GetTimeState() const;

  /// specify/get parameters for time generator (eg from generator/pos commands) overriding ROOT file
  /// @param[in] state the pos generator state
  void SetPosState(G4String state);
  G4String GetPosState() const;

protected:
  G4String fStateStr;
  int fCurrentEvent; ///< Index of current event from ROOT file
  int fNumEvents; ///< Read up to this many events from file
  int fMaxEvent; ///< Number of events in ROOT file

  TTimeStamp fLastEventTime; ///< Used to calculate time between events

  GLG4TimeGen* fTimeGen;
  GLG4PosGen* fPosGen;

  TTree *fTTree;
  DS::Root *fDS;
};

}
#endif
