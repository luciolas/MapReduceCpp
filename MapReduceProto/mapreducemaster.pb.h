// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mapreducemaster.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_mapreducemaster_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_mapreducemaster_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3012000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3012000 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
#include "mapreduce_common.pb.h"
#include "google/api/annotations.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_mapreducemaster_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_mapreducemaster_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_mapreducemaster_2eproto;
namespace mapreduce_master {
class JobStatus;
class JobStatusDefaultTypeInternal;
extern JobStatusDefaultTypeInternal _JobStatus_default_instance_;
}  // namespace mapreduce_master
PROTOBUF_NAMESPACE_OPEN
template<> ::mapreduce_master::JobStatus* Arena::CreateMaybeMessage<::mapreduce_master::JobStatus>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace mapreduce_master {

enum JobStatus_Status : int {
  JobStatus_Status_IDLE = 0,
  JobStatus_Status_WORKING = 1,
  JobStatus_Status_DONE = 2,
  JobStatus_Status_ERR = 4,
  JobStatus_Status_JobStatus_Status_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  JobStatus_Status_JobStatus_Status_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool JobStatus_Status_IsValid(int value);
constexpr JobStatus_Status JobStatus_Status_Status_MIN = JobStatus_Status_IDLE;
constexpr JobStatus_Status JobStatus_Status_Status_MAX = JobStatus_Status_ERR;
constexpr int JobStatus_Status_Status_ARRAYSIZE = JobStatus_Status_Status_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* JobStatus_Status_descriptor();
template<typename T>
inline const std::string& JobStatus_Status_Name(T enum_t_value) {
  static_assert(::std::is_same<T, JobStatus_Status>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function JobStatus_Status_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    JobStatus_Status_descriptor(), enum_t_value);
}
inline bool JobStatus_Status_Parse(
    const std::string& name, JobStatus_Status* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<JobStatus_Status>(
    JobStatus_Status_descriptor(), name, value);
}
// ===================================================================

class JobStatus PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:mapreduce_master.JobStatus) */ {
 public:
  inline JobStatus() : JobStatus(nullptr) {};
  virtual ~JobStatus();

  JobStatus(const JobStatus& from);
  JobStatus(JobStatus&& from) noexcept
    : JobStatus() {
    *this = ::std::move(from);
  }

  inline JobStatus& operator=(const JobStatus& from) {
    CopyFrom(from);
    return *this;
  }
  inline JobStatus& operator=(JobStatus&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const JobStatus& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const JobStatus* internal_default_instance() {
    return reinterpret_cast<const JobStatus*>(
               &_JobStatus_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(JobStatus& a, JobStatus& b) {
    a.Swap(&b);
  }
  inline void Swap(JobStatus* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(JobStatus* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline JobStatus* New() const final {
    return CreateMaybeMessage<JobStatus>(nullptr);
  }

  JobStatus* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<JobStatus>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const JobStatus& from);
  void MergeFrom(const JobStatus& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(JobStatus* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "mapreduce_master.JobStatus";
  }
  protected:
  explicit JobStatus(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_mapreducemaster_2eproto);
    return ::descriptor_table_mapreducemaster_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  typedef JobStatus_Status Status;
  static constexpr Status IDLE =
    JobStatus_Status_IDLE;
  static constexpr Status WORKING =
    JobStatus_Status_WORKING;
  static constexpr Status DONE =
    JobStatus_Status_DONE;
  static constexpr Status ERR =
    JobStatus_Status_ERR;
  static inline bool Status_IsValid(int value) {
    return JobStatus_Status_IsValid(value);
  }
  static constexpr Status Status_MIN =
    JobStatus_Status_Status_MIN;
  static constexpr Status Status_MAX =
    JobStatus_Status_Status_MAX;
  static constexpr int Status_ARRAYSIZE =
    JobStatus_Status_Status_ARRAYSIZE;
  static inline const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor*
  Status_descriptor() {
    return JobStatus_Status_descriptor();
  }
  template<typename T>
  static inline const std::string& Status_Name(T enum_t_value) {
    static_assert(::std::is_same<T, Status>::value ||
      ::std::is_integral<T>::value,
      "Incorrect type passed to function Status_Name.");
    return JobStatus_Status_Name(enum_t_value);
  }
  static inline bool Status_Parse(const std::string& name,
      Status* value) {
    return JobStatus_Status_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  enum : int {
    kStatusFieldNumber = 1,
    kIdFieldNumber = 2,
    kJobIdFieldNumber = 3,
  };
  // .mapreduce_master.JobStatus.Status status = 1;
  void clear_status();
  ::mapreduce_master::JobStatus_Status status() const;
  void set_status(::mapreduce_master::JobStatus_Status value);
  private:
  ::mapreduce_master::JobStatus_Status _internal_status() const;
  void _internal_set_status(::mapreduce_master::JobStatus_Status value);
  public:

  // uint32 id = 2;
  void clear_id();
  ::PROTOBUF_NAMESPACE_ID::uint32 id() const;
  void set_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_id() const;
  void _internal_set_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 job_id = 3;
  void clear_job_id();
  ::PROTOBUF_NAMESPACE_ID::uint32 job_id() const;
  void set_job_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_job_id() const;
  void _internal_set_job_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // @@protoc_insertion_point(class_scope:mapreduce_master.JobStatus)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  int status_;
  ::PROTOBUF_NAMESPACE_ID::uint32 id_;
  ::PROTOBUF_NAMESPACE_ID::uint32 job_id_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_mapreducemaster_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// JobStatus

// .mapreduce_master.JobStatus.Status status = 1;
inline void JobStatus::clear_status() {
  status_ = 0;
}
inline ::mapreduce_master::JobStatus_Status JobStatus::_internal_status() const {
  return static_cast< ::mapreduce_master::JobStatus_Status >(status_);
}
inline ::mapreduce_master::JobStatus_Status JobStatus::status() const {
  // @@protoc_insertion_point(field_get:mapreduce_master.JobStatus.status)
  return _internal_status();
}
inline void JobStatus::_internal_set_status(::mapreduce_master::JobStatus_Status value) {
  
  status_ = value;
}
inline void JobStatus::set_status(::mapreduce_master::JobStatus_Status value) {
  _internal_set_status(value);
  // @@protoc_insertion_point(field_set:mapreduce_master.JobStatus.status)
}

// uint32 id = 2;
inline void JobStatus::clear_id() {
  id_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 JobStatus::_internal_id() const {
  return id_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 JobStatus::id() const {
  // @@protoc_insertion_point(field_get:mapreduce_master.JobStatus.id)
  return _internal_id();
}
inline void JobStatus::_internal_set_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  id_ = value;
}
inline void JobStatus::set_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:mapreduce_master.JobStatus.id)
}

// uint32 job_id = 3;
inline void JobStatus::clear_job_id() {
  job_id_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 JobStatus::_internal_job_id() const {
  return job_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 JobStatus::job_id() const {
  // @@protoc_insertion_point(field_get:mapreduce_master.JobStatus.job_id)
  return _internal_job_id();
}
inline void JobStatus::_internal_set_job_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  job_id_ = value;
}
inline void JobStatus::set_job_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_job_id(value);
  // @@protoc_insertion_point(field_set:mapreduce_master.JobStatus.job_id)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace mapreduce_master

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::mapreduce_master::JobStatus_Status> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::mapreduce_master::JobStatus_Status>() {
  return ::mapreduce_master::JobStatus_Status_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_mapreducemaster_2eproto
