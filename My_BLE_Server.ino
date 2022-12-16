


#include <BLEDevice.h>
#include <BLEUtils.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


class MyCallbacks: public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string value = pCharacteristic->getValue();

    if (value.length() > 0)
    {
      Serial.println("*********");
      Serial.print("New value: ");
      for (int i = 0; i < value.length(); i++)
      {
        Serial.print(value[i]);
      }

      Serial.println();
      Serial.println("*********");
    }
  }
};

 BLEService *pService;
 BLECharacteristic *pCharacteristic;

void setup()
{
  Serial.begin(115200);

  Serial.println("1- Download and install an BLE scanner app in your phone");
  Serial.println("2- Scan for BLE devices in the app");
  Serial.println("3- Connect to ESP32-BLE_Server");
  Serial.println("4- Go to CUSTOM CHARACTERISTIC in CUSTOM SERVICE and write something");
  Serial.println("5- See the magic =)");

  BLEDevice::init("ESP32-BLE-Server");
  BLEServer *pServer = BLEDevice::createServer();

 pService = pServer->createService(SERVICE_UUID);

 pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("Hello World");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void loop()
{


  BLEScan *scan = BLEDevice::getScan(); //initializing an instance of the scan
  scan->setActiveScan(true);// call setActiveScan function
  BLEScanResults results = scan->start(1); // get scan results


  for (int i = 0; i < results.getCount(); i++) { //loop through the scan results
    BLEAdvertisedDevice device = results.getDevice(i); //instantiate the device
    int rssi = device.getRSSI(); // get the RSSI value

    int dist = rssi;
    char rsString[7];
    dtostrf(dist, 6, 2, rsString);
    Serial.print("RSSI: ");
    Serial.println(rssi);
    Serial.print("Distance: ");
    Serial.println(dist);


    pCharacteristic->setValue(rsString);
    
    
  }
  delay(2000);
}

float get_distance(int rssi_val){
  int distance;
  float sec_parameter;
  sec_parameter=(-12-(rssi_val))/(10*3);
  distance= pow(10, sec_parameter);
  return distance;
}
