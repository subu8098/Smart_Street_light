#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#define FIREBASE_HOST "your_project_id.firebaseio.com"
#define FIREBASE_AUTH "your_firebase_auth_token"

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASSWORD "your_wifi_password"

// Define Firebase data object
FirebaseData firebaseData;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");

  // Initialize FirebaseConfig object
  FirebaseConfig firebaseConfig;
  firebaseConfig.host = FIREBASE_HOST;

  // Initialize FirebaseAuth object
  FirebaseAuth firebaseAuth;
  firebaseAuth.signer.tokens.legacy_token = FIREBASE_AUTH;

  // Initialize Firebase
  Firebase.begin(&firebaseConfig, &firebaseAuth);

  // Set database read timeout to 1 minute (optional)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
}

void loop() {
  // Example: Writing a string to Firebase
  String message = "Hello, Firebase!";
  Firebase.setString(firebaseData, "/messages", message);

  if (firebaseData.dataType() == "string") {
    Serial.print("String sent to Firebase: ");
    Serial.println(firebaseData.stringData());
  } else {
    Serial.println("Failed to send string to Firebase!");
  }

  delay(5000); // Wait for 5 seconds before next iteration
}
