// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: mapreduceworker.proto

#include "mapreduceworker.pb.h"
#include "mapreduceworker.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace mapreduce_worker {

static const char* MapReduceWorker_method_names[] = {
  "/mapreduce_worker.MapReduceWorker/Work",
  "/mapreduce_worker.MapReduceWorker/Shutdown",
};

std::unique_ptr< MapReduceWorker::Stub> MapReduceWorker::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< MapReduceWorker::Stub> stub(new MapReduceWorker::Stub(channel));
  return stub;
}

MapReduceWorker::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_Work_(MapReduceWorker_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Shutdown_(MapReduceWorker_method_names[1], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status MapReduceWorker::Stub::Work(::grpc::ClientContext* context, const ::mapreduce_worker::JobMessage& request, ::mapreduce_common::EmptyMessage* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_Work_, context, request, response);
}

void MapReduceWorker::Stub::experimental_async::Work(::grpc::ClientContext* context, const ::mapreduce_worker::JobMessage* request, ::mapreduce_common::EmptyMessage* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Work_, context, request, response, std::move(f));
}

void MapReduceWorker::Stub::experimental_async::Work(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::mapreduce_common::EmptyMessage* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Work_, context, request, response, std::move(f));
}

void MapReduceWorker::Stub::experimental_async::Work(::grpc::ClientContext* context, const ::mapreduce_worker::JobMessage* request, ::mapreduce_common::EmptyMessage* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_Work_, context, request, response, reactor);
}

void MapReduceWorker::Stub::experimental_async::Work(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::mapreduce_common::EmptyMessage* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_Work_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::mapreduce_common::EmptyMessage>* MapReduceWorker::Stub::AsyncWorkRaw(::grpc::ClientContext* context, const ::mapreduce_worker::JobMessage& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::mapreduce_common::EmptyMessage>::Create(channel_.get(), cq, rpcmethod_Work_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::mapreduce_common::EmptyMessage>* MapReduceWorker::Stub::PrepareAsyncWorkRaw(::grpc::ClientContext* context, const ::mapreduce_worker::JobMessage& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::mapreduce_common::EmptyMessage>::Create(channel_.get(), cq, rpcmethod_Work_, context, request, false);
}

::grpc::Status MapReduceWorker::Stub::Shutdown(::grpc::ClientContext* context, const ::mapreduce_common::EmptyMessage& request, ::mapreduce_common::EmptyMessage* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_Shutdown_, context, request, response);
}

void MapReduceWorker::Stub::experimental_async::Shutdown(::grpc::ClientContext* context, const ::mapreduce_common::EmptyMessage* request, ::mapreduce_common::EmptyMessage* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Shutdown_, context, request, response, std::move(f));
}

void MapReduceWorker::Stub::experimental_async::Shutdown(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::mapreduce_common::EmptyMessage* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Shutdown_, context, request, response, std::move(f));
}

void MapReduceWorker::Stub::experimental_async::Shutdown(::grpc::ClientContext* context, const ::mapreduce_common::EmptyMessage* request, ::mapreduce_common::EmptyMessage* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_Shutdown_, context, request, response, reactor);
}

void MapReduceWorker::Stub::experimental_async::Shutdown(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::mapreduce_common::EmptyMessage* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_Shutdown_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::mapreduce_common::EmptyMessage>* MapReduceWorker::Stub::AsyncShutdownRaw(::grpc::ClientContext* context, const ::mapreduce_common::EmptyMessage& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::mapreduce_common::EmptyMessage>::Create(channel_.get(), cq, rpcmethod_Shutdown_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::mapreduce_common::EmptyMessage>* MapReduceWorker::Stub::PrepareAsyncShutdownRaw(::grpc::ClientContext* context, const ::mapreduce_common::EmptyMessage& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::mapreduce_common::EmptyMessage>::Create(channel_.get(), cq, rpcmethod_Shutdown_, context, request, false);
}

MapReduceWorker::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MapReduceWorker_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MapReduceWorker::Service, ::mapreduce_worker::JobMessage, ::mapreduce_common::EmptyMessage>(
          std::mem_fn(&MapReduceWorker::Service::Work), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MapReduceWorker_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MapReduceWorker::Service, ::mapreduce_common::EmptyMessage, ::mapreduce_common::EmptyMessage>(
          std::mem_fn(&MapReduceWorker::Service::Shutdown), this)));
}

MapReduceWorker::Service::~Service() {
}

::grpc::Status MapReduceWorker::Service::Work(::grpc::ServerContext* context, const ::mapreduce_worker::JobMessage* request, ::mapreduce_common::EmptyMessage* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MapReduceWorker::Service::Shutdown(::grpc::ServerContext* context, const ::mapreduce_common::EmptyMessage* request, ::mapreduce_common::EmptyMessage* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace mapreduce_worker
