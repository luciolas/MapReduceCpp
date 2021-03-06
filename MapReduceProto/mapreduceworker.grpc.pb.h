// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: mapreduceworker.proto
#ifndef GRPC_mapreduceworker_2eproto__INCLUDED
#define GRPC_mapreduceworker_2eproto__INCLUDED

#include "mapreduceworker.pb.h"

#include <functional>
#include <grpc/impl/codegen/port_platform.h>
#include <grpcpp/impl/codegen/async_generic_service.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/client_context.h>
#include <grpcpp/impl/codegen/completion_queue.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

namespace mapreduce_worker {

class MapReduceWorker final {
 public:
  static constexpr char const* service_full_name() {
    return "mapreduce_worker.MapReduceWorker";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status Work(::grpc::ClientContext* context, const ::mapreduce_common::JobMessage& request, ::mapreduce_common::EmptyMessage* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::mapreduce_common::EmptyMessage>> AsyncWork(::grpc::ClientContext* context, const ::mapreduce_common::JobMessage& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::mapreduce_common::EmptyMessage>>(AsyncWorkRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::mapreduce_common::EmptyMessage>> PrepareAsyncWork(::grpc::ClientContext* context, const ::mapreduce_common::JobMessage& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::mapreduce_common::EmptyMessage>>(PrepareAsyncWorkRaw(context, request, cq));
    }
    virtual ::grpc::Status Shutdown(::grpc::ClientContext* context, const ::mapreduce_common::EmptyMessage& request, ::mapreduce_common::EmptyMessage* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::mapreduce_common::EmptyMessage>> AsyncShutdown(::grpc::ClientContext* context, const ::mapreduce_common::EmptyMessage& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::mapreduce_common::EmptyMessage>>(AsyncShutdownRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::mapreduce_common::EmptyMessage>> PrepareAsyncShutdown(::grpc::ClientContext* context, const ::mapreduce_common::EmptyMessage& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::mapreduce_common::EmptyMessage>>(PrepareAsyncShutdownRaw(context, request, cq));
    }
    class experimental_async_interface {
     public:
      virtual ~experimental_async_interface() {}
      virtual void Work(::grpc::ClientContext* context, const ::mapreduce_common::JobMessage* request, ::mapreduce_common::EmptyMessage* response, std::function<void(::grpc::Status)>) = 0;
      virtual void Work(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::mapreduce_common::EmptyMessage* response, std::function<void(::grpc::Status)>) = 0;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void Work(::grpc::ClientContext* context, const ::mapreduce_common::JobMessage* request, ::mapreduce_common::EmptyMessage* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      #else
      virtual void Work(::grpc::ClientContext* context, const ::mapreduce_common::JobMessage* request, ::mapreduce_common::EmptyMessage* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
      #endif
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void Work(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::mapreduce_common::EmptyMessage* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      #else
      virtual void Work(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::mapreduce_common::EmptyMessage* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
      #endif
      virtual void Shutdown(::grpc::ClientContext* context, const ::mapreduce_common::EmptyMessage* request, ::mapreduce_common::EmptyMessage* response, std::function<void(::grpc::Status)>) = 0;
      virtual void Shutdown(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::mapreduce_common::EmptyMessage* response, std::function<void(::grpc::Status)>) = 0;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void Shutdown(::grpc::ClientContext* context, const ::mapreduce_common::EmptyMessage* request, ::mapreduce_common::EmptyMessage* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      #else
      virtual void Shutdown(::grpc::ClientContext* context, const ::mapreduce_common::EmptyMessage* request, ::mapreduce_common::EmptyMessage* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
      #endif
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void Shutdown(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::mapreduce_common::EmptyMessage* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      #else
      virtual void Shutdown(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::mapreduce_common::EmptyMessage* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
      #endif
    };
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    typedef class experimental_async_interface async_interface;
    #endif
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    async_interface* async() { return experimental_async(); }
    #endif
    virtual class experimental_async_interface* experimental_async() { return nullptr; }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::mapreduce_common::EmptyMessage>* AsyncWorkRaw(::grpc::ClientContext* context, const ::mapreduce_common::JobMessage& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::mapreduce_common::EmptyMessage>* PrepareAsyncWorkRaw(::grpc::ClientContext* context, const ::mapreduce_common::JobMessage& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::mapreduce_common::EmptyMessage>* AsyncShutdownRaw(::grpc::ClientContext* context, const ::mapreduce_common::EmptyMessage& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::mapreduce_common::EmptyMessage>* PrepareAsyncShutdownRaw(::grpc::ClientContext* context, const ::mapreduce_common::EmptyMessage& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    ::grpc::Status Work(::grpc::ClientContext* context, const ::mapreduce_common::JobMessage& request, ::mapreduce_common::EmptyMessage* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::mapreduce_common::EmptyMessage>> AsyncWork(::grpc::ClientContext* context, const ::mapreduce_common::JobMessage& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::mapreduce_common::EmptyMessage>>(AsyncWorkRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::mapreduce_common::EmptyMessage>> PrepareAsyncWork(::grpc::ClientContext* context, const ::mapreduce_common::JobMessage& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::mapreduce_common::EmptyMessage>>(PrepareAsyncWorkRaw(context, request, cq));
    }
    ::grpc::Status Shutdown(::grpc::ClientContext* context, const ::mapreduce_common::EmptyMessage& request, ::mapreduce_common::EmptyMessage* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::mapreduce_common::EmptyMessage>> AsyncShutdown(::grpc::ClientContext* context, const ::mapreduce_common::EmptyMessage& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::mapreduce_common::EmptyMessage>>(AsyncShutdownRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::mapreduce_common::EmptyMessage>> PrepareAsyncShutdown(::grpc::ClientContext* context, const ::mapreduce_common::EmptyMessage& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::mapreduce_common::EmptyMessage>>(PrepareAsyncShutdownRaw(context, request, cq));
    }
    class experimental_async final :
      public StubInterface::experimental_async_interface {
     public:
      void Work(::grpc::ClientContext* context, const ::mapreduce_common::JobMessage* request, ::mapreduce_common::EmptyMessage* response, std::function<void(::grpc::Status)>) override;
      void Work(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::mapreduce_common::EmptyMessage* response, std::function<void(::grpc::Status)>) override;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void Work(::grpc::ClientContext* context, const ::mapreduce_common::JobMessage* request, ::mapreduce_common::EmptyMessage* response, ::grpc::ClientUnaryReactor* reactor) override;
      #else
      void Work(::grpc::ClientContext* context, const ::mapreduce_common::JobMessage* request, ::mapreduce_common::EmptyMessage* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
      #endif
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void Work(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::mapreduce_common::EmptyMessage* response, ::grpc::ClientUnaryReactor* reactor) override;
      #else
      void Work(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::mapreduce_common::EmptyMessage* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
      #endif
      void Shutdown(::grpc::ClientContext* context, const ::mapreduce_common::EmptyMessage* request, ::mapreduce_common::EmptyMessage* response, std::function<void(::grpc::Status)>) override;
      void Shutdown(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::mapreduce_common::EmptyMessage* response, std::function<void(::grpc::Status)>) override;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void Shutdown(::grpc::ClientContext* context, const ::mapreduce_common::EmptyMessage* request, ::mapreduce_common::EmptyMessage* response, ::grpc::ClientUnaryReactor* reactor) override;
      #else
      void Shutdown(::grpc::ClientContext* context, const ::mapreduce_common::EmptyMessage* request, ::mapreduce_common::EmptyMessage* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
      #endif
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void Shutdown(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::mapreduce_common::EmptyMessage* response, ::grpc::ClientUnaryReactor* reactor) override;
      #else
      void Shutdown(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::mapreduce_common::EmptyMessage* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
      #endif
     private:
      friend class Stub;
      explicit experimental_async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class experimental_async_interface* experimental_async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class experimental_async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::mapreduce_common::EmptyMessage>* AsyncWorkRaw(::grpc::ClientContext* context, const ::mapreduce_common::JobMessage& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::mapreduce_common::EmptyMessage>* PrepareAsyncWorkRaw(::grpc::ClientContext* context, const ::mapreduce_common::JobMessage& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::mapreduce_common::EmptyMessage>* AsyncShutdownRaw(::grpc::ClientContext* context, const ::mapreduce_common::EmptyMessage& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::mapreduce_common::EmptyMessage>* PrepareAsyncShutdownRaw(::grpc::ClientContext* context, const ::mapreduce_common::EmptyMessage& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_Work_;
    const ::grpc::internal::RpcMethod rpcmethod_Shutdown_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status Work(::grpc::ServerContext* context, const ::mapreduce_common::JobMessage* request, ::mapreduce_common::EmptyMessage* response);
    virtual ::grpc::Status Shutdown(::grpc::ServerContext* context, const ::mapreduce_common::EmptyMessage* request, ::mapreduce_common::EmptyMessage* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_Work : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_Work() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_Work() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Work(::grpc::ServerContext* /*context*/, const ::mapreduce_common::JobMessage* /*request*/, ::mapreduce_common::EmptyMessage* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestWork(::grpc::ServerContext* context, ::mapreduce_common::JobMessage* request, ::grpc::ServerAsyncResponseWriter< ::mapreduce_common::EmptyMessage>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_Shutdown : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_Shutdown() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_Shutdown() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Shutdown(::grpc::ServerContext* /*context*/, const ::mapreduce_common::EmptyMessage* /*request*/, ::mapreduce_common::EmptyMessage* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestShutdown(::grpc::ServerContext* context, ::mapreduce_common::EmptyMessage* request, ::grpc::ServerAsyncResponseWriter< ::mapreduce_common::EmptyMessage>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_Work<WithAsyncMethod_Shutdown<Service > > AsyncService;
  template <class BaseClass>
  class ExperimentalWithCallbackMethod_Work : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithCallbackMethod_Work() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodCallback(0,
          new ::grpc_impl::internal::CallbackUnaryHandler< ::mapreduce_common::JobMessage, ::mapreduce_common::EmptyMessage>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::mapreduce_common::JobMessage* request, ::mapreduce_common::EmptyMessage* response) { return this->Work(context, request, response); }));}
    void SetMessageAllocatorFor_Work(
        ::grpc::experimental::MessageAllocator< ::mapreduce_common::JobMessage, ::mapreduce_common::EmptyMessage>* allocator) {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
    #else
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::experimental().GetHandler(0);
    #endif
      static_cast<::grpc_impl::internal::CallbackUnaryHandler< ::mapreduce_common::JobMessage, ::mapreduce_common::EmptyMessage>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~ExperimentalWithCallbackMethod_Work() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Work(::grpc::ServerContext* /*context*/, const ::mapreduce_common::JobMessage* /*request*/, ::mapreduce_common::EmptyMessage* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* Work(
      ::grpc::CallbackServerContext* /*context*/, const ::mapreduce_common::JobMessage* /*request*/, ::mapreduce_common::EmptyMessage* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* Work(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::mapreduce_common::JobMessage* /*request*/, ::mapreduce_common::EmptyMessage* /*response*/)
    #endif
      { return nullptr; }
  };
  template <class BaseClass>
  class ExperimentalWithCallbackMethod_Shutdown : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithCallbackMethod_Shutdown() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodCallback(1,
          new ::grpc_impl::internal::CallbackUnaryHandler< ::mapreduce_common::EmptyMessage, ::mapreduce_common::EmptyMessage>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::mapreduce_common::EmptyMessage* request, ::mapreduce_common::EmptyMessage* response) { return this->Shutdown(context, request, response); }));}
    void SetMessageAllocatorFor_Shutdown(
        ::grpc::experimental::MessageAllocator< ::mapreduce_common::EmptyMessage, ::mapreduce_common::EmptyMessage>* allocator) {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(1);
    #else
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::experimental().GetHandler(1);
    #endif
      static_cast<::grpc_impl::internal::CallbackUnaryHandler< ::mapreduce_common::EmptyMessage, ::mapreduce_common::EmptyMessage>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~ExperimentalWithCallbackMethod_Shutdown() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Shutdown(::grpc::ServerContext* /*context*/, const ::mapreduce_common::EmptyMessage* /*request*/, ::mapreduce_common::EmptyMessage* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* Shutdown(
      ::grpc::CallbackServerContext* /*context*/, const ::mapreduce_common::EmptyMessage* /*request*/, ::mapreduce_common::EmptyMessage* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* Shutdown(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::mapreduce_common::EmptyMessage* /*request*/, ::mapreduce_common::EmptyMessage* /*response*/)
    #endif
      { return nullptr; }
  };
  #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
  typedef ExperimentalWithCallbackMethod_Work<ExperimentalWithCallbackMethod_Shutdown<Service > > CallbackService;
  #endif

  typedef ExperimentalWithCallbackMethod_Work<ExperimentalWithCallbackMethod_Shutdown<Service > > ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_Work : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_Work() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_Work() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Work(::grpc::ServerContext* /*context*/, const ::mapreduce_common::JobMessage* /*request*/, ::mapreduce_common::EmptyMessage* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_Shutdown : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_Shutdown() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_Shutdown() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Shutdown(::grpc::ServerContext* /*context*/, const ::mapreduce_common::EmptyMessage* /*request*/, ::mapreduce_common::EmptyMessage* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_Work : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_Work() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_Work() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Work(::grpc::ServerContext* /*context*/, const ::mapreduce_common::JobMessage* /*request*/, ::mapreduce_common::EmptyMessage* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestWork(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawMethod_Shutdown : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_Shutdown() {
      ::grpc::Service::MarkMethodRaw(1);
    }
    ~WithRawMethod_Shutdown() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Shutdown(::grpc::ServerContext* /*context*/, const ::mapreduce_common::EmptyMessage* /*request*/, ::mapreduce_common::EmptyMessage* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestShutdown(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class ExperimentalWithRawCallbackMethod_Work : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithRawCallbackMethod_Work() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodRawCallback(0,
          new ::grpc_impl::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->Work(context, request, response); }));
    }
    ~ExperimentalWithRawCallbackMethod_Work() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Work(::grpc::ServerContext* /*context*/, const ::mapreduce_common::JobMessage* /*request*/, ::mapreduce_common::EmptyMessage* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* Work(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* Work(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #endif
      { return nullptr; }
  };
  template <class BaseClass>
  class ExperimentalWithRawCallbackMethod_Shutdown : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithRawCallbackMethod_Shutdown() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodRawCallback(1,
          new ::grpc_impl::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->Shutdown(context, request, response); }));
    }
    ~ExperimentalWithRawCallbackMethod_Shutdown() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Shutdown(::grpc::ServerContext* /*context*/, const ::mapreduce_common::EmptyMessage* /*request*/, ::mapreduce_common::EmptyMessage* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* Shutdown(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* Shutdown(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #endif
      { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_Work : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_Work() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler< ::mapreduce_common::JobMessage, ::mapreduce_common::EmptyMessage>(std::bind(&WithStreamedUnaryMethod_Work<BaseClass>::StreamedWork, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithStreamedUnaryMethod_Work() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status Work(::grpc::ServerContext* /*context*/, const ::mapreduce_common::JobMessage* /*request*/, ::mapreduce_common::EmptyMessage* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedWork(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::mapreduce_common::JobMessage,::mapreduce_common::EmptyMessage>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_Shutdown : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_Shutdown() {
      ::grpc::Service::MarkMethodStreamed(1,
        new ::grpc::internal::StreamedUnaryHandler< ::mapreduce_common::EmptyMessage, ::mapreduce_common::EmptyMessage>(std::bind(&WithStreamedUnaryMethod_Shutdown<BaseClass>::StreamedShutdown, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithStreamedUnaryMethod_Shutdown() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status Shutdown(::grpc::ServerContext* /*context*/, const ::mapreduce_common::EmptyMessage* /*request*/, ::mapreduce_common::EmptyMessage* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedShutdown(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::mapreduce_common::EmptyMessage,::mapreduce_common::EmptyMessage>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_Work<WithStreamedUnaryMethod_Shutdown<Service > > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_Work<WithStreamedUnaryMethod_Shutdown<Service > > StreamedService;
};

}  // namespace mapreduce_worker


#endif  // GRPC_mapreduceworker_2eproto__INCLUDED
