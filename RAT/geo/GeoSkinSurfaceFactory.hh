/// \class RAT::GeoSkinSurfaceFactory
///
/// \brief  Define a logical border between two solids
///
/// \author name T. Wongjirad
///
/// REVISION HISTORY:\n
///   25/03/2011 : P Gorel - New file \n
///
/// \detail Implement the possibility to have a logical border between two solids
///         Created in order to have TPB only between two defined solids, and not as a skin
///
////////////////////////////////////////////////////////////////////////

#ifndef __RAT_GeoSkinSurfaceFactory__
#define __RAT_GeoSkinSurfaceFactory__

#include <RAT/GeoFactory.hh>

namespace RAT {

class GeoSkinSurfaceFactory : public GeoFactory {
public:
  GeoSkinSurfaceFactory() : GeoFactory("skin") { };
  virtual G4VPhysicalVolume *Construct(DBLinkPtr table);
};

} // namespace RAT
#endif
