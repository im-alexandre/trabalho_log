#include <SPI.h>
#include <MFRC522.h>

//Variáveis para comunicação entre arduino e módulo RFID
#define RST_PIN         9          
#define SS_1_PIN        10       
#define SS_2_PIN        8        

#define NR_OF_READERS   2

//definindo objeto (instância) de comunicação
byte ssPins[] = {SS_1_PIN, SS_2_PIN};
MFRC522 mfrc522[NR_OF_READERS];  

//Variáveis para comparação com a tag lida
float valor = 0;
String N_identidade    = "14695174";
String Militar       = "1T(IM) Alexandre";
String N_produto[] =  {"9915716489"    , "16115459217"   ,  "4311036217"   , "6624139131"};
String produto[]  =   {"Feijão(KG)" , "Arroz(5KG)"  ,  "Whiskey(700ml)" , "coca-cola(3L)"};
float  preco []   =   {5.00         , 4.50          ,  137.93             , 10.00      };
String codigo;
int tempo;
int contador = 0;
int tempo_de_resposta = 6; //tempo em segundos


void setup() {

  
  while (!Serial);    

  SPI.begin();        

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); 
    mfrc522[reader].PCD_DumpVersionToSerial();
  }
  Serial.begin(9600);

  Serial.print("                              MARINHA DO BRASIL");
  Serial.print("\n");
  Serial.print("\n");
  Serial.print("      CENTRO DE INSTRUÇÃO ALMIRANTE NEWTON BRAGA");
  Serial.print("\n");
  Serial.print("\n");
  
}



void loop() {
  codigo = "";
   for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
     if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {
      //Serial.print(F("Card UID "));
      dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);
//      Serial.print("\n");
      MFRC522::PICC_Type piccType = mfrc522[reader].PICC_GetType(mfrc522[reader].uid.sak);
      mfrc522[reader].PICC_HaltA();
      mfrc522[reader].PCD_StopCrypto1();
    }
  }
  autorizacao();
  ler_produtos();
  

}


void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
      Serial.print(buffer[i]);
      codigo += buffer[i];
  }
}

void autorizacao (){
    if (codigo == N_identidade){
      Serial.print("\n");
      Serial.print("Saída realizada pelo: ");
      Serial.print(Militar);
      Serial.print("\n");
      Serial.print("Valor Total: ");
      Serial.print("R$");
      Serial.print(float(valor));
      Serial.print("\n");
      valor = 0;
      Serial.print("x\n");

    }
    }

void ler_produtos (){
  for (int i = 0; i < 4; i++){
    if (codigo == N_produto[i]){
//      Serial.print("\n");
//      Serial.print("\n");
      Serial.print(produto [i]);
      Serial.print("\n");
      Serial.print ("valor");
      Serial.print ("= R$");
      Serial.print (preco[i]);
      Serial.print ("\n");
//      Serial.print ("\n");/
      valor += preco[i];
//      Serial.print("Subtotal: R$");/
//      Serial.print(valor);/
//      Serial.print("\n");/
//      Serial.print("\n");/
      Serial.print("==========");
      Serial.print("\n");      
  }
}


}
