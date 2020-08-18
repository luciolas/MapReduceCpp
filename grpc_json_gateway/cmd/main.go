package main

import (
	gw "build_tools/v1/gw/mapreduce_master"
	"context"
	"flag"
	"net/http"

	// gw "build_tools/v1/gw/mapreduce_master" // Update

	"github.com/golang/glog"
	"github.com/grpc-ecosystem/grpc-gateway/runtime"
	"google.golang.org/grpc"
)

var (
	// command-line options:
	// gRPC server endpoint
	grpcServerEndpoint = flag.String("grpc-server-endpoint", "localhost:5050", "gRPC server endpoint")
)

func CustomMatcher(key string) (string, bool) {
	switch key {
	case "X-Client-Id":
		return key, true
	default:
		return runtime.DefaultHeaderMatcher(key)
	}
}

func run() error {
	ctx := context.Background()
	ctx, cancel := context.WithCancel(ctx)
	defer cancel()

	// Register gRPC server endpoint
	// Note: Make sure the gRPC server is running properly and accessible
	mux := runtime.NewServeMux(runtime.WithIncomingHeaderMatcher(CustomMatcher))
	opts := []grpc.DialOption{grpc.WithInsecure()}

	err := gw.RegisterMapReduceMasterHandlerFromEndpoint(ctx, mux, *grpcServerEndpoint, opts)
	if err != nil {
		return err
	}
	glog.Infoln("Listening...")
	// Start HTTP server (and proxy calls to gRPC server endpoint)
	return http.ListenAndServe(":5049", mux)
}

func main() {
	flag.Parse()
	defer glog.Flush()

	if err := run(); err != nil {
		glog.Fatal(err)
	}
}
