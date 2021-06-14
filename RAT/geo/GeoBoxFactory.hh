#ifndef __RAT_GeoBoxFactory__
#define __RAT_GeoBoxFactory__

#include <RAT/geo/GeoSolidFactory.hh>

namespace RAT {
 class GeoBoxFactory : public GeoSolidFactory {
 public:
   GeoBoxFactory() : GeoSolidFactory("box") {};
   virtual G4VSolid *ConstructSolid(DBLinkPtr table);
 };
  
} // namespace RAT

#endif
