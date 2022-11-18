
#include<MsTimer2.h>

//--------변수---------//

int Ena = 6; int IN1 = 7; int IN2 = 8; // 핀번호 초기화
int ang; int pos;
int CW= 21; int CCW =20;
int cw =0; int ccw=0;
int ang_cw =0; int ang_ccw=0;

double vel;
double n =0; 
double current_pos, pre_pos, elapsed_pos, elapsed_pos_rms;
double elapsed_pos_square_sum = 0;

String readString;


int input = 0;
double current_Time, elapsed_Time, previous_Time;


double current_err = 0; double pre_err = 0; double sum_err =0; double delta_err = 0;
double diff_err = 0;

int u; int u_sat;
int u_max = 255; int u_min = 120;
double Kp = 12;
double Ki = 0.08;
double Kd = 0.2;
double Kaw = 0.2;


//----------코드---------//


 void setup() {
  // put your setup code here, to run once:
  
  pinMode(IN1, OUTPUT); pinMode(IN2 ,OUTPUT); pinMode(Ena, OUTPUT);
  pinMode(CW,INPUT); pinMode(CCW, INPUT);
  attachInterrupt(2, check_A, CHANGE);
  attachInterrupt(3, check_B, CHANGE);
  MsTimer2::set(50,timerISR);
  MsTimer2::start();
  Serial.begin(9600);
}



 void check_A() {     //A핀의 변화가 있을때 checking//
  cw = digitalRead(CW);
  ccw = digitalRead(CCW);
  if(cw != ccw) ang_cw++;
  if(cw == ccw) ang_ccw++;
}


 void check_B() {    //B핀의 변화가 있을떄 checking//
  cw=digitalRead(CW);
  ccw=digitalRead(CCW);
  if(cw != ccw) ang_ccw++;
  if(cw == ccw) ang_cw++;
}

//-------타이머 인터럽트-------//

 void timerISR() {
  n++; // 타이머 인터럽트 횟수
  
  //------속도 계산------//
  current_pos = pos;
  elapsed_pos = current_pos-pre_pos;
  pre_pos = pos;   
  if(abs(elapsed_pos)<10000){ 
    elapsed_pos_square_sum += elapsed_pos*elapsed_pos;
  }

//cur    pre   elapse
//50000 49000 1000
//-50000 50000 -100000
//-49000 -50000 1000
 
  if(input>0){
  elapsed_pos_rms = sqrt(elapsed_pos_square_sum/n);
  }
  else if (input ==0){
  elapsed_pos_rms = 0;
  }
  else{
  elapsed_pos_rms = -sqrt(elapsed_pos_square_sum/n);
  //elapsed_pos_rms = sqrt(elapsed_pos_square_sum/n);
  }

  vel = 500*elapsed_pos_rms/50; // rpm으로 변환 60*1000*edge/t*ppr*4

  if(Serial.available()>0){ //Serial 플로터에 입력되는 값이 있으면 elapsed_pos_square_sum을 초기화 // elapsed_pos가 제곱되어 누적되기 때문에 값이 입력되어도 입력값으로 바로 가지 않는다.
    n=1;
    elapsed_pos_square_sum = 0;
  }

  //---------PID계산----------//

  if(u>0) {
  if(u >= u_max) u_sat =u_max;
  else if( u < u_max && u_min<u) u_sat = u; 
  else u_sat=u_min;
  }
  else {
  if(u <= -u_max) u_sat = -u_max;
  else if( u > -u_max && -u_min> u) u_sat = u;
  else u_sat= -u_min;
  }
  current_Time = millis();
  elapsed_Time = current_Time - previous_Time;
  previous_Time = current_Time;
  
  current_err = input - vel;
  delta_err = current_err - pre_err;
  pre_err = current_err;
  sum_err += (current_err-Kaw*(u-u_sat))*elapsed_Time;
  diff_err = delta_err / abs(elapsed_Time);
  
  u= (int)(Kp*current_err + Ki*sum_err + Kd*diff_err);


  //------ 속도 값 입력-------// 
 while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString
    delay(2);  // allow buffer to fill with next character
    }
  if (readString.length() >0) {
    input = readString.toInt();
    //Serial.println(input);
    readString=""; //empty for next input
 }
  
}


 void loop() {
  pos = -ang_ccw + ang_cw;



// 아마 시계방향
if(input>0){
 digitalWrite(IN1,HIGH); digitalWrite(IN2, LOW);
 analogWrite(Ena, u_sat);
}
 
// 아마 반시계??
else{
 digitalWrite(IN1,LOW); digitalWrite(IN2,HIGH);
 analogWrite(Ena, -u_sat);
}

  Serial.println(vel);
  Serial.print(',');
  Serial.println(input);
  //Serial.print(',');
  //Serial.println(vel);
 //Serial.println(elapsed_pos);
 //Serial.print(',');
 //Serial.println(elapsed_pos_rms);

}
