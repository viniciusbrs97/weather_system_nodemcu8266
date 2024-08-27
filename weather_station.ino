

#include <Wire.h>
#include <Adafruit_ADS1015.h> //Lib conversor analogico
#include <OneWire.h> //Lib inclusa sensor temp
#include <DallasTemperature.h> //Lib Sensor Temp
#include <DHT.h>
#include <virtuabotixRTC.h>   
#include <SD.h>


//Parâmetros para obter horas.
int currentHour; //Variavel para as horas
int currentMinute; //Variavel para os minutos
int currentSecond; //Variavel para os segundos
int dia;
int mes;
int ano;
String data= "100"; //String concatenando H+M+S
String hora;
#define   clk   D1
#define   dat   D2
#define   rst   D0
virtuabotixRTC   myRTC(clk, dat, rst);

#define chipSelect D8


int ref;
int counter=0;


float arr_temp[12];

float arr_umiR_ar[12];
float arr_temp_dht11[12];

float arr_umiR_ar_dht22[12];
float arr_temp_dht22[12];

           
//============#CRIAÇÃO DAS CLASSES DOS SENSORES E DEFINIÇÃO DOS PINOS NA PLACA #=======//
  #define DHTPIN 3 //pino dht11
  #define DHTTYPE DHT11 //define o tipo do sensor
  #define DHTTYPE22 DHT22
   
  DHT dht(DHTPIN, DHTTYPE);
  
  DHT dht22(DHTPIN, DHTTYPE22);
  
  #define ONE_WIRE_BUS D3 //PINO SENSOR TEMPERATURA DS18B20
  OneWire oneWire(ONE_WIRE_BUS); //Instancia sensor temp
  DallasTemperature sensors(&oneWire);//Objeto sensor temp

  Adafruit_ADS1115 ads(0x48); //Objeto ADs ADC


  

  
//==============================================================================//
  
   //Protótipo das funções
  
  float temp();

  float umiR_ar();

  void send_info();

  void data_log();
  
  void log_write(String val);

  void check_counter();
  void time_stamp();
//===============================================================================//

//Varíaveis do cartão SD
  File dataFile; //log de 15 em 15min
  File logMedia; //log com as médias dentro do periodo de 1 hora
 size_t k=0; //contador bkp log 15min
 size_t k_M=0; //contador bkp log médio
 String BKP[3]; //array salvar varíaveis log 15min
 String BKP_M[3]; //array salvar varíaveis log médio.
 String LogStart={" DATA (D/M/A),HORA:MIN,TEMPERATURA DS18B20(°C),UMIDADE RELATIVA DO AR DHT11 (%), UMIDADE RELATIVA DO AR DHT22 (%), TEMPERATURA DHT11(°C),  TEMPERATURA DHT22(°C) "}; //cabeçalho sd log 15min
  String val_Ambientais;
                   
//=======================================================
void setup() {
  sensors.begin(); // inicializa o sensor de temperatura ds18b20
  dht.begin(); //inicializa o dht (umidade e temp) dht11
  dht22.begin();//inicializa o dht22 (umidade e temp) dht11
 //================================================================================== 
    Serial.begin(115200);
    //Conecta na rede wi-fi

    //===============================
//     Inicialização do cartão SD
    Serial.print("Initializing SD card...");

 /// see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
//     don't do anything more:
    return;
  }
 Serial.println("card initialized.");

 if(SD.exists("log.txt")){
  Serial.println("Arquivo Existente");
  return;
 }else if(!SD.exists("log.txt")){
  dataFile= SD.open("log.txt", FILE_WRITE);
  dataFile.println(LogStart);
  dataFile.close();
 }



}

void loop() {
 myRTC.updateTime();
 time_stamp();   
 
 //arr_umiR_ar_dht22[0] = umiR_ar_dht22();
// arr_temp_dht22[0] = temp_dht22();
      
       //     Serial.println(arr_umiR_ar_dht22[0]);
        //   Serial.println(arr_temp_dht22[0]);
           delay(1000);
    //Funções desenvolvidas
}

//ds18b20
    float temp(){
      sensors.requestTemperatures();
      float temp= sensors.getTempCByIndex(0);
      //Serial.print("Temperatura:");
      //Serial.println(temp);
      return temp;
    }
//dht 11  umidade
     float umiR_ar(){ 
       float h = dht.readHumidity();
       //Serial.print("Umidade relativa do ar: ");
      // Serial.println(h);
      delay(250);
       return h;
     }
     
//dht 22  umidade
     float umiR_ar_dht22(){ 
       float c = dht22.readHumidity();
       //Serial.print("Umidade relativa do ar: ");
      // Serial.println(h);
      delay(250);
       return c;
     }
     //dht 11  temp
          float temp_dht11(){ 
       float t = dht.readTemperature();
       //Serial.print("Umidade relativa do ar: ");
      // Serial.println(h);
      delay(250);
       return t;
     }
     //dht 22  temp     
          float temp_dht22(){ 
       float l = dht22.readTemperature();
       //Serial.print("Umidade relativa do ar: ");
      // Serial.println(h);
      delay(250);
       return l;
     }


 void time_stamp(){ 
         currentHour = myRTC.hours;
         currentMinute = myRTC.minutes;         
         //currentSecond= myRTC.seconds;
         dia=myRTC.dayofmonth;
         mes=myRTC.month;
         ano=myRTC.year;
         data= String(dia)+"/"+String(mes)+"/"+String (ano);
         hora= String(currentHour)+":"+String(currentMinute);
        int horaEmin_min= (currentHour* 60) + (currentMinute);//ok
        if(horaEmin_min==0){
          horaEmin_min= 1440;
        }
       Serial.print("HORA E MIN EM MINUTOS: ");
       Serial.println(horaEmin_min);
        delay(10000);
        
      int inter_medioQ1[]={0,65,125,185,245,305,365,425,485,545,605,665,725,785,
                            845,905,965,1025,1085,1145,1205,1265,1325,1385
                            };
  
      int inter_medioQ2[]={10,70,130,190,250,310,370,430,490,550,610,670,730,
                            790,850,910,970,1030,1090,1150,1210,1270,1330,1390};
                            
      int inter_medioQ3[]={15,75,135,195,285,345,405,465,525,585,645,705,765,825,
                            885,945,1005,1065,1125,1185,1245,1305,1365,1425};

      int inter_medioQ4[]={20,80,140,200,260,320,380,440,500,560,620,680,740,
                            800,860,920,980,1040,1100,1160,1220,1280,1340,1400};
                            
      int inter_medioQ5[]={25,85,145,205,265,325,385,445,505,565,625,685,745,805,
                            865,925,985,1045,1105,1165,1225,1285,1345,1405
                            };
  
      int inter_medioQ6[]={30,90,150,210,270,330,390,450,510,570,630,690,750,
                            810,870,930,990,1050,1110,1170,1230,1290,1350,1410};
                            
      int inter_medioQ7[]={35,95,155,215,275,335,395,455,515,575,635,695,755,815,
                            875,935,995,1055,1115,1175,1235,1295,1355,1415};

      int inter_medioQ8[]={40,100,160,220,280,340,400,460,520,580,640,700,760,
                            820,880,940,1000,1060,1120,1200,1260,1320,1380,1420};
                            
      int inter_medioQ9[]={45,105,165,225,285,345,405,465,525,585,645,705,765,825,
                            885,945,1005,1065,1125,1185,1245,1305,1365,1425
                            };
  
      int inter_medioQ10[]={50,110,170,230,290,350,410,470,530,590,650,710,770,
                            830,890,950,1010,1070,1130,1190,1250,1310,1370,1430};
                            
      int inter_medioQ11[]={55,115,175,235,295,355,415,475,535,595,655,715,775,835,
                            895,955,1015,1075,1135,1095,1255,1315,1375,1435};

      int inter_medioQ12[]={1440,60,120,180,240,300,360,420,480,540,600,660,720,
                            780,840,900,960,1020,1080,1140,1200,1260,1320,1380};

           for(size_t i=0;i<=23;i++){
            
            if(inter_medioQ1[i]==horaEmin_min){
             ref= inter_medioQ1[i];
            arr_temp[0]=temp();
            
            arr_umiR_ar[0]=umiR_ar();   
            arr_temp_dht11[0]= temp_dht11();      
            delay(500);
            arr_umiR_ar_dht22[0]= umiR_ar_dht22();
            arr_temp_dht22[0]= temp_dht22();
             counter++;
            String val_Ambientais=data+","+hora+","+String(arr_temp[0]) + ","+String(arr_umiR_ar[0])+","+String( arr_umiR_ar_dht22[0])+","+String(arr_temp_dht11[0])+","+String( arr_temp_dht22[0]);
             log_write(val_Ambientais);
            delay(50000);
            return;
              } 
              else if(inter_medioQ2[i]==horaEmin_min && (inter_medioQ2[i]- ref) ==5){
            arr_temp[1]=temp();
            arr_umiR_ar[1]=umiR_ar();   
            arr_temp_dht11[1]= temp_dht11();      
            delay(500);
            arr_umiR_ar_dht22[1]= umiR_ar_dht22();
            arr_temp_dht22[1]= temp_dht22();
             counter++;
            val_Ambientais=data+","+hora+","+String(arr_temp[1]) + ","+String(arr_umiR_ar[1])+","+String(arr_umiR_ar_dht22[1])+","+String(arr_temp_dht11[1])+","+String(arr_temp_dht22[1]);
            log_write(val_Ambientais);
           // send_fb(datafb,hora , horaEmin_min,arr_luz[1],arr_solo[1],  arr_temp[1],arr_umiR_ar[1]);
            delay(50000);
               return;
           } 
           else if(inter_medioQ3[i]==horaEmin_min && (inter_medioQ3[i]- ref ==10)){
            arr_temp[2]=temp();
            arr_umiR_ar[2]=umiR_ar();   
            arr_temp_dht11[2]= temp_dht11();      
            delay(500);
            arr_umiR_ar_dht22[2]= umiR_ar_dht22();
            arr_temp_dht22[2]= temp_dht22();
             counter++;
            val_Ambientais=data+","+hora+","+String(arr_temp[2]) + ","+String(arr_umiR_ar[2])+","+String(arr_umiR_ar_dht22[2])+","+String(arr_temp_dht11[2])+","+String(arr_temp_dht22[2]);
             log_write(val_Ambientais);
            // send_fb(datafb,hora , horaEmin_min,arr_luz[2],arr_solo[2],  arr_temp[2],arr_umiR_ar[2]);
            delay(50000);
               return;
         }
         else if(inter_medioQ4[i]==horaEmin_min && ( inter_medioQ4[i]-ref== 15) ){
             arr_temp[3]=temp();
            arr_umiR_ar[3]=umiR_ar();   
            arr_temp_dht11[3]= temp_dht11();      
            delay(500);
            arr_umiR_ar_dht22[3]= umiR_ar_dht22();
            arr_temp_dht22[3]= temp_dht22();
             counter++;
           val_Ambientais=data+","+hora+","+String(arr_temp[3]) + ","+String(arr_umiR_ar[3])+","+String(arr_umiR_ar_dht22[3])+","+String(arr_temp_dht11[3])+","+String(arr_temp_dht22[3]);
            log_write(val_Ambientais);
             delay(50000);
               return;
          }
          else if(inter_medioQ5[i]==horaEmin_min && ( inter_medioQ5[i]-ref== 20) ){
            arr_temp[4]=temp();
            arr_umiR_ar[4]=umiR_ar();   
            arr_temp_dht11[4]= temp_dht11();      
            delay(500);
            arr_umiR_ar_dht22[4]= umiR_ar_dht22();
            arr_temp_dht22[4]= temp_dht22();
             counter++;
           val_Ambientais=data+","+hora+","+String(arr_temp[4]) + ","+String(arr_umiR_ar[4])+","+String(arr_umiR_ar_dht22[4])+","+String(arr_temp_dht11[4])+","+String(arr_temp_dht22[4]);
            log_write(val_Ambientais);
             delay(50000);
               return;
           }
           else if(inter_medioQ6[i]==horaEmin_min && ( inter_medioQ6[i]-ref== 25) ){
            arr_temp[5]=temp();
            arr_umiR_ar[5]=umiR_ar();   
            arr_temp_dht11[5]= temp_dht11();      
            delay(500);
            arr_umiR_ar_dht22[5]= umiR_ar_dht22();
            arr_temp_dht22[5]= temp_dht22();
             counter++;
           val_Ambientais=data+","+hora+","+String(arr_temp[5]) + ","+String(arr_umiR_ar[5])+","+String(arr_umiR_ar_dht22[5])+","+String(arr_temp_dht11[5])+","+String(arr_temp_dht22[5]);
            log_write(val_Ambientais);
             delay(50000);
               return;
           }
           else if(inter_medioQ7[i]==horaEmin_min && ( inter_medioQ7[i]-ref== 30) ){
            arr_temp[6]=temp();
            arr_umiR_ar[6]=umiR_ar();   
            arr_temp_dht11[6]= temp_dht11();      
            delay(500);
            arr_umiR_ar_dht22[6]= umiR_ar_dht22();
            arr_temp_dht22[6]= temp_dht22();
             counter++;
           val_Ambientais=data+","+hora+","+String(arr_temp[6]) + ","+String(arr_umiR_ar[6])+","+String(arr_umiR_ar_dht22[6])+","+String(arr_temp_dht11[6])+","+String(arr_temp_dht22[6]);
            log_write(val_Ambientais);
             delay(50000);
               return;
           }
           else if(inter_medioQ8[i]==horaEmin_min && ( inter_medioQ8[i]-ref== 35) ){
            arr_temp[7]=temp();
            arr_umiR_ar[7]=umiR_ar();   
            arr_temp_dht11[7]= temp_dht11();      
            delay(500);
            arr_umiR_ar_dht22[7]= umiR_ar_dht22();
            arr_temp_dht22[7]= temp_dht22();
             counter++;
           val_Ambientais=data+","+hora+","+String(arr_temp[7]) + ","+String(arr_umiR_ar[7])+","+String(arr_umiR_ar_dht22[7])+","+String(arr_temp_dht11[7])+","+String(arr_temp_dht22[7]);
            log_write(val_Ambientais);
             delay(50000);
               return;
            }
            else if(inter_medioQ9[i]==horaEmin_min && ( inter_medioQ9[i]-ref== 40) ){
              arr_temp[8]=temp();
            arr_umiR_ar[8]=umiR_ar();   
            arr_temp_dht11[8]= temp_dht11();      
            delay(500);
            arr_umiR_ar_dht22[8]= umiR_ar_dht22();
            arr_temp_dht22[8]= temp_dht22();
             counter++;
           val_Ambientais=data+","+hora+","+String(arr_temp[8]) + ","+String(arr_umiR_ar[8])+","+String(arr_umiR_ar_dht22[8])+","+String(arr_temp_dht11[8])+","+String(arr_temp_dht22[8]);
            log_write(val_Ambientais);
             delay(50000);
               return;
            }
            else if(inter_medioQ10[i]==horaEmin_min && ( inter_medioQ10[i]-ref== 45) ){
              arr_temp[9]=temp();
            arr_umiR_ar[9]=umiR_ar();   
            arr_temp_dht11[9]= temp_dht11();      
            delay(500);
            arr_umiR_ar_dht22[9]= umiR_ar_dht22();
            arr_temp_dht22[9]= temp_dht22();
             counter++;
           val_Ambientais=data+","+hora+","+String(arr_temp[9]) + ","+String(arr_umiR_ar[9])+","+String(arr_umiR_ar_dht22[9])+","+String(arr_temp_dht11[9])+","+String(arr_temp_dht22[9]);
            log_write(val_Ambientais);
             delay(50000);
               return;
            }
            else if(inter_medioQ11[i]==horaEmin_min && ( inter_medioQ11[i]-ref== 50) ){
              arr_temp[10]=temp();
            arr_umiR_ar[10]=umiR_ar();   
            arr_temp_dht11[10]= temp_dht11();      
            delay(500);
            arr_umiR_ar_dht22[10]= umiR_ar_dht22();
            arr_temp_dht22[10]= temp_dht22();
             counter++;
           val_Ambientais=data+","+hora+","+String(arr_temp[10]) + ","+String(arr_umiR_ar[10])+","+String(arr_umiR_ar_dht22[10])+","+String(arr_temp_dht11[10])+","+String(arr_temp_dht22[10]);
            log_write(val_Ambientais);
             delay(50000);
               return;
            }
            else if(inter_medioQ12[i]==horaEmin_min && ( inter_medioQ12[i]-ref== 55) ){
              arr_temp[11]=temp();
            arr_umiR_ar[11]=umiR_ar();   
            arr_temp_dht11[11]= temp_dht11();      
            delay(500);
            arr_umiR_ar_dht22[11]= umiR_ar_dht22();
            arr_temp_dht22[11]= temp_dht22();
             counter++;
           val_Ambientais=data+","+hora+","+String(arr_temp[11]) + ","+String(arr_umiR_ar[11])+","+String(arr_umiR_ar_dht22[11])+","+String(arr_temp_dht11[11])+","+String(arr_temp_dht22[11]);
            log_write(val_Ambientais);
            check_counter();
             delay(50000);
               return;
            }
           }
           }
                              
                
          void check_counter(){
            if(counter==12){
              for(size_t i=0;i<=12;i++){
               
            arr_temp[i]={0};
            arr_umiR_ar[i]={0};         
            arr_umiR_ar_dht22[i]={0};
            arr_temp_dht11[i]={0};
            arr_temp_dht22[i]={0};

             }
             counter=0;
            }
          }

          void log_write(String val){
            String nobug= val;          
            if (SD.begin(chipSelect)==0) {//TRUE É IGUAL A SEM O CARTÃO SD.
             BKP[k]= nobug;
            k++;
            Serial.println("BKP SALVO NO ARRAY");
            return;
            }
             if(k>0 && SD.begin(chipSelect)==1 ){
              for(size_t j=0;j<k;++j){
                Serial.println("BKP ENVIADO PARA O CARTÃO");
               dataFile= SD.open("log.txt", FILE_WRITE);
               dataFile.println(BKP[j]);
               dataFile.close();
               BKP[j]={""};
               k=0;
               }
               }
                if(SD.begin(chipSelect)==1){ 
                Serial.println("GRAVANDO NO SD SEM NECESSIDADE DO BKP");
                dataFile= SD.open("log.txt", FILE_WRITE);
                dataFile.println(nobug);
                dataFile.close();
                return;}
          }
         

          


 
