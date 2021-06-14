// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: photonHit.proto

#ifndef PROTOBUF_photonHit_2eproto__INCLUDED
#define PROTOBUF_photonHit_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace hitPhotons {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_photonHit_2eproto();
void protobuf_AssignDesc_photonHit_2eproto();
void protobuf_ShutdownFile_photonHit_2eproto();

class Photon;
class PhotonHits;

enum Photon_OriginFlag {
  Photon_OriginFlag_CHERENKOV = 0,
  Photon_OriginFlag_SCINTILLATION = 1,
  Photon_OriginFlag_REEMISSION = 2,
  Photon_OriginFlag_CHROMA = 3,
  Photon_OriginFlag_Photon_OriginFlag_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  Photon_OriginFlag_Photon_OriginFlag_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool Photon_OriginFlag_IsValid(int value);
const Photon_OriginFlag Photon_OriginFlag_OriginFlag_MIN = Photon_OriginFlag_CHERENKOV;
const Photon_OriginFlag Photon_OriginFlag_OriginFlag_MAX = Photon_OriginFlag_CHROMA;
const int Photon_OriginFlag_OriginFlag_ARRAYSIZE = Photon_OriginFlag_OriginFlag_MAX + 1;

const ::google::protobuf::EnumDescriptor* Photon_OriginFlag_descriptor();
inline const ::std::string& Photon_OriginFlag_Name(Photon_OriginFlag value) {
  return ::google::protobuf::internal::NameOfEnum(
    Photon_OriginFlag_descriptor(), value);
}
inline bool Photon_OriginFlag_Parse(
    const ::std::string& name, Photon_OriginFlag* value) {
  return ::google::protobuf::internal::ParseNamedEnum<Photon_OriginFlag>(
    Photon_OriginFlag_descriptor(), name, value);
}
// ===================================================================

class Photon : public ::google::protobuf::Message {
 public:
  Photon();
  virtual ~Photon();

  Photon(const Photon& from);

  inline Photon& operator=(const Photon& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Photon& default_instance();

  void Swap(Photon* other);

  // implements Message ----------------------------------------------

  inline Photon* New() const { return New(NULL); }

  Photon* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Photon& from);
  void MergeFrom(const Photon& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(Photon* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  typedef Photon_OriginFlag OriginFlag;
  static const OriginFlag CHERENKOV = Photon_OriginFlag_CHERENKOV;
  static const OriginFlag SCINTILLATION = Photon_OriginFlag_SCINTILLATION;
  static const OriginFlag REEMISSION = Photon_OriginFlag_REEMISSION;
  static const OriginFlag CHROMA = Photon_OriginFlag_CHROMA;
  static inline bool OriginFlag_IsValid(int value) {
    return Photon_OriginFlag_IsValid(value);
  }
  static const OriginFlag OriginFlag_MIN =
    Photon_OriginFlag_OriginFlag_MIN;
  static const OriginFlag OriginFlag_MAX =
    Photon_OriginFlag_OriginFlag_MAX;
  static const int OriginFlag_ARRAYSIZE =
    Photon_OriginFlag_OriginFlag_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  OriginFlag_descriptor() {
    return Photon_OriginFlag_descriptor();
  }
  static inline const ::std::string& OriginFlag_Name(OriginFlag value) {
    return Photon_OriginFlag_Name(value);
  }
  static inline bool OriginFlag_Parse(const ::std::string& name,
      OriginFlag* value) {
    return Photon_OriginFlag_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // optional int32 PMTID = 1;
  void clear_pmtid();
  static const int kPMTIDFieldNumber = 1;
  ::google::protobuf::int32 pmtid() const;
  void set_pmtid(::google::protobuf::int32 value);

  // optional double Time = 2;
  void clear_time();
  static const int kTimeFieldNumber = 2;
  double time() const;
  void set_time(double value);

  // optional double KineticEnergy = 3;
  void clear_kineticenergy();
  static const int kKineticEnergyFieldNumber = 3;
  double kineticenergy() const;
  void set_kineticenergy(double value);

  // optional double posX = 4;
  void clear_posx();
  static const int kPosXFieldNumber = 4;
  double posx() const;
  void set_posx(double value);

  // optional double posY = 5;
  void clear_posy();
  static const int kPosYFieldNumber = 5;
  double posy() const;
  void set_posy(double value);

  // optional double posZ = 6;
  void clear_posz();
  static const int kPosZFieldNumber = 6;
  double posz() const;
  void set_posz(double value);

  // optional double momX = 7;
  void clear_momx();
  static const int kMomXFieldNumber = 7;
  double momx() const;
  void set_momx(double value);

  // optional double momY = 8;
  void clear_momy();
  static const int kMomYFieldNumber = 8;
  double momy() const;
  void set_momy(double value);

  // optional double momZ = 9;
  void clear_momz();
  static const int kMomZFieldNumber = 9;
  double momz() const;
  void set_momz(double value);

  // optional double polX = 10;
  void clear_polx();
  static const int kPolXFieldNumber = 10;
  double polx() const;
  void set_polx(double value);

  // optional double polY = 11;
  void clear_poly();
  static const int kPolYFieldNumber = 11;
  double poly() const;
  void set_poly(double value);

  // optional double polZ = 12;
  void clear_polz();
  static const int kPolZFieldNumber = 12;
  double polz() const;
  void set_polz(double value);

  // optional int32 trackID = 14;
  void clear_trackid();
  static const int kTrackIDFieldNumber = 14;
  ::google::protobuf::int32 trackid() const;
  void set_trackid(::google::protobuf::int32 value);

  // optional .hitPhotons.Photon.OriginFlag origin = 15;
  void clear_origin();
  static const int kOriginFieldNumber = 15;
  ::hitPhotons::Photon_OriginFlag origin() const;
  void set_origin(::hitPhotons::Photon_OriginFlag value);

  // @@protoc_insertion_point(class_scope:hitPhotons.Photon)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  double time_;
  double kineticenergy_;
  double posx_;
  double posy_;
  double posz_;
  double momx_;
  ::google::protobuf::int32 pmtid_;
  ::google::protobuf::int32 trackid_;
  double momy_;
  double momz_;
  double polx_;
  double poly_;
  double polz_;
  int origin_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_photonHit_2eproto();
  friend void protobuf_AssignDesc_photonHit_2eproto();
  friend void protobuf_ShutdownFile_photonHit_2eproto();

  void InitAsDefaultInstance();
  static Photon* default_instance_;
};
// -------------------------------------------------------------------

class PhotonHits : public ::google::protobuf::Message {
 public:
  PhotonHits();
  virtual ~PhotonHits();

  PhotonHits(const PhotonHits& from);

  inline PhotonHits& operator=(const PhotonHits& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const PhotonHits& default_instance();

  void Swap(PhotonHits* other);

  // implements Message ----------------------------------------------

  inline PhotonHits* New() const { return New(NULL); }

  PhotonHits* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const PhotonHits& from);
  void MergeFrom(const PhotonHits& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(PhotonHits* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 count = 1;
  void clear_count();
  static const int kCountFieldNumber = 1;
  ::google::protobuf::int32 count() const;
  void set_count(::google::protobuf::int32 value);

  // repeated .hitPhotons.Photon photon = 2;
  int photon_size() const;
  void clear_photon();
  static const int kPhotonFieldNumber = 2;
  const ::hitPhotons::Photon& photon(int index) const;
  ::hitPhotons::Photon* mutable_photon(int index);
  ::hitPhotons::Photon* add_photon();
  const ::google::protobuf::RepeatedPtrField< ::hitPhotons::Photon >&
      photon() const;
  ::google::protobuf::RepeatedPtrField< ::hitPhotons::Photon >*
      mutable_photon();

  // @@protoc_insertion_point(class_scope:hitPhotons.PhotonHits)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::RepeatedPtrField< ::hitPhotons::Photon > photon_;
  ::google::protobuf::int32 count_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_photonHit_2eproto();
  friend void protobuf_AssignDesc_photonHit_2eproto();
  friend void protobuf_ShutdownFile_photonHit_2eproto();

  void InitAsDefaultInstance();
  static PhotonHits* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// Photon

// optional int32 PMTID = 1;
inline void Photon::clear_pmtid() {
  pmtid_ = 0;
}
inline ::google::protobuf::int32 Photon::pmtid() const {
  // @@protoc_insertion_point(field_get:hitPhotons.Photon.PMTID)
  return pmtid_;
}
inline void Photon::set_pmtid(::google::protobuf::int32 value) {
  
  pmtid_ = value;
  // @@protoc_insertion_point(field_set:hitPhotons.Photon.PMTID)
}

// optional double Time = 2;
inline void Photon::clear_time() {
  time_ = 0;
}
inline double Photon::time() const {
  // @@protoc_insertion_point(field_get:hitPhotons.Photon.Time)
  return time_;
}
inline void Photon::set_time(double value) {
  
  time_ = value;
  // @@protoc_insertion_point(field_set:hitPhotons.Photon.Time)
}

// optional double KineticEnergy = 3;
inline void Photon::clear_kineticenergy() {
  kineticenergy_ = 0;
}
inline double Photon::kineticenergy() const {
  // @@protoc_insertion_point(field_get:hitPhotons.Photon.KineticEnergy)
  return kineticenergy_;
}
inline void Photon::set_kineticenergy(double value) {
  
  kineticenergy_ = value;
  // @@protoc_insertion_point(field_set:hitPhotons.Photon.KineticEnergy)
}

// optional double posX = 4;
inline void Photon::clear_posx() {
  posx_ = 0;
}
inline double Photon::posx() const {
  // @@protoc_insertion_point(field_get:hitPhotons.Photon.posX)
  return posx_;
}
inline void Photon::set_posx(double value) {
  
  posx_ = value;
  // @@protoc_insertion_point(field_set:hitPhotons.Photon.posX)
}

// optional double posY = 5;
inline void Photon::clear_posy() {
  posy_ = 0;
}
inline double Photon::posy() const {
  // @@protoc_insertion_point(field_get:hitPhotons.Photon.posY)
  return posy_;
}
inline void Photon::set_posy(double value) {
  
  posy_ = value;
  // @@protoc_insertion_point(field_set:hitPhotons.Photon.posY)
}

// optional double posZ = 6;
inline void Photon::clear_posz() {
  posz_ = 0;
}
inline double Photon::posz() const {
  // @@protoc_insertion_point(field_get:hitPhotons.Photon.posZ)
  return posz_;
}
inline void Photon::set_posz(double value) {
  
  posz_ = value;
  // @@protoc_insertion_point(field_set:hitPhotons.Photon.posZ)
}

// optional double momX = 7;
inline void Photon::clear_momx() {
  momx_ = 0;
}
inline double Photon::momx() const {
  // @@protoc_insertion_point(field_get:hitPhotons.Photon.momX)
  return momx_;
}
inline void Photon::set_momx(double value) {
  
  momx_ = value;
  // @@protoc_insertion_point(field_set:hitPhotons.Photon.momX)
}

// optional double momY = 8;
inline void Photon::clear_momy() {
  momy_ = 0;
}
inline double Photon::momy() const {
  // @@protoc_insertion_point(field_get:hitPhotons.Photon.momY)
  return momy_;
}
inline void Photon::set_momy(double value) {
  
  momy_ = value;
  // @@protoc_insertion_point(field_set:hitPhotons.Photon.momY)
}

// optional double momZ = 9;
inline void Photon::clear_momz() {
  momz_ = 0;
}
inline double Photon::momz() const {
  // @@protoc_insertion_point(field_get:hitPhotons.Photon.momZ)
  return momz_;
}
inline void Photon::set_momz(double value) {
  
  momz_ = value;
  // @@protoc_insertion_point(field_set:hitPhotons.Photon.momZ)
}

// optional double polX = 10;
inline void Photon::clear_polx() {
  polx_ = 0;
}
inline double Photon::polx() const {
  // @@protoc_insertion_point(field_get:hitPhotons.Photon.polX)
  return polx_;
}
inline void Photon::set_polx(double value) {
  
  polx_ = value;
  // @@protoc_insertion_point(field_set:hitPhotons.Photon.polX)
}

// optional double polY = 11;
inline void Photon::clear_poly() {
  poly_ = 0;
}
inline double Photon::poly() const {
  // @@protoc_insertion_point(field_get:hitPhotons.Photon.polY)
  return poly_;
}
inline void Photon::set_poly(double value) {
  
  poly_ = value;
  // @@protoc_insertion_point(field_set:hitPhotons.Photon.polY)
}

// optional double polZ = 12;
inline void Photon::clear_polz() {
  polz_ = 0;
}
inline double Photon::polz() const {
  // @@protoc_insertion_point(field_get:hitPhotons.Photon.polZ)
  return polz_;
}
inline void Photon::set_polz(double value) {
  
  polz_ = value;
  // @@protoc_insertion_point(field_set:hitPhotons.Photon.polZ)
}

// optional int32 trackID = 14;
inline void Photon::clear_trackid() {
  trackid_ = 0;
}
inline ::google::protobuf::int32 Photon::trackid() const {
  // @@protoc_insertion_point(field_get:hitPhotons.Photon.trackID)
  return trackid_;
}
inline void Photon::set_trackid(::google::protobuf::int32 value) {
  
  trackid_ = value;
  // @@protoc_insertion_point(field_set:hitPhotons.Photon.trackID)
}

// optional .hitPhotons.Photon.OriginFlag origin = 15;
inline void Photon::clear_origin() {
  origin_ = 0;
}
inline ::hitPhotons::Photon_OriginFlag Photon::origin() const {
  // @@protoc_insertion_point(field_get:hitPhotons.Photon.origin)
  return static_cast< ::hitPhotons::Photon_OriginFlag >(origin_);
}
inline void Photon::set_origin(::hitPhotons::Photon_OriginFlag value) {
  
  origin_ = value;
  // @@protoc_insertion_point(field_set:hitPhotons.Photon.origin)
}

// -------------------------------------------------------------------

// PhotonHits

// optional int32 count = 1;
inline void PhotonHits::clear_count() {
  count_ = 0;
}
inline ::google::protobuf::int32 PhotonHits::count() const {
  // @@protoc_insertion_point(field_get:hitPhotons.PhotonHits.count)
  return count_;
}
inline void PhotonHits::set_count(::google::protobuf::int32 value) {
  
  count_ = value;
  // @@protoc_insertion_point(field_set:hitPhotons.PhotonHits.count)
}

// repeated .hitPhotons.Photon photon = 2;
inline int PhotonHits::photon_size() const {
  return photon_.size();
}
inline void PhotonHits::clear_photon() {
  photon_.Clear();
}
inline const ::hitPhotons::Photon& PhotonHits::photon(int index) const {
  // @@protoc_insertion_point(field_get:hitPhotons.PhotonHits.photon)
  return photon_.Get(index);
}
inline ::hitPhotons::Photon* PhotonHits::mutable_photon(int index) {
  // @@protoc_insertion_point(field_mutable:hitPhotons.PhotonHits.photon)
  return photon_.Mutable(index);
}
inline ::hitPhotons::Photon* PhotonHits::add_photon() {
  // @@protoc_insertion_point(field_add:hitPhotons.PhotonHits.photon)
  return photon_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::hitPhotons::Photon >&
PhotonHits::photon() const {
  // @@protoc_insertion_point(field_list:hitPhotons.PhotonHits.photon)
  return photon_;
}
inline ::google::protobuf::RepeatedPtrField< ::hitPhotons::Photon >*
PhotonHits::mutable_photon() {
  // @@protoc_insertion_point(field_mutable_list:hitPhotons.PhotonHits.photon)
  return &photon_;
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace hitPhotons

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::hitPhotons::Photon_OriginFlag> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::hitPhotons::Photon_OriginFlag>() {
  return ::hitPhotons::Photon_OriginFlag_descriptor();
}

}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_photonHit_2eproto__INCLUDED