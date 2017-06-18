
//Multi Measure
//Artist: Arenas
//Data: 2017.6.17
//Version : 1.0

/*
 * D2   
 * D3   报警蜂鸣器
 * D4   
 * D5   DHT12温度
 * ----------------
 * D6   旋转编码器CLK
 * D7   旋转编码器DT
 * D8   旋转编码器SW
 * D9   
 * D10    
 * D11    
 * D12    
 * -----------------
 * A0   
 * A1   
 * A2   
 * A3   
 * A4   LCD SDA
 * A5   LCD SCL
 * A6   电压量测点
 * A7   电流量测点
*/
#include <Wire.h>
#include <Thread.h>
#include <LiquidCrystal_I2C.h>
#define buzzer 3
Thread Thread_voltageData = Thread();
Thread Thread_dht = Thread();
Thread Thread_encoder = Thread();
Thread Thread_current = Thread();
Thread Thread_currentData = Thread();
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

//电压、电流、温度修正系数数据,倍率型

double  measVol = 4.13;//该设备的测试电压
double  factVol = 24.04;//实际电压
double  measCur = 2.05;//该设备的测试电流
double  factCur = 2.00;//实际电流
double  measTemp = 26.1;//测试温度
double  factTemp = 26.6;//实际温度

bool disonlcd = true;//if false ,all data not display(when enter menu)


void beep()
{
  int i;
  for(i=0;i<100;i++)//输出一个频率的声音
    {
      digitalWrite(buzzer,HIGH);//发声音
      delayMicroseconds(100);//延时1ms
      digitalWrite(buzzer,LOW);//不发声音
      delayMicroseconds(250);//延时ms
    }
}


//初始化LCD显示屏I2C
void LCD_init()
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(5, 0);
  lcd.printStr("Welcome");
  delay(800);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.printStr("0.00  V");
  lcd.setCursor(15, 0);
  lcd.print("A");
}

//计算一个数组的平均数值
double averageData(double data[], int n)
{
  double outdata = 0;

  for (int i = 0; i < n; i++)
  {
    outdata += data[i];
  }
  outdata /= n;
  return outdata;
}

#define ADCTIME  7  //滤波采样次数

double volData[ADCTIME];//读取的等待滤波的电压值的存储数组


int volCounter = 0;


//获取数据并且滤波,打印到LCD上,此处用于电压数值的滤波
void dataget7(int ch, double input[], int *counter)
{
  double outdata[ADCTIME];
  double printdata = 0;
  volData[volCounter] = analogRead(ch);
  volCounter ++;
  //go to filter
  if (volCounter >= ADCTIME)
  {
    cubicSmooth7(input, outdata, ADCTIME);
    //linearSmooth5(volData, outdata, ADCTIME);
    printdata = averageData(outdata, ADCTIME) ;

    Serial.print("Analog channel ");
    Serial.print(ch);
    Serial.print(":");
    Serial.println(printdata );
    printdata = ( printdata * 5 / 1024) * (factVol / measVol); //测量的电压值
    lcd.setCursor(0, 0);
    if(disonlcd)
    lcd.print(printdata);
    volCounter = 0;
  }
}

//获取数据并滤波
void volDataget()
{
  dataget7(7, volData, &volCounter);
}


//打印模拟值数据
void analogprint()
{
  LCD_currentToprint(6);
}




//读取并打印指定ADC通道的值
void LCD_currentToprint(int ch)
{

  float data = 0 ;
  float readdata = analogRead(ch);
  Serial.print("Analog channel ");
  Serial.print(ch);
  Serial.print(":");
  Serial.println(readdata);

  data = 5 * (readdata / 1024.0) * (factCur / measCur);

  lcd.setCursor(10, 0);
  if(disonlcd)
  lcd.print(data);
}

//显示温度值和湿度值
void LCD_DispTempAndHumi()
{
  char duh = 0x43;
  char dul = 0xDF;
  DHT12_ReadTemp();
  if(disonlcd)
  {
  lcd.setCursor(0, 1);
  lcd.print(getTemp() + (factTemp - measTemp));
  lcd.print(dul);
  lcd.print(duh);
  lcd.setCursor(9, 1);
  lcd.print(getHumi());
  lcd.printStr("RH");
  if((getTemp() + (factTemp - measTemp)>40 ))
  beep();
  }
}

//==================MAIN FUNCTION====================//
void setup()
{
  Serial.begin(115200);

  // Print a message to the LCD.
   pinMode(buzzer,OUTPUT);
  LCD_init();
  revEncoder_init();
  Thread_voltageData.onRun(volDataget);
  Thread_voltageData.setInterval(120);
  Thread_currentData.onRun(analogprint);
  Thread_currentData.setInterval(100);
  Thread_dht.onRun(LCD_DispTempAndHumi);
  Thread_dht.setInterval(1500);
  Thread_encoder.onRun(revEncoder_judge);
  Thread_encoder.setInterval(10);
}

void loop()
{

  if (Thread_voltageData.shouldRun()) //电压
    Thread_voltageData.run();
  if (Thread_currentData.shouldRun())//电流
    Thread_currentData.run();
   if (Thread_dht.shouldRun())//温度湿度
      Thread_dht.run();
    if (Thread_encoder.shouldRun())//编码器
      Thread_encoder.run();

  // volDataget();
}
