//========================CITATION=============================================
// the general outline of the code is from Dr Williamson from his mypal-server.c
// file on the website and he has allowed the use of the code. Minor edits have
// been made by Nuha Shaikh, 30084641. Links to his code can be found here
// https://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass0/mypal-server.c 

/* TCP-based proxy of socket programming.                */
/* The proxy manipulates HTTP web pages    by changing   */
/* occurences of the word "Happy" found in the HTTP web  */
/* page to "Silly" and chnaging all JPG images to an     */
/* of a happy clown. To end                              */

/* Usage: gcc proxy_two.c -o proxy                       */
/*        ./proxy                                        */
/*                                                       */
/* Written by Nuha Shaikh            January 26, 2022    */

/* Include files for C socket programming and stuff */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>	//strlen
#include <sys/socket.h>
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>	//write
#include <netdb.h>
#include <regex.h>
#include <ctype.h>

/* Include files for C socket programming and stuff */
#define SERVEPORT  11180 /* This port can be changed depending on your local machine */
#define PORT 80

//wordReplace: takes in a sentence -> char array, a char array that needs to be
//removed from the sentense which is word and the replacement of word
void str_replace(char *sentence, const char *target, const char *replacement)
{
    char buffer[1024] = { 0 };
    char *found = &buffer[0];
    const char *tmp = sentence;
    size_t tar_len = strlen(target);
    size_t repl_len = strlen(replacement);

    while (1) {
        const char *point = strstr(tmp, target);

        // if gone past last occurence of target in sentence
        if (point == NULL) {
            strcpy(found, tmp); //copy rest
            break;
        }

        // copy chars before point of target word
        memcpy(found, tmp, point - tmp);
        found += point - tmp;

        // copy repl array 
        memcpy(found, replacement, repl_len);
        found += repl_len;
        tmp = point + tar_len;
    }

    // write altered string back to sentence
    strcpy(sentence, buffer);
}

// main program of the proxy
int main(int argc, char *argv[]){

	int socket_desc, client_sock;
    int mysocket1 ;
	struct sockaddr_in server, client;
	char client_message[2000];
    char messageOut[2000];

	//======server begins======================
	//Socket() Create socket for server
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket\n");
	}
	puts("Socket created\n");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(SERVEPORT);
	
	//bind() server
    int bindStatus = bind(socket_desc, (struct sockaddr *)&server, sizeof(server));
	if( bindStatus == -1)
    {
		//print the error message
		perror("Binding failed");
		return 1;
	}
	printf("Binding done.\n");
	
	//listen() server
	if( listen(socket_desc, 5) == -1 )
    {
        fprintf(stderr, "mypal-server: listen() call failed!\n");
        exit(1);
    }

	printf("Waiting for clients...\n");

	//Receive a message from client
    int recvStatus;
	while(1){
        //accept() accept connection from an incoming client
        client_sock = accept(socket_desc, NULL, NULL);
        
        if (client_sock < 0)
        {
            perror("Connection failed");
            return 1;
        }
        printf("Connection accepted\n");
        
        //recv() server
        recvStatus = recv(client_sock, client_message, 1000, 0);
        if (recvStatus==-1)
        {
            printf("Error in receiving!");
            break;
        }
        //======server ends ======================

        printf("Client says: %s\n", client_message);
        //=========================CITATION===================================
        // The general outline of this code is from TA Bardia Abhari from his 
        // file Hostname.cpp and HttpClient.cpp which are both permitted to use
        // by Dr Williamson, links to his code can be found here: 
        // https://d2l.ucalgary.ca/d2l/le/content/426495/Home

        //parsing client message to get hostname
		char serverRequest[10000];
        char path[10000];
        char url[10000];
        char host[10000];
        int i;
        strcpy(serverRequest, client_message);
        
        char *line = strtok(client_message, "\r\n"); // get first line of request

        sscanf(line, "GET http://%s", url); //scan url from first line of request

        //retrive only host -> get rid of appending path
        for (i = 0; i < strlen(url); i++)
        {
            if (url[i] == '/')
            {
                strncpy(host, url, i);
                host[i] = '\0';
                break;
            }
        }
        /* clear out message strings*/
        bzero(path, 10000);
        strcat(path, &url[i]);

        // socket() for client socket creation
        if(host[0] != '\0')
        {
            printf("client socket creation!\n");
            struct sockaddr_in server_address ;
            memset(&server_address, 0, sizeof(server_address));
            server_address.sin_family = AF_INET;
            server_address.sin_port = htons(PORT);

            struct hostent *address;
            address = gethostbyname(host);
            bcopy((char *) address->h_addr, (char *) &server_address.sin_addr.s_addr, address->h_length);


            //connect() clinet socket
            mysocket1 = socket(AF_INET, SOCK_STREAM, 0) ;
            if (mysocket1 == -1)
            {
                printf("Socket creation failed !\n") ;
            }
            printf("Socket created!\n") ;
            int status = connect(mysocket1, (struct sockaddr*) &server_address, sizeof(struct sockaddr_in)) ;
            if (status == -1)
            {
                printf("connect failed!\n");
                return -1 ;
            }
            printf("Connection succesful!\n") ;

            //send() GET request recived only if GET request was received
            printf("This is first letter of serverRequest: %c\n",serverRequest[0]);
            if(serverRequest[0]=='G')
            {
                //replace all JPG image files on a given Web page with an image of a happy clown instead
                int count;
                char buffer[1000];
                char manipulation[1000];
                char imgurl[1000];

                //clearning all buffers
                memset(buffer, 0, 1000);
                memset(imgurl, 0, 1000);
                memset(manipulation, 0, 1000);

                //incase of corruption doing manipulation on manipulation string instead of main string
                strcpy(manipulation, serverRequest);
                char* check;
                check= strstr(serverRequest, ".jpg");
                if(check){ //if serverRequest contains a .jpg image 
             
                    sscanf(serverRequest, "GET %s HTTP/1.", imgurl); //retrive imgurl
                    printf("This is img url: %s\n", imgurl);
                    char replacement[1000]= "http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/clown1.png"; //clown replacement url
                    printf("This is img replacement: %s\n", replacement);
                    printf("NOT Altered string is: %s\n", serverRequest);
                    str_replace(serverRequest, imgurl, replacement); //calling replacement function
                    printf("Altered string is: %s\n", serverRequest);
                }
     
                printf("Sending to server:%s\n", serverRequest);
                
                count = send(mysocket1, serverRequest, strlen(serverRequest), 0) ;
                if (count == -1)
                {
                    printf("send failed!\n") ;
                }else
                {
                    printf("send succesful!\n") ;
                }
                    //to ensure all images download completely
                    while((count = recv(mysocket1, buffer, 1000, 0))>0)
                    {
                        buffer[count] = '\0';
                        printf("recieve succesful!\n") ;
                        printf("Server is saying:\n %s", buffer) ;
                    
                        //do the string manipulation and change up messageOut
                        // strcpy(messageOut, buffer);
                        bcopy(buffer, messageOut, count);
                        //change all occurences of "Happy" to "Silly"

                        char word[1000]= "Happy";
                        char replacement[1000]= "Silly";
                        printf("NOT Altered string is: %s\n", messageOut);
                        str_replace(messageOut, word, replacement);
                        printf("Altered string is: %s\n", messageOut);
                    //=========================END CITATION====================

                        //======server begins again======================

                        // send() the result message back to the client

                        recvStatus = send(client_sock, messageOut, count, 0);
                        if( recvStatus < 0 )
                        {
                            fprintf(stderr, "mypal-server: send() failed! OMG!!\n");
                        }

                        /* clear out message strings*/
                        bzero(buffer, 1000);
                        bzero(client_message, 1000);
                        bzero(messageOut, 1000);
                    }

            }
            else
            {
                printf("Request received was not GET, unable to handle this request from server!\n");
            }
            

        }
        else
        {
            printf("We cant handle this web browser!\n");
        }

        // strcat(serverMsg,"Hi Client, this is your msg: ");
        // strcat(serverMsg, client_message);
        // printf("IM HERE");
		// write(client_sock , serverMsg , strlen(serverMsg));
	}

    //closing all sockets when done
    close(mysocket1);
    close(client_sock);
    close(socket_desc);
	return 0;
}

//========================END CITATION=============================================
