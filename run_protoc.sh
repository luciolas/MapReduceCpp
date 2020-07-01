#!/bin/sh

export PATH="B:\OpenSource\vcpkg\packages\protobuf_x64-windows\tools\protobuf:$PATH"
DEST=MapReduceProto/
FROM=.
echo "Begin running protoc recursively"
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

for i in "${PROTO_PATHS[@]}"
do 
    echo "$i"
    protoc -I ./Common/proto --grpc_out=$DEST \
        --plugin=protoc-gen-grpc=./Common/proto/grpc_cpp_plugin.exe $i

    protoc -I ./Common/proto --cpp_out=$DEST \
        $i
done

