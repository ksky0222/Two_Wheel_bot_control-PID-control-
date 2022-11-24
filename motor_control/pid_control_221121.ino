#include<MsTimer2.h>

//--------변수---------//

int Ena = 6; int IN1 = 7; int IN2 = 8; 
int Enb = 9; int IN3 = 10; int IN4 = 11; 

int ang; int pos;

int R_CW = 19; int R_CCW = 18;
int L_CW = 21; int L_CCW = 20;

int R_cw =0; int R_ccw=0;
int L_cw =0; int L_ccw=0;

int R_ang_cw =0; int R_ang_ccw=0;
int L_ang_cw =0; int L_ang_ccw=0;

double n =0; 
int number = 0;

double R_current_pos = 0; double R_pre_pos = 0; double R_elapsed_pos, R_elapsed_pos_rms;
double R_elapsed_pos_square_sum = 0;

double L_current_pos = 0; double L_pre_pos = 0; double L_elapsed_pos, L_elapsed_pos_rms;
double L_elapsed_pos_square_sum = 0;

String readString;
String input1_readString;
String input2_readString;


double R_pos, L_pos;
double R_vel, L_vel, R_temp_vel, L_temp_vel;

int R_value,L_value;
int vel_max = 460; int vel_min = 400;
int vel_max_neg = 460; int vel_min_neg =400;
int value_max = 255; int value_min = 150;

int input_1 = 0; int input_2 = 0;
double current_Time, elapsed_Time, previous_Time;

double R_current_err = 0; double R_pre_err = 0; double R_sum_err =0; double R_delta_err = 0;
double R_diff_err = 0;

double L_current_err = 0; double L_pre_err = 0; double L_sum_err =0; double L_delta_err = 0;
double L_diff_err = 0;


int R_u=0; int R_u_sat=0; int R_temp_u = 0;
int R_u_max = 255; int R_u_min = 150;
int L_u=0; int L_u_sat=0; int L_temp_u = 0;
int L_u_max = 255; int L_u_min = 150;
double Kp = 8;
double Ki = 0.06;
double Kd = 0.2;
double Kaw = 0.2;


//----------코드---------//


 void setup() {
  
  pinMode(Ena,OUTPUT); pinMode(IN1,OUTPUT); pinMode(IN2,OUTPUT);
  pinMode(Enb,OUTPUT); pinMode(IN3,OUTPUT); pinMode(IN4,OUTPUT);
  pinMode(R_CW,INPUT); pinMode(R_CCW,INPUT);
  pinMode(L_CW,INPUT); pinMode(L_CCW,INPUT);
  
   attachInterrupt(digitalPinToInterrupt(R_CW), check_R_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(R_CCW), check_R_B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(L_CW), check_L_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(L_CCW), check_L_B, CHANGE);
  
  MsTimer2::set(50,timerISR);
  MsTimer2::start();
  Serial.begin(9600);
}


 void check_R_A() {     //A핀의 변화가 있을때 checking//
  R_cw = digitalRead(R_CW);
  R_ccw = digitalRead(R_CCW);
  if(R_cw != R_ccw) R_ang_cw++;
  if(R_cw == R_ccw) R_ang_ccw++;
}
 void check_R_B() {    //B핀의 변화가 있을떄 checking//
  R_cw=digitalRead(R_CW);
  R_ccw=digitalRead(R_CCW);
  if(R_cw != R_ccw) R_ang_ccw++;
  if(R_cw == R_ccw) R_ang_cw++;
}
void check_L_A(){
  L_cw = digitalRead(L_CW);
  L_ccw = digitalRead(L_CCW);
  if(L_cw != L_ccw) L_ang_cw++;
  if(L_cw == L_ccw) L_ang_ccw++; 
}

void check_L_B(){
  L_cw = digitalRead(L_CW);
  L_ccw = digitalRead(L_CCW);
  if(L_cw == L_ccw) L_ang_cw++;
  if(L_cw != L_ccw) L_ang_ccw++; 
}




double compute_R_vel(){
  R_current_pos = R_pos;
  R_elapsed_pos = R_current_pos-R_pre_pos;
  R_pre_pos = R_pos;   
  if(abs(R_elapsed_pos)<10000){ 
    R_elapsed_pos_square_sum += R_elapsed_pos*R_elapsed_pos;
  }

   if(input_2>0){
  R_elapsed_pos_rms = sqrt(R_elapsed_pos_square_sum/n);
  }
  else if (input_2 ==0){
  R_elapsed_pos_rms = 0;
  }
  else{
  R_elapsed_pos_rms = -sqrt(R_elapsed_pos_square_sum/n);
  }
  
  if(Serial.available()>0){ //Serial 플로터에 입력되는 값이 있으면 elapsed_pos_square_sum을 초기화 // elapsed_pos가 제곱되어 누적되기 때문에 값이 입력되어도 입력값으로 바로 가지 않는다.
    n=1;
    R_elapsed_pos_square_sum = 0;
  }

  return R_temp_vel = 125*R_elapsed_pos_rms/50; // rpm으로 변환 60*1000*edge/t*ppr*4

}


double compute_L_vel(){
  L_current_pos = L_pos;
  L_elapsed_pos = L_current_pos-L_pre_pos;
  L_pre_pos = L_pos;   
  if(abs(L_elapsed_pos)<10000){ 
    L_elapsed_pos_square_sum += L_elapsed_pos*L_elapsed_pos;
  }

   if(input_1>0){
  L_elapsed_pos_rms = sqrt(L_elapsed_pos_square_sum/n);
  }
  else if (input_1 ==0){
  L_elapsed_pos_rms = 0;
  }
  else{
  L_elapsed_pos_rms = -sqrt(L_elapsed_pos_square_sum/n);
  }

  if(Serial.available()>0){ //Serial 플로터에 입력되는 값이 있으면 elapsed_pos_square_sum을 초기화 // elapsed_pos가 제곱되어 누적되기 때문에 값이 입력되어도 입력값으로 바로 가지 않는다.
    n=1;
    L_elapsed_pos_square_sum = 0;
  }

   return L_temp_vel = 125*L_elapsed_pos_rms/50; // rpm으로 변환 60*1000*edge/t*ppr*4
}

double compute_R_PID() {
  if(R_u>0) {
  if(R_u >= R_u_max) R_temp_u =R_u_max;
  else if( R_u < R_u_max && R_u_min < R_u) R_temp_u = R_u; 
  else R_temp_u=R_u_min;
  }
  else {
  if(R_u <= -R_u_max) R_temp_u = -R_u_max;
  else if( R_u > -R_u_max && -R_u_min> R_u) R_temp_u = R_u;
  else R_temp_u= -R_u_min;
  }
  current_Time = millis();
  elapsed_Time = current_Time - previous_Time;
  previous_Time = current_Time;
  
  R_current_err = input_2 - R_vel;
  R_delta_err = R_current_err - R_pre_err;
  R_pre_err = R_current_err;
  R_sum_err += (R_current_err-Kaw*(R_u-R_u_sat))*elapsed_Time;
  R_diff_err = R_delta_err / abs(elapsed_Time);  
  
  R_u= (int)(Kp*R_current_err + Ki*R_sum_err + Kd*R_diff_err);
  
  return R_temp_u;
}


double compute_L_PID(){
  
  if(L_u>0) {
    if(L_u >= L_u_max) L_temp_u =L_u_max;
    else if( L_u < L_u_max && L_u_min < L_u) L_temp_u = L_u; 
    else L_temp_u =L_u_min;
  }
    else {
  if(L_u <= -L_u_max) L_temp_u = -L_u_max;
  else if( L_u > -L_u_max && -L_u_min> L_u) L_temp_u = L_u;
  else L_temp_u= -L_u_min;
  }
  current_Time = millis();
  elapsed_Time = current_Time - previous_Time;
  previous_Time = current_Time;
  
  L_current_err = input_1 - L_vel;
  L_delta_err = L_current_err - L_pre_err;
  L_pre_err = L_current_err;
  L_sum_err += (L_current_err-Kaw*(L_u-L_u_sat))*elapsed_Time;
  L_diff_err = L_delta_err / abs(elapsed_Time);  
  
  L_u= (int)(Kp*L_current_err + Ki*L_sum_err + Kd*L_diff_err);
  return L_temp_u;
}


 void timerISR() {
  n++; // 타이머 인터럽트 횟수
  
  //------속도 계산------//
  R_vel = compute_R_vel();
  L_vel = compute_L_vel();

  //---------PID계산----------//
  R_u_sat = compute_R_PID();
  L_u_sat = compute_L_PID();


  //------ 속도 값 입력-------// 
 while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString
    delay(2);  // allow buffer to fill with next character
 }
  if (readString.length() >0) {
    number = 0;
    for(int i = 0; readString[i] != ','; i++){
      input1_readString += readString[i];
      number++;
    }
    input_1 = input1_readString.toInt();
    for(int i = number+1; i<readString.length(); i++){
      input2_readString += readString[i];
    }
    input_2 = input2_readString.toInt();
    readString=""; //empty for next input
    input1_readString="";
    input2_readString="";
    number = 0;
  }
}


 void loop() {
  R_pos = R_ang_cw - R_ang_ccw;
  L_pos = L_ang_cw - L_ang_ccw; //왼쪽 바퀴는 반대로 달려있어서 정방향으로 회전하면 뒤로 가기 떄문에 오른쪽과는 부호를 반대로 해준다.


  if(input_2>0){
   digitalWrite(IN3,HIGH); digitalWrite(IN4, LOW);
   analogWrite(Enb, R_u_sat);
  }
  else if(input_2 ==0){
    analogWrite(Enb,0);
  }
  else{
    digitalWrite(IN3,LOW); digitalWrite(IN4,HIGH);
   analogWrite(Enb,-R_u_sat); 
  }


  if(input_1>0){
   digitalWrite(IN1,LOW); digitalWrite(IN2, HIGH);
   analogWrite(Ena, -L_u_sat);
  }
  else if(input_1 ==0){
    analogWrite(Ena,0);
  }
  else{
    digitalWrite(IN1,HIGH); digitalWrite(IN2,LOW);
   analogWrite(Ena, L_u_sat); 
  }

  //Serial.println(input_1);
  //Serial.print(',');
  Serial.println(L_vel);
  Serial.print(',');
  //Serial.println(input_2);
  //Serial.print(',');
  Serial.println(R_vel);
  //Serial.print(',');
  //Serial.println(vel);
 //Serial.println(elapsed_pos);
 //Serial.print(',');
 //Serial.println(elapsed_pos_rms);
  //Serial.println(number);
}
