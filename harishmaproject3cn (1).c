
#include<stdio.h>
#include<string.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<stdlib.h>


#define max_clients 10  // max val of  client db that can handle

int clients_joined=0; // var count of clients joined.


struct client_variable  // struct var that keep client data.
{	
	int socket_id;      //socket descriptor socket id val 
    char client_name[100];  //stores client name.
};


struct client_variable *ClientsList[max_clients]; // clients info that is stored in struct db.


void *thread_handler(void *arg);  //thread function.

int main ( int argInput, char **argc)
{
	int socketfd;     //socket descriptor value for client
	int client_socket;   
	int result;
	int port_id;      //port number of server
	int i=0;         
	struct sockaddr_in server_address; //server address  
	struct sockaddr_in client_address; //client address 
	int client_length;           //client address length.
	pthread_t client_t[50];   //store the thread id.
	
	/*check required cmd aruguments*/
	if (argInput != 2)
    {
        printf("usage:./prog3svr <svr_port>\n");
        exit(1);
    }
	/*socket file descriptor for UDP socket */
	socketfd=socket(AF_INET,SOCK_STREAM,0);
	
	/*Check whether the socket is created successfully or not */	
	if(socketfd==-1)
	{
		printf("\n : As Connection is Failed it could not create socket");
		exit(1);
	}
	
	port_id = atoi(argc[1]);
	
	// Configure settings of the server address struct

  
	bzero((char*)&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); //Set port number
    server_address.sin_port=htons(port_id); //IP address is set to localhost
	
	client_length=sizeof(client_address);
    /*function which assign local port address to a socket*/
	result= bind(socketfd,(struct sockaddr *)&server_address, sizeof(server_address));
	
	if(result == -1)
	{
		printf("\n Error:Connection is Failed it could not bind address with socket \n ");
		exit(1);		
	}
	
	client_length=sizeof(client_address);
	// 10 max connection requests queued 
	result=listen(socketfd,max_clients);
	
	if(result<0)   
	{
		printf("Error on listen\n");
		exit(1);
	}
	/*Server  waits for client connection*/
	printf("Waits for all Incoming Connections...\n");
	
	
	//Accept call will create a new socket for the incoming connection
	while( (client_socket = accept(socketfd, (struct sockaddr *)&client_address, &client_length)) )
	{
		
		struct client_variable *cli = (struct client_variable *)malloc(sizeof(struct client_variable));
		
		cli -> socket_id = client_socket;
		
		printf("Client (%d): Connection is Accepted\n",cli -> socket_id);
		

       //main thread for next request
		if(pthread_create(&client_t[i++], NULL ,&thread_handler, (void *)cli ) != 0)
		{
			printf("can not create thread\n");
			exit(1);
		}	
		
		
	}
	//closing the threads
	while(i!=0)  
	{
		pthread_join(client_t[i],NULL);
		i--;
		
	}
	return 0;
}





void *thread_handler(void *arg)  // thread handler function
{
	struct client_variable *cli1 = (struct client_variable *)arg;  //stores the clients info in a struct variable mentioned above.
	
	printf("Client (%d): Connection Handler Assigned\n",cli1->socket_id);
	
	while(1)  //making the server to listen to client 
	{
		//variables for looping and for string extraction from the client message.
		char message1[4096];
		int s=0;
		int n=0;
		int m=0;
		int i=0;
		int j=0;
		int k=0;
		int a=0;
		char command[100] = {'\0'};
		char client_name[100] = {'\0'};
		char client_message[2048] = {'\0'};
		char client_bmessage[2048] = {'\0'};
		int already_joined=0;
		
		//receiving query from the client.
		int recv_len = recv(cli1->socket_id, message1, 4096 ,0);
		message1[recv_len] = '\0';
		strncpy(command,message1,4);
		m=strlen(message1);
		
		// based on the query "JOIN" "LIST" "MESG" "BCST" "QUIT" extracting the required information from the message received from the server.
		if(strncmp(command,"JOIN",4)==0)
		{
			s=1;
			
			if(message1[4]==' ')
			{
				for(i=5; message1[i]!='\0'&& i<m-1;i++)  
				{	
					char user[10]={'\0'};
					sprintf(user,"%c",message1[i]); 
					strcat(client_name,user);
				}
				client_name[strlen(client_name)]='\0';
			}
		}
		else if(strncmp(command,"LIST",4)==0)
		{
			s=2;
		}
		else if(strncmp(command,"MESG",4)==0)
		{
			s=3;
			m=strlen(message1);
			i=0;
			if(message1[4] ==' ')
			{
				for(k=5;message1[k]!=' ';k++)
				{	
					char user[10]={'\0'};
					sprintf(user,"%c",message1[k]); 
					strcat(client_name,user);
				}
				
				i=k;

				if(message1[i]==' ')
				{	
					i++;
					while( message1[i]!='\0'&& i<m-1) 
					{	
						char mesg1[10]={'\0'};
						sprintf(mesg1,"%c",message1[i]); 
						strcat(client_message,mesg1);
						i++;
					}
				}
			}
		}
			
		else if (strncmp(command,"BCST",4)==0)
		{
			m=strlen(message1);
			s=4;
			if(message1[4]==' ')
			{
				int j=5;
				while(message1[j]!='\0'&& j<m-1)  
				{	
					char msg1[10]={'\0'};
					sprintf(msg1,"%c",message1[j]); 
					strcat(client_bmessage,msg1);
					j=j+1;
				}
			}
		}
		else if (strncmp(command,"QUIT",4)==0)
		{
			s=5;
		}
		
		
		//switch case
		switch(s)
		{
	
			case 1: //for join operation based on client joined we add client to database accroding to number of clients it supports.
					
					
					
					n=strlen(cli1->client_name);
					if(n==0)
					{
						int n1=0;
						n1=strlen(client_name);
						clients_joined++;
						if( clients_joined > max_clients) // if the limit exceeds.
						{
							printf("Client (%d): Database is Full. Disconnecting the User\n",cli1->socket_id);  
							printf("Error: Too Many Clients are Connected\n");
							send(cli1->socket_id,"Too Many Users are there. Disconnecting  the User.\n",strlen("Too Many Users. Disconnecting User.\n"),0); //sending status to client 
							close(cli1->socket_id); //closing the socket connection
							clients_joined--;  //decreasing the count of clients joined.
							cli1=NULL; //freeing up the cli1 struct memory
				
							pthread_exit(NULL);
						}
						
						i=0;
						
						for(a=0;a<max_clients;a++)
						{		
					
								if(ClientsList[a])
								{	
									if(strncmp(ClientsList[a]->client_name,client_name,strlen(ClientsList[a]->client_name))==0 && strncmp(ClientsList[a]->client_name,client_name,strlen(client_name)-1)==0)  
									{
										already_joined=1;
										
										break;
									}
								}
								
								a++;
						}
						
						
						
					
						//if client name not null
						if (n1!=0 && already_joined==0 ) // for non register client, adding client entry to database.
						{	 
							strncpy(cli1->client_name,client_name,strlen(client_name)-1);  
							
							for(i=0;i<max_clients;i++)
							{
								if (!ClientsList[i])  
								{
									ClientsList[i] = cli1;
									break;
								}
							}
							printf("Client (%d): JOIN %s\n",cli1->socket_id,cli1->client_name);  
							strncpy(message1,message1,sizeof(message1)-1);
							strcat(message1," Request is Accepted\n");
							send(cli1->socket_id,message1,strlen(message1),0);  //sending status message client
						}
						else  // if the submision of query is done improperly
						{
							if(already_joined==1)
							{
								printf("Client (%d): User Already Registered. Discarding the JOIN\n",cli1->socket_id);
								char fd[50];
								char msg[1000]={'\0'};
								for(i=0;i<max_clients;i++)  
								{
									if(ClientsList[i])
									{
										if (strncmp(ClientsList[i]->client_name,client_name,strlen(ClientsList[i]->client_name))==0)  
										{
											sprintf(fd,"%d", ClientsList[i]->socket_id);
											break;
										}
									}
								}
							strcpy(msg,"User is Already Registered : Username - ");
							strncat(msg,client_name,strlen(client_name)-1);
							strcat(msg," FD - ");
							strcat(msg,fd);
							strcat(msg,"\n");
							send(cli1->socket_id,msg,strlen(msg),0); //sending status message to server.
							memset(msg,0,1000);
							}
						
							else
							{
								
								printf("Client (%d): Unrecognizable Message is received. Discarding UNKNOWN Message\n",cli1->socket_id);  //printing status message on server side  
								n=strlen(cli1->client_name);
								if(n!=0)   
									send(cli1->socket_id,"Unknown Message is approched. Discarding UNKNOWN Message.\n",strlen("Unknown Message. Discarding UNKNOWN Message.\n") ,0); //sending status message to client.
								else 
									send(cli1->socket_id,"Unregistered User. Use \"JOIN <username>\" to Register.\n",strlen("Unregistered User. Use \"JOIN <username>\" to Register.\n"), 0);  //sending status message to client.
							}
						}
					}
					else // for registered client.
					{	
						printf("Client (%d): User Already Registered. Discarding JOIN\n",cli1->socket_id);
						char fd[50];
						char msg[1000]={'\0'};
						for(i=0;i<max_clients;i++)  
						{
							if(ClientsList[i])
							{
								if (strncmp(ClientsList[i]->client_name,client_name,strlen(ClientsList[i]->client_name))==0)  
								{
									sprintf(fd,"%d", ClientsList[i]->socket_id);
									break;
								}
							}
						}
						strcpy(msg,"User Already Registered : Username - ");
						strcat(msg,cli1->client_name);
						strcat(msg,"\t,FD - ");
						strcat(msg,fd);
						strcat(msg,"\n");
						send(cli1->socket_id,msg,strlen(msg),0); //sending status message to server.
						memset(msg,0,1000);
					}
					break;
		

			case 2:  //For  query LIST operation.
					printf("Client (%d): LIST\n",cli1->socket_id);
					n=strlen(cli1->client_name);
					if(n==0) 
					{
						printf("Unable to Locate Client (%d) in Database. Discarding LIST\n",cli1->socket_id);    
						send(cli1->socket_id,"Unregistered User. Use \"JOIN <username>\" to Register.\n",strlen("Unregistered User. Use \"JOIN <username>\" to Register.\n"), 0); //sending status message to server
					}
					//sending the information of the records in the database to the client.
					else							
					{
						char msg[1000]="USERNAME\tFD\n----------------------------\n";
						//extracting the client records from the data base.
						for(i=0;i<max_clients;i++)  
						{
							if(ClientsList[i])
							{
								char fd[50];
								strcat(msg,ClientsList[i]->client_name);
								strcat(msg,"		");
								sprintf(fd,"%d",ClientsList[i]->socket_id);
								strcat(msg,fd);
								strcat(msg,"\n");
							}
						}	
						strcat(msg,"----------------------------\n");
						send(cli1->socket_id,msg,strlen(msg),0);//sending status message to client.  
						memset(msg,0,1000);
					}
					break;
					
			case 3: //For query MESG to send message to recipient client by one of the clients in the database 
					n=strlen(cli1->client_name);
				
					if(n==0)
					{
						printf("Unable to find Client (%d) location in Database. Discarding MESG\n",cli1->socket_id);    // server status msessage
						
						send(cli1->socket_id,"Unregistered User. Use \"JOIN <username>\" to Register.\n",strlen("Unregistered User. Use \"JOIN <username>\" to Register.\n"), 0); // sending status msessage to client
						
					}
				
					else
					{	
						int recipients_socket=0;
						int recipient=0;
						j=0;
						k=0;
						for(i=0;i<max_clients;i++)  
						{
							if(ClientsList[i])
							{
							//checking if the recipient is registered in the database.
								if(strncmp(ClientsList[i]->client_name,client_name,strlen(client_name)) == 0 && strncmp(ClientsList[i]->client_name,client_name,strlen(ClientsList[i]->client_name)) == 0) 
								{
									recipients_socket = ClientsList[i]->socket_id;
									recipient=1;  
									break;
								}
							}
						}
					
						k=strlen(client_message);
						if(strcmp(cli1->client_name,client_name)!=0) // checking if the sender is not sending message to himself
						{
							if(k!=0)
							{	
								if(recipient==1) 
								{	
									char msg[4096]={'\0'};
									char msg1[2048]={'\0'};
									char sendername[100]={'\0'};
									char sender[100]={'\0'};
									strncat(msg1,client_message,strlen(client_message)-1);
									strncpy(sendername,cli1->client_name,strlen(cli1->client_name)+1);
									strcat(msg,"FROM ");
									sprintf(sender,"%s :",sendername);
									strcat(msg,sender);
									strcat(msg,msg1);
									strcat(msg,"\n");
									msg[strlen(msg)]='\0';
									send(recipients_socket,msg,strlen(msg),0);//sending status message to client
									memset(msg,0,2048);
								}
								
								else
								{ 	
									//if the recipient is not registered in databse.
									printf("Unable to find Recipient location (%s) in Database. Discarding MESG.\n",client_name); 
									char msg1[1000]="Unknown Recipient (";
									strcat(msg1,client_name);
									strcat(msg1,"). MESG Discarded.\n");
									send(cli1->socket_id,msg1,strlen(msg1),0);  //sending status message to client.
									memset(msg1,0,1000);
								}	
							}
							else
							{
								//condition if the Message is NULL
								printf("Message is NULL  Discarding MESG.\n"); 
								char msg1[1000]="Message is NULL-";
								strcat(msg1," MESG Discarded.\n");
								send(cli1->socket_id,msg1,strlen(msg1),0); 
								memset(msg1,0,1000);
							}
						}
							
						else 
						{	
							//if the client is attempting to send message to itself.
							printf("client (%d) :Unable to send message sender and receiver are same.\n",cli1->socket_id); 
							char msg1[1000]="Client can't send message to self -";
							strcat(msg1," MESG is Discarded.\n");
							send(cli1->socket_id,msg1,strlen(msg1),0); 
							memset(msg1,0,1000);
						}
							
					
							
					}
					
						
					
					break;
						
						
			case 4:  
					// case for BCST query
					
					n=strlen(cli1->client_name);
					if(n==0) // checking if the client is registered or not to send broadcast message
					{	
						//for non registered user.
						printf("Unable to Locate Client (%d) in Database. Discarding BCST\n",cli1->socket_id);    
						send(cli1->socket_id,"Unregistered User. Use \"JOIN <username>\" to Register.\n",strlen("Unregistered User. Use \"JOIN <username>\" to Register.\n"), 0); //sending status message to client.
					}
					else 
					{
						//Sending the broadcast message sent by the registered client to all other registered clients except itself.
						i=0;
						if(strlen(client_bmessage)==0)
						{
								//condition if the Message is NULL
								printf("Message is NULL  Discarding BCST.\n"); 
								char msg1[1000]="Message is NULL(";
								strcat(msg1,") BCST is Discarded.\n");
								send(cli1->socket_id,msg1,strlen(msg1),0); 
								memset(msg1,0,1000);
								
								
						}
						else
						{
							
							for(i=0;i<max_clients;i++)  
					
							{
								if(ClientsList[i]) //sending bcst message to all the registered users
								{
									char msg[1000]={'\0'};
									strcat(msg,"FROM ");
									strncat(msg,cli1->client_name,strlen(cli1->client_name)+1);
									strcat(msg,": ");
									strcat(msg,client_bmessage);
									strcat(msg,"\n");
									if(ClientsList[i]->socket_id != cli1->socket_id)
										send(ClientsList[i]->socket_id,msg,strlen(msg),0); 
									memset(msg,0,1000);
								}
							
							}
						}
					
					}
					
					break;
				
			case 5:
					//case for QUIT quert
					printf("Client (%d): QUIT\n",cli1->socket_id);
					n=strlen(cli1->client_name);
					//If the client is not registered in the database closing the connection upon QUIT request by the client.
					if(n==0)		
					{	
			
						printf("Unable to identify Client location (%d) in Database. Disconnecting User.\n",cli1->socket_id);
						printf("Client (%d): Disconnecting  the User.\n",cli1->socket_id);
						close(cli1->socket_id); //closing the client socket connection.
						cli1=NULL;// freeing up the struct variable memory.
						pthread_exit(NULL);//closing  the thread upon quit.
					}
					else
					{
					
						printf("Client (%d): Disconnecting the User.\n",cli1->socket_id);
						//closing the client socket connection.
						for(j=0;j<max_clients;j++)   //Removing the client from the database when QUIT is requested
						{
							if(ClientsList[j])
							{
								if (strcmp(ClientsList[j]->client_name,cli1->client_name)==0)
								{	
							
									
									ClientsList[j]=NULL;
									break;
								}
							}
						}
						close(cli1->socket_id);//closing the client socket connection.
						cli1=NULL;//freeing up the client variable
						clients_joined--; //decreasing the clients joined.
						pthread_exit(NULL);
						
					}
					break;
					
					
			default: 
					//default case for non query messages and for unregistered clients.
					printf("Client (%d): Unrecognizable Message is received. Discarding UNKNOWN Message\n",cli1->socket_id);  
					n=strlen(cli1->client_name);
					if(n==0)  
						send(cli1->socket_id,"Unregistered User. Use \"JOIN <username>\" to Register.\n",strlen("Unregistered User. Use \"JOIN <username>\" to Register.\n"), 0); 			
							
					else 
						send(cli1->socket_id,"Unknown Message is received. Discarding UNKNOWN Message.\n",strlen("Unknown Message. Discarding UNKNOWN Message.\n") ,0);			
					
		
		}	
	
	}

}      



