#!/bin/sh

# OPTIONAL Comment if not needed
export PATH="B:\OpenSource\vcpkg\packages\protobuf_x64-windows\tools\protobuf:$PATH"
GOOGLE_PROTO_PATH="B:\OpenSource\vcpkg\packages\protobuf_x64-windows\include"
# OPTIONAL 

DEST=MapReduceProto
PROTOC_DIR=./Common/proto
GW_PATH=$DEST/gw/
GO_GW_PATH="grpc_json_gateway\gw\mapreduce_master"
GO_GW_BUILD_PATH="grpc_json_gateway\cmd"

FROM=.

echo "#########################################"
echo "Begin running protoc files recursively"
echo "Protoc builder from ${GOOGLE_PROTO_PATH}"
echo "Protoc files from ${PROTOC_DIR}"
echo "#########################################"
COUNT=0
PROTO_PATHS=()
walk_dir () {
    for pathname in "$1"/*; do
        if [ -d "$pathname" ]; then
            walk_dir "$pathname"
        elif [ -e "$pathname" ]; then
            case "$pathname" in
                *.proto)
                    #printf '%s\n' "$pathname"
                    PROTO_PATHS+=("$pathname")
            esac
        fi
    done
}

walk_dir "$FROM"
mkdir -p $GW_PATH

for i in "${PROTO_PATHS[@]}"
do 
    echo "$i"
    protoc -I $PROTOC_DIR --proto_path=$GOOGLE_PROTO_PATH  --grpc_out=$DEST/ \
        --plugin=protoc-gen-grpc=$PROTOC_DIR/grpc_cpp_plugin.exe $i

    protoc -I $PROTOC_DIR  --proto_path=$GOOGLE_PROTO_PATH  --cpp_out=$DEST/ \
        $i

    protoc -I $PROTOC_DIR --proto_path=$GOOGLE_PROTO_PATH  --grpc-gateway_out=logtostderr=true,paths=source_relative:$GO_GW_PATH \
        $i

    protoc -I $PROTOC_DIR  --proto_path=$GOOGLE_PROTO_PATH  --go_out=plugins=grpc,paths=source_relative:$GO_GW_PATH \
    $i
done

echo "Building gateway..."
cd $GO_GW_BUILD_PATH && go build . 
echo "Done building gateway..."