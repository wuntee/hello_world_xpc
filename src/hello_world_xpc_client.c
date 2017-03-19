#include <stdio.h>
#include <stdlib.h>
#include <xpc/xpc.h>
#include <bootstrap.h>

void print_object(xpc_object_t obj){
	char * desc = xpc_copy_description(obj);
	printf("%s\n", desc);
	free(desc);
}

int main(void){
	// mach_port_t port, bp, replyPort;
	// kern_return_t ret = task_get_bootstrap_port(mach_task_self(), &bp);	
	// ret = bootstrap_look_up(bp, "com.wuntee.helloworld", &port);
	// printf("BS lookup return: 0x%x\n", ret);
	// printf("com.wuntee.helloworld port: 0x%x : %i\n", port, port);

    xpc_object_t msg = xpc_dictionary_create(0x0, 0x0, 0x0);
    xpc_dictionary_set_string(msg, "test", "test_value");

	xpc_connection_t conn = xpc_connection_create_mach_service("com.wuntee.helloworld", dispatch_get_main_queue(), 0);
	if(!conn){
		printf("Could not create connection\n");
		exit(-1);
	}

	printf("Connection:\n");
	print_object(conn);

	xpc_connection_set_event_handler(conn, ^(xpc_object_t event) {
 		print_object(event);
	});

	printf("Resuming...\n");
	xpc_connection_resume(conn);

	printf("Sending message...\n");
	xpc_connection_send_message(conn, msg);

	dispatch_main();
}
