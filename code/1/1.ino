#include <Servo.h>//c:\Users\ACTO-Industrial\Documents\电子实习\电子实习\电子实习\小车循迹代码\2\2.ino
// ==========================================
// 1. 引脚定义与全局变量
// ==========================================
int Motor1_IN1 = 8, Motor1_IN2 = A0, EN1 = 9;
int Motor2_IN1 = A1, Motor2_IN2 = 12, EN2 = 10;
const int SensorLeft = 4, SensorRight = 2;
const int ObsLeft = A3, ObsRight = A2;

Servo base, rArm, fArm, claw;
int curBase = 90, curRArm = 90, curFArm = 90, curClaw = 90;
bool isAtPlatform = false;

// ==========================================
// 2. 底层驱动
// ==========================================
void Motor1_Brake() { digitalWrite(Motor1_IN1,LOW); digitalWrite(Motor1_IN2,LOW); }
void Motor1_Run(int s) { digitalWrite(Motor1_IN1,HIGH); digitalWrite(Motor1_IN2,LOW); analogWrite(EN1,s); }
void Motor1_Back(int s) { digitalWrite(Motor1_IN1,LOW); digitalWrite(Motor1_IN2,HIGH); analogWrite(EN1,s); }
void Motor2_Brake() { digitalWrite(Motor2_IN1,LOW); digitalWrite(Motor2_IN2,LOW); }
void Motor2_Run(int s) { digitalWrite(Motor2_IN1,LOW); digitalWrite(Motor2_IN2,HIGH); analogWrite(EN2,s); }
void Motor2_Back(int s) { digitalWrite(Motor2_IN1,HIGH); digitalWrite(Motor2_IN2,LOW); analogWrite(EN2,s); }
void Car_Forward(int L, int R) { Motor1_Run(L); Motor2_Run(R); }
void Car_Stop() { Motor1_Brake(); Motor2_Brake(); }
void Car_TurnLeft(int s) { Motor1_Brake(); Motor2_Run(s); }
void Car_TurnRight(int s) { Motor1_Run(s); Motor2_Brake(); }

void smoothMoveAll(int tBase, int tRArm, int tFArm, int tClaw, int stepDelay=15) {
  while (curBase!=tBase || curRArm!=tRArm || curFArm!=tFArm || curClaw!=tClaw) {
    if (curBase<tBase) base.write(++curBase); else if (curBase>tBase) base.write(--curBase);
    if (curRArm<tRArm) rArm.write(++curRArm); else if (curRArm>tRArm) rArm.write(--curRArm);
    if (curFArm<tFArm) fArm.write(++curFArm); else if (curFArm>tFArm) fArm.write(--curFArm);
    if (curClaw<tClaw) claw.write(++curClaw); else if (curClaw>tClaw) claw.write(--curClaw);
    delay(stepDelay);
  }
}

// ==========================================
// 3. 初始化
// ==========================================
void setup() {
  pinMode(Motor1_IN1,OUTPUT); pinMode(Motor1_IN2,OUTPUT); pinMode(EN1,OUTPUT);
  pinMode(Motor2_IN1,OUTPUT); pinMode(Motor2_IN2,OUTPUT); pinMode(EN2,OUTPUT);
  pinMode(SensorLeft,INPUT); pinMode(SensorRight,INPUT);
  pinMode(ObsLeft,INPUT_PULLUP); pinMode(ObsRight,INPUT_PULLUP);
  Car_Stop();
  delay(1000);
}

// ==========================================
// 4. 主循环
// ==========================================
void loop() {
  if (!isAtPlatform) {
    int speed=90;
    if (digitalRead(ObsLeft)==LOW || digitalRead(ObsRight)==LOW) {
      Car_Stop(); isAtPlatform=true;
      base.attach(11); rArm.attach(5); fArm.attach(3); claw.attach(6);
      base.write(90); rArm.write(90); fArm.write(90); claw.write(90);
      delay(1500); return;
    }
    int SL=digitalRead(SensorLeft), SR=digitalRead(SensorRight);
    if (SL==LOW && SR==LOW) Car_Forward(speed,speed);
    else if (SL==HIGH && SR==LOW) Car_TurnLeft(speed);
    else if (SR==HIGH && SL==LOW) Car_TurnRight(speed);
    else Car_Stop();
  } else {
    // ========== 抽签1：B1(球体72→147推) → B2(正方体115→165) → B3(圆柱体93→180) ==========

    // --- B1 球体 (任务2模板：推版) ---
    smoothMoveAll(72, 90, 90, curClaw); delay(200);
    smoothMoveAll(curBase, curRArm, curFArm, 32); delay(200);
    smoothMoveAll(curBase, 180, 20, curClaw); delay(200);
    smoothMoveAll(curBase, curRArm, curFArm, 6.8); delay(200);   // 球体闭合
    smoothMoveAll(curBase, 130, 50, curClaw);
    smoothMoveAll(90, curRArm, curFArm, curClaw);
    smoothMoveAll(120, curRArm, curFArm, curClaw);
    smoothMoveAll(147, curRArm, curFArm, curClaw); delay(200);    // 放置147°
    smoothMoveAll(curBase, 180, 20, curClaw); delay(200);
    smoothMoveAll(curBase, curRArm, curFArm, 25); delay(200);
    // 推的动作
    smoothMoveAll(curBase, curRArm, curFArm, 25);
    smoothMoveAll(curBase, 90, 90, curClaw);
    smoothMoveAll(curBase, 90, 180, 5);
    smoothMoveAll(curBase, 90, 90, curClaw);
    smoothMoveAll(curBase, 140, 100, curClaw);
    smoothMoveAll(curBase, 160, 20, curClaw);
    smoothMoveAll(curBase, 90, 90, 5);
    smoothMoveAll(curBase, 90, 90, curClaw);
    delay(200);
    smoothMoveAll(90, 90, 90, 90);
    delay(500);

    // --- B2 正方体 (任务1模板：普通版，过渡120→145→165) ---
    smoothMoveAll(115, 90, 90, curClaw); delay(200);
    smoothMoveAll(curBase, curRArm, curFArm, 32); delay(200);
    smoothMoveAll(curBase, 180, 20, curClaw); delay(200);
    smoothMoveAll(curBase, curRArm, curFArm, 6.8); delay(200);   // 正方体闭合
    smoothMoveAll(curBase, 130, 50, curClaw);
    smoothMoveAll(120, curRArm, curFArm, curClaw);
    smoothMoveAll(145, curRArm, curFArm, curClaw);
    smoothMoveAll(165, curRArm, curFArm, curClaw); delay(200);   // 放置165°
    smoothMoveAll(curBase, 180, 20, curClaw); delay(200);
    smoothMoveAll(curBase, curRArm, curFArm, 35); delay(200);
    smoothMoveAll(curBase, curRArm, curFArm, 45);
    smoothMoveAll(curBase, 90, 90, curClaw);
    smoothMoveAll(90, 90, 90, 90);
    delay(500);

    // --- B3 圆柱体 (任务3模板：普通版，过渡145→165→180) ---
    smoothMoveAll(93, 90, 90, curClaw); delay(200);
    smoothMoveAll(curBase, curRArm, curFArm, 32); delay(200);
    smoothMoveAll(curBase, 180, 20, curClaw); delay(200);
    smoothMoveAll(curBase, curRArm, curFArm, 7.5); delay(200);   // 圆柱体闭合
    smoothMoveAll(curBase, 130, 50, curClaw);
    smoothMoveAll(145, curRArm, curFArm, curClaw);
    smoothMoveAll(165, curRArm, curFArm, curClaw);
    smoothMoveAll(180, curRArm, curFArm, curClaw); delay(200);   // 放置180°
    smoothMoveAll(curBase, 180, 20, curClaw); delay(200);
    smoothMoveAll(curBase, curRArm, curFArm, 25); delay(200);
    smoothMoveAll(curBase, curRArm, curFArm, 25);
    smoothMoveAll(curBase, 90, 90, curClaw);
    smoothMoveAll(90, 90, 90, 90);
    delay(500);

    while(1);
  }
}