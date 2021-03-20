// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_POINTERINPUT_STARDUSTXR_H_
#define FLATBUFFERS_GENERATED_POINTERINPUT_STARDUSTXR_H_

#include "flatbuffers/flatbuffers.h"

#include "common_generated.h"

namespace StardustXR {

struct Pointer;
struct PointerBuilder;

struct Pointer FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef PointerBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ORIGIN = 4,
    VT_DIRECTION = 6,
    VT_TILT = 8
  };
  const StardustXR::vec3 *origin() const {
    return GetStruct<const StardustXR::vec3 *>(VT_ORIGIN);
  }
  StardustXR::vec3 *mutable_origin() {
    return GetStruct<StardustXR::vec3 *>(VT_ORIGIN);
  }
  const StardustXR::vec3 *direction() const {
    return GetStruct<const StardustXR::vec3 *>(VT_DIRECTION);
  }
  StardustXR::vec3 *mutable_direction() {
    return GetStruct<StardustXR::vec3 *>(VT_DIRECTION);
  }
  float tilt() const {
    return GetField<float>(VT_TILT, 0.0f);
  }
  bool mutate_tilt(float _tilt) {
    return SetField<float>(VT_TILT, _tilt, 0.0f);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyFieldRequired<StardustXR::vec3>(verifier, VT_ORIGIN) &&
           VerifyFieldRequired<StardustXR::vec3>(verifier, VT_DIRECTION) &&
           VerifyField<float>(verifier, VT_TILT) &&
           verifier.EndTable();
  }
};

struct PointerBuilder {
  typedef Pointer Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_origin(const StardustXR::vec3 *origin) {
    fbb_.AddStruct(Pointer::VT_ORIGIN, origin);
  }
  void add_direction(const StardustXR::vec3 *direction) {
    fbb_.AddStruct(Pointer::VT_DIRECTION, direction);
  }
  void add_tilt(float tilt) {
    fbb_.AddElement<float>(Pointer::VT_TILT, tilt, 0.0f);
  }
  explicit PointerBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<Pointer> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Pointer>(end);
    fbb_.Required(o, Pointer::VT_ORIGIN);
    fbb_.Required(o, Pointer::VT_DIRECTION);
    return o;
  }
};

inline flatbuffers::Offset<Pointer> CreatePointer(
    flatbuffers::FlatBufferBuilder &_fbb,
    const StardustXR::vec3 *origin = 0,
    const StardustXR::vec3 *direction = 0,
    float tilt = 0.0f) {
  PointerBuilder builder_(_fbb);
  builder_.add_tilt(tilt);
  builder_.add_direction(direction);
  builder_.add_origin(origin);
  return builder_.Finish();
}

}  // namespace StardustXR

#endif  // FLATBUFFERS_GENERATED_POINTERINPUT_STARDUSTXR_H_
