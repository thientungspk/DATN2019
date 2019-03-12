#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <DHT.h>
#include <SerialCommand.h>  // Thêm vào sketch thư viện Serial Command

#define COI     13 
#define RELAY1  4
#define RELAY2  5      
#define RELAY3  6
#define RELAY4  7     
#define DHTPIN  8           // chan cam bien nhiet do va do am
const byte RX = 3;          // Chân 3 được dùng làm chân RX
const byte TX = 2;          // Chân 2 được dùng làm chân TX

SoftwareSerial mySerial = SoftwareSerial(RX, TX); 
SerialCommand sCmd(mySerial);              // Khai báo biến sử dụng thư viện Serial Command
int TT_RELAY1 = 0;                         // khai báo biến
int TT_RELAY2 = 0;                         // khai báo biến
int TT_RELAY3 = 0;                         // khai báo biến
int TT_RELAY4 = 0;                         // khai báo biến
int TT_TATCA = 0;                          // khai báo biến
float h = 0;
float t = 0;
const unsigned long chu_ky_lap_lai = 100UL;  //Cứ sau 100ms = 0.1s thì chu kỳ lặp lại
unsigned long chuky1 = 0;
DHT dht(DHTPIN, DHT22);
 //################################################################################################
void setup() {
  Serial.begin(57600);      //Khởi tạo Serial ở baudrate 57600 để debug ở serial monitor
  mySerial.begin(57600);    //Khởi tạo Serial ở baudrate 57600 cho cổng Serial thứ hai, dùng cho việc kết nối với ESP8266
  
  
  pinMode(RELAY1,OUTPUT);            //pinMode 4  là OUTPUT
  pinMode(RELAY2,OUTPUT);           
  pinMode(RELAY3,OUTPUT);            
  pinMode(RELAY4,OUTPUT);      
  pinMode(COI,OUTPUT);        
  
  digitalWrite(RELAY1,1);   // khi khởi động thì tắt hết
  digitalWrite(RELAY2,1);   
  digitalWrite(RELAY3,1);  
  digitalWrite(RELAY4,1);  
  digitalWrite(COI,1);  
  
  TT_RELAY1 = 0; 
  TT_RELAY2 = 0;
  TT_RELAY3 = 0;
  TT_RELAY4 = 0;
  TT_TATCA  = 0;
  
  dht.begin();                                   // bắt đầu cảm biến
  // Một số hàm trong thư viện Serial Command
  sCmd.addCommand("NRL1", ngat_relay_1);
  sCmd.addCommand("DRL1", dong_relay_1);        //Khi có lệnh DRL1 gởi đến thì sẽ thực thi thuc hien ham dong_relay_1
  sCmd.addCommand("NRL2", ngat_relay_2);
  sCmd.addCommand("DRL2", dong_relay_2); 
  sCmd.addCommand("NRL3", ngat_relay_3);
  sCmd.addCommand("DRL3", dong_relay_3); 
  sCmd.addCommand("NRL4", ngat_relay_4);
  sCmd.addCommand("DRL4", dong_relay_4);  
  sCmd.addCommand("NTC",  ngat_tatca);
  sCmd.addCommand("DTC",  dong_tatca);  
  sCmd.addCommand("CNTB", phan_hoi);
  sCmd.addCommand("CNCB", nhietdo_doan); 
  Serial.println("Da san sang nhan lenh");
}
//###################################################################################################
void loop() {
    delay(1000);
    if (millis() - chuky1 > chu_ky_lap_lai) {   //Khởi tạo một chu kỳ lệnh, chu kỳ là 2000ms giống như delay
    chuky1 = millis();
    nhietdo_doan();
    //phan_hoi();
    }
  sCmd.readSerial();                            //đọc các chuỗi gởi tới arduino
  }
//###################################################################################################
void phan_hoi(){                                //hàm phản hồi để hiện lên web
  StaticJsonBuffer<200> jsonBuffer;             //tạo một môi trường lưu chuỗi json
  JsonObject& root = jsonBuffer.createObject(); //tạo biến root mang kiểu json
  root["tt_relay1"] =  TT_RELAY1;               // tạo một phần tử thứ nhất có tên tt_relay1 trong root gán giá trị là biến TT_RELAY1
  root["tt_relay2"] =  TT_RELAY2;               // tương tự
  root["tt_relay3"] =  TT_RELAY3;               // tương tự
  root["tt_relay4"] =  TT_RELAY4;               // tương tự
  root["tt_tatca"] =   TT_TATCA;                // tương tự
  
  mySerial.print("P_HOI");          //gửi tên lệnh
  mySerial.print('\r');             // gửi \r
  root.printTo(mySerial);           //gửi chuỗi JSON
  mySerial.print('\r');             // gửi \r
  root.printTo(Serial);             //Xuống dòng
  }
 //#########################################################
void dong_tatca() {
  digitalWrite(RELAY1,0);    //xuất mức thấp để đóng relay
    digitalWrite(RELAY2,0);    //xuất mức thấp để đóng relay
      digitalWrite(RELAY3,0);    //xuất mức thấp để đóng relay
        digitalWrite(RELAY4,0);    //xuất mức thấp để đóng relay
        TT_RELAY1 = 1;
        TT_RELAY2 = 1;
        TT_RELAY3 = 1;
        TT_RELAY4 = 1;
        TT_TATCA = 1;             //báo trạng thái
        phan_hoi();
  }
void ngat_tatca() {
  digitalWrite(RELAY1,1);    //xuất mức thấp để đóng relay
    digitalWrite(RELAY2,1);    //xuất mức thấp để đóng relay
      digitalWrite(RELAY3,1);    //xuất mức thấp để đóng relay
        digitalWrite(RELAY4,1);    //xuất mức thấp để đóng relay
        TT_RELAY1 = 0;
        TT_RELAY2 = 0;
        TT_RELAY3 = 0;
        TT_RELAY4 = 0;
        TT_TATCA = 0;             //báo trạng thái
        phan_hoi();
  }
 //#########################################################
void dong_relay_1() {
  digitalWrite(RELAY1,0);    //xuất mức thấp để đóng relay
  TT_RELAY1 = 1;             //báo trạng thái
  phan_hoi();
  }
void ngat_relay_1() {
  digitalWrite(RELAY1,1);    //xuất mức cao để ngắt relay
  TT_RELAY1 = 0;             //báo trạng thái
  phan_hoi();
  }
//############################################################
void dong_relay_2() {
  digitalWrite(RELAY2,0);    //xuất mức thấp để đóng relay
  TT_RELAY2 = 1;             //báo trạng thái
  phan_hoi();
  }
void ngat_relay_2() {
  digitalWrite(RELAY2,1);    //xuất mức cao để ngắt relay
  TT_RELAY2 = 0;             //báo trạng thái
  phan_hoi();
  }
//############################################################
void dong_relay_3() {
  digitalWrite(RELAY3,0);    //xuất mức thấp để đóng relay
  TT_RELAY3 = 1;             //báo trạng thái
  phan_hoi();
  }
void ngat_relay_3() {
  digitalWrite(RELAY3,1);    //xuất mức cao để ngắt relay
  TT_RELAY3 = 0;             //báo trạng thái
  phan_hoi();
  }
//############################################################
void dong_relay_4() {
  digitalWrite(RELAY4,0);    //xuất mức thấp để đóng relay
  TT_RELAY4 = 1;             //báo trạng thái
  phan_hoi();
  }
void ngat_relay_4() {
  digitalWrite(RELAY4,1);    //xuất mức cao để ngắt relay
  TT_RELAY4 = 0;             //báo trạng thái
  phan_hoi();
  }
 //############################################################
void nhietdo_doan(){
    h = dht.readHumidity();       //đọc cảm biến độ ẩm
    t = dht.readTemperature();    //đọc cảm biến nhiệt độ
    StaticJsonBuffer<200> jsonBuffer;   //tạo một môi trường lưu chuỗi json
    JsonObject& root = jsonBuffer.createObject(); //tạo biến root mang kiểu json
    //đọc giá trị cảm biến rồi in ra root
    root["do_am"] = h;                // tạo một phần tử thứ nhất có tên do_am trong root gán giá trị là h
    root["nhiet_do"] = t;             // tạo một phần tử thứ hai có tên nhiet_do trong root gán giá trị là t
    
    mySerial.print("CBIEN");          //gửi tên lệnh
    mySerial.print('\r');             // gửi \r
    root.printTo(mySerial);           //gửi chuỗi JSON
    mySerial.print('\r');             // gửi \r
    root.printTo(Serial);             //Xuống dòng
    }







