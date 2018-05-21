#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#include<pthread.h> //for threading , link with lpthread

void *connection_handler(void *);

int main() {
	int socket_desc , new_socket , c , *new_sock;
	    struct sockaddr_in server , client;
	    char *message;
	// open a file to serve

	// create a socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	 if (socket_desc == -1)
	 {
			 printf("Could not create socket");
	 }

	 //Prepare the sockaddr_in structure
     server.sin_family = AF_INET;
     server.sin_addr.s_addr = INADDR_ANY;
     server.sin_port = htons( 8888 );

     //Bind
     if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
     {
         puts("bind failed");
         return 1;
     }
     puts("bind done");

     //Listen
     listen(socket_desc , 5);
		 puts("Waiting for incoming connections...");
	    c = sizeof(struct sockaddr_in);
	    while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
	    {
	        puts("Connection accepted");

	        //Reply to the client
	     
	        pthread_t sniffer_thread;
	        new_sock = malloc(1);
	        *new_sock = new_socket;

	        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
	        {
	            perror("could not create thread");
	            return 1;
	        }

	        //Now join the thread , so that we dont terminate before the thread
	        //pthread_join( sniffer_thread , NULL);
	        puts("Handler assigned");
	    }

	    if (new_socket<0)
	    {
	        perror("accept failed");
	        return 1;
	    }

	    return 0;


}
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
		FILE *html_data;


		html_data =  fopen("index.html", "r");


		//char response_data[1024];
		//fgets(response_data, 1024, html_data);
		char http_header[5000] = "HTTP/1.1 200 OK\r\n";
	//	char type[1024]="Content-Type: application/pdf\r\n\r\n";




    char string[100];
		while(fgets(string, 100, html_data)) {
		   write(sock, string, strlen(string));

		}
	//char http_header[2048] = "HTTP/1.1 200 OK\r\n\n";
//	strcat(http_header, document);

  //  send(sock,http_header,sizeof(http_header),0);

    //Free the socket pointer


free(socket_desc);
    return 0;
}
