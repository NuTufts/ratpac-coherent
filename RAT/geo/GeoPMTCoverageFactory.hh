#ifndef __RAT_GeoPMTCoverageFactory__
#define __RAT_GeoPMTCoverageFactory__

#include <RAT/geo/GeoPMTFactoryBase.hh>

namespace RAT {
 class GeoPMTCoverageFactory : public GeoPMTFactoryBase {
 public:
   GeoPMTCoverageFactory() : GeoPMTFactoryBase("pmtcoverage") {};
   virtual G4VPhysicalVolume *Construct(DBLinkPtr table);
 };
} // namespace RAT

#endif
