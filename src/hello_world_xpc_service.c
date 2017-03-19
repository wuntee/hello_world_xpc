#include <stdio.h>
#include <stdlib.h>
#include <xpc/xpc.h>
#include <bootstrap.h>

void print_object(xpc_object_t obj){
	char * desc = xpc_copy_description(obj);
	printf("Description:\n%s\n", desc);

	// iterate keys
	if(xpc_get_type(obj) == XPC_TYPE_DICTIONARY){
		printf("Iterate keys:\n");
		xpc_dictionary_apply(obj, ^bool(const char *key, xpc_object_t val){
			printf("\tkey: %s\n", key);
			xpc_object_t pulled_val = xpc_dictionary_get_value(obj, key);
			char* pulled_val_desc = xpc_copy_description(pulled_val);
			printf("\tval: %s\n", pulled_val_desc);
			free(pulled_val_desc);
			return(true);
		});
	}
	free(desc);
}

int main(void){
	printf("Staring...\n");
	fflush(stdout);
	xpc_connection_t listener = xpc_connection_create_mach_service("com.wuntee.helloworld", NULL, XPC_CONNECTION_MACH_SERVICE_LISTENER);
	printf("Created lsitener...\n");
	fflush(stdout);
	xpc_connection_set_event_handler(listener, ^(xpc_object_t peer) {
		printf("Object:\n");
		fflush(stdout);
		print_object(peer);
		fflush(stdout);
		if(xpc_get_type(peer) == XPC_TYPE_ERROR){
			printf("ERROR:\n");
			fflush(stdout);
			exit(-1);
		} else {
		   printf("Valid connection:\n");
		   fflush(stdout);
		   print_object(peer);
		   fflush(stdout);
	       // It is safe to cast 'peer' to xpc_connection_t assuming
	       // we have a correct configuration in our launchd.plist.
	       xpc_connection_set_event_handler(peer, ^(xpc_object_t event) {
	               // Handle event, whether it is a message or an error.
	       		printf("Inner object:\n");
       			fflush(stdout);
	       		print_object(event);
				fflush(stdout);
	       });
	       printf("Resuming inner connection\n");
       	   fflush(stdout);
	       xpc_connection_resume(peer);
	   }
	});
	printf("Resuming listener...\n");
	fflush(stdout);
	xpc_connection_resume(listener);

	printf("Dispatching main...\n");
	fflush(stdout);
	dispatch_main();

	printf("Exititng...\n");
	fflush(stdout);
}