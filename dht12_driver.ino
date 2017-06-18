#define u8 unsigned short
#define u16 unsigned int
#define u32 unsigned long
const int DHT12Pin = 5;

double lastTemp = 0;
double lastHumi = 0;
int DHT12_flag = 0;
int DHT12_error = 0;
void DHT12_Init()
{
  pinMode(DHT12Pin, OUTPUT);
  digitalWrite(DHT12Pin, LOW);
  delay(50);
}

/*=======================
**Name:
**Brief:
========================*/
u8 DHT12_Rdata(void)
{
  u8 i;
  u16 j;
  u8 data = 0;
  u8 dbit = 0;
  pinMode(DHT12Pin, INPUT);
  for (i = 0; i < 8; i++)
  {
    while (digitalRead(DHT12Pin) == LOW) //check the last low state is over
    {
      if (++j >= 50000)
      {

      }
    }
    delayMicroseconds(50);//delay 26-70us,to overed data0 high level

    dbit = 0;
    if (digitalRead(DHT12Pin) == HIGH)
    {
      dbit = 1;
    }
    while (digitalRead(DHT12Pin) == HIGH) //等待高电平结束
    {
      if (++j >= 50000) //防止进入死循环
      {
        break;
      }
    }
    data <<= 1;
    data |= dbit;

  }
  return data;
}

/*=======================
**Name:
**Brief:
========================*/
u8 DHT12_ReadTemp()
{
  u32 j;
  double Humi=0,Temprature =0;
  u8 Humi_H, Humi_L, Temp_H, Temp_L, Temp_CAL, temp;
  pinMode(DHT12Pin, OUTPUT);
  digitalWrite(DHT12Pin, LOW);
  delay(20);
  digitalWrite(DHT12Pin, HIGH);
  pinMode(DHT12Pin, INPUT);
  delayMicroseconds(30);
  DHT12_flag = 0;
  if (digitalRead(DHT12Pin) == LOW)
  {
    DHT12_flag = 1;

    j = 0;
    while (digitalRead(DHT12Pin) == LOW) //判断从机发出 80us 的低电平响应信号是否结束
  {
    if (++j >= 500) //防止进入死循环
      {
        DHT12_error = 1;
        break;
      }
    }
    j = 0;
        while (digitalRead(DHT12Pin) == HIGH) //判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
  {
    if (++j >= 800) //防止进入死循环
      {
        DHT12_error = 1;
        break;
      }
    }
    Humi_H = DHT12_Rdata();
             Humi_L = DHT12_Rdata();
             Temp_H = DHT12_Rdata();
             Temp_L = DHT12_Rdata();
             Temp_CAL = DHT12_Rdata();
             temp = (u8)(Humi_H + Humi_L + Temp_H + Temp_L);

             //Verify
             //if success ,continue
             if (Temp_CAL == temp) //如果校验成功，往下运行
  {
    Humi = Humi_H * 10 + Humi_L; //湿度

    if (Temp_L & 0X80) //为负温度
      {
        Temprature = 0 - (Temp_H * 10 + ((Temp_L & 0x7F)));
      }
      else   //为正温度
      {
        Temprature = Temp_H * 10 + Temp_L; //为正温度
      }
      //判断数据是否超过量程（温度：-20℃~60℃，湿度20％RH~95％RH）
      if (Humi > 950)
      {
        Humi = 950;
      }
      if (Humi < 200)
      {
        Humi = 200;
      }
      if (Temprature > 600)
      {
        Temprature = 600;
      }
      if (Temprature < -200)
      {
        Temprature = -200;
      }
      Temprature = Temprature / 10; //计算为温度值
      Humi = Humi / 10; 
      lastTemp = Temprature;
      lastHumi = Humi;
      Serial.print("\r\n温度为: ");
      Serial.print(Temprature);
      Serial.print("  ℃\r\n"); 
      Serial.print("湿度为: ");
      Serial.print(Humi);
      Serial.print(" RH\r\n"); 
    }
    else
    {
      Serial.print("CAL Error!!\r\n");
      //Serial.print("%d \r%d \r%d \r%d \r%d \r%d \r\n", Humi_H, Humi_L, Temp_H, Temp_L, Temp_CAL, temp);
    }
  }
  else
  {
    DHT12_error = 0;
    Serial.print("Sensor Error!!\r\n");
  }

  return 1;
}

double getTemp()
{
  return lastTemp;
}
double getHumi()
{
  return lastHumi;
}

