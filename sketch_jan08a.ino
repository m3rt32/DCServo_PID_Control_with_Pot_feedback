double setPoint=585;
double girisSinyali,cikisSinyali;
double toplamHata,sonHata;
double kp,ki,kd;
int newData;
String konum="";
char girilen_deger;
double threshold=0;
int hesaplamaSuresi=1;//1sn
unsigned long lastTime;//Son Compute zamanı
unsigned long totalTime;
signed long int  darbe=0;
unsigned long last_time;
void setup() {
  pid_set(.01,0,30); //0.01 0 30 
 //threshold = (kp + ki +kd) * threshold;
  Serial.begin(9600);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(A0,INPUT);
  last_time = millis();
}

void loop() {
 if(Serial.available()) 
  {
    girilen_deger=Serial.read(); //char char okur
    if(girilen_deger=='s')
    {
      setPoint=konum.toInt();
      konum="";
    }
    else
    {
       konum=konum+String(girilen_deger);
     }
  }
  // put your main code here, to run repeatedly:
girisSinyali = analogRead(A0);
double error = Pid_Hesapla();
if(millis()-last_time>5){ //print the signal in every 5ms
  Serial.print(setPoint-threshold);
  Serial.print('\t');
  Serial.println(setPoint+error-threshold);
}
 double fark = setPoint-girisSinyali;
if(abs(error)<0.3){
  toplamHata=0;
  sonHata=0;
  digitalWrite(7,LOW);
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
}else if(error>=threshold){
 // Serial.print('H');
  //value is lower than target turn the motor
  digitalWrite(7,HIGH);
  digitalWrite(8,LOW);
  digitalWrite(9,HIGH);
}
else if(error<threshold){
  //Serial.print('m');
  digitalWrite(7,HIGH);
  digitalWrite(9,LOW);
  digitalWrite(8,HIGH);
}
}
double Pid_Hesapla(){
  unsigned long simdikiZaman = millis();
  totalTime+=simdikiZaman;
  double zamanfarki = (double)(simdikiZaman-lastTime);
  if(zamanfarki>=hesaplamaSuresi){
    //hatalari hesapla
  double hata = setPoint-girisSinyali;
  toplamHata +=(hata);
  double turevHata = (hata-sonHata);
  //Pid sinyalini hesapla
  cikisSinyali = kp*hata + ki*toplamHata + kd * turevHata;
  //Sonraki adim icin simdiki hata ve zamani kaydet
  sonHata = hata;
  lastTime = simdikiZaman;
  }
  return cikisSinyali;
}
void pid_set(double p,double i,double d){
  double anlikGainler = ((double)hesaplamaSuresi/1000);
  kp = p ;
  kd = d*anlikGainler;
  ki = i/anlikGainler;
}

