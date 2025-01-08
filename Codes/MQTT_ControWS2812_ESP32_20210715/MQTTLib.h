#include <PubSubClient.h>
#include <MQTT.h>
WiFiClient mqclient;
PubSubClient mqttclient(mqclient) ;
char clintid[20];
String TopicHead  = String("ncnu/ledlight/") ;
String Topic;
char SubTopic[100];
const char* dataptr; 
int intdataptr ;










//---------------
void fillCID(String mm)
{
  // generate a random clientid based MAC
  //compose clientid with "tw"+MAC 
  clintid[0]= 't' ;  
  clintid[1]= 'w' ;  
      mm.toCharArray(&clintid[2],mm.length()+1) ;
    clintid[2+mm.length()+1] = '\n' ;

}
