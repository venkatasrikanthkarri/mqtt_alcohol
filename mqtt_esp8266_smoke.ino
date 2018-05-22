#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "hachi";
const char* password = "12345678";
const char* mqtt_server = "34.226.134.195";
int smoke = A0;//D1
int relay = 4;//D2
WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];
int value = 0;

void setup() {
  pinMode(smoke,INPUT);
  pinMode(relay, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("buddi", "Ready to publish");
      // ... and resubscribe
      client.subscribe("buddy");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  client.publish("buddi", msg);
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  value=analogRead(smoke);
  if(value>=120){

//     long now = millis();
    snprintf (msg, 75, "your alchohol levels are to much to drive the car");
    Serial.print("Publish message: ");
    Serial.println(msg);
    digitalWrite(relay,HIGH);
    delay(3000);
  }
  else{
    snprintf (msg, 75, "you can start the car");
    Serial.print("Publish message: ");
    Serial.println(msg);
    digitalWrite(relay,LOW);
    delay(3000);
    
  }
    
  }

 

