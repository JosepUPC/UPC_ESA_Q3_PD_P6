#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <MFRC522.h>

#define RST_PIN	27    //Pin 9 para el reset del RC522
#define SS_PIN	15   //Pin 10 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); //Creamos el objeto para el RC522
File myFile;
String UID;

void setup() {
	Serial.begin(115200); //Iniciamos la comunicación  serial

        Serial.print("Iniciando SD ...");
        if (!SD.begin(5)) {
        Serial.println("No se pudo inicializar");
        return;
        }
        Serial.println("inicializacion exitosa");

	SPI.begin();        //Iniciamos el Bus SPI
	mfrc522.PCD_Init(); // Iniciamos  el MFRC522
	Serial.println("Lectura del UID");
}

void loop() {
	// Revisamos si hay nuevas tarjetas  presentes
	if ( mfrc522.PICC_IsNewCardPresent()) 
        {
                UID="";
                for (byte i = 0; i < mfrc522.uid.size; i++) {
                          UID=UID+(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                          UID=UID+(mfrc522.uid.uidByte[i], HEX);   
                }
  		//Seleccionamos una tarjeta
            if ( mfrc522.PICC_ReadCardSerial()) 
            {
                
                  // Enviamos serialemente su UID
                  myFile = SD.open("/archivo.txt");//abrimos  el archivo 
                if (myFile) {
                        Serial.println("archivo.txt:");
                        Serial.print("Card UID:");
                        Serial.print(UID);
                        Serial.println();
                  // Terminamos la lectura de la tarjeta  actual
                  mfrc522.PICC_HaltA();  
                while (myFile.available()) {
                        myFile.println(UID);
    	                Serial.write(myFile.getLastWrite());
                }
                myFile.close(); //cerramos el archivo
                }
                else {
                        Serial.println("Error al abrir el archivo");
                }       
            }      
	}	
}