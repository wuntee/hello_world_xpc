default: service client

service:
	clang -g src/hello_world_xpc_service.c -o bin/hello_world_xpc_service

client:
	clang -g src/hello_world_xpc_client.c -o bin/hello_world_xpc_client

clean:
	rm -rf bin/*
