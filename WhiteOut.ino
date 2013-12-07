#define SMOOTHINGAMOUNT 5

int inputArray[3][3];
int lightPinArray[3][3];
boolean lightStateArray[3][3];
int analogSmoothingArray[6];
int smoothingCounter = 0;

//test comment
void setup()
{
  Serial.begin(9600);
  Serial.println("I'm up!");
  initArrays();
  startupSequence();
  initButtons();
  initSwitches();
}

void startupSequence()
{
  for (int row = 0; row<3;row++)
  {
    for(int column = 0;column<3;column++)
    {
      if(lightPinArray[row][column] != -1)
      {
        flipLight(row, column);
        delay(200);
        flipLight(row,column);
      }
    }
  }
  for (int row = 2; row>=0;row--)
  {
    for(int column = 2;column>=0;column--)
    {
      if(lightPinArray[row][column] != -1)
      {
        flipLight(row, column);
        delay(200);
        flipLight(row,column);
      }
    }
  }
  delay(500);
  initLightState();  
}
void initOutputs()
{
  for (int row = 0; row<3;row++)
  {
    for(int column = 0;column<3;column++)
    {
      if(lightPinArray[row][column] != -1)
      {
        pinMode(lightPinArray[row][column],OUTPUT);
      }
    }
  }
  Serial.println("Outputs have been initialized");
}

void initButtons()
{
  for(int row=0;row<3;row++)
  {
    for(int column=0;column<3;column++)
    {
      if(lightPinArray[row][column] !=-1)
      {
        digitalWrite(lightPinArray[row][column], (lightStateArray[row][column])?HIGH:LOW);
      }
    }
  }
  Serial.println("Buttons have been initialized");  
}
void initSwitches()
{
  
  pinMode(13, INPUT);
  pinMode(8, INPUT);
  pinMode(11, INPUT);
  Serial.println("Digital inputs have been initialized");
}
void initArrays()
{
  for (int i = 0; i<3;i++)
  {
    for(int j = 0; j<3;j++)
    {
      //putting in sentinal values
      inputArray[i][j] = 0;
      lightPinArray[i][j]=-1; 
      lightStateArray[i][j] = false;
    }
  }
  for(int i= 0; i<6; i++)
  {
    analogSmoothingArray[i]=0;
  }

  //hard code the array information
  inputArray[0][0] = A3; //A3
  inputArray[0][1] = A4; //A4
  inputArray[0][2] = A5; //A5
  inputArray[1][0] = A0; //A0
  inputArray[1][1] = A1; //A1
  inputArray[1][2] = A2; //A2
  inputArray[2][0] = 13; //13
  inputArray[2][1] = 11;//11
  inputArray[2][2] = 8; //8

  lightPinArray[0][0] = 2; //2
  lightPinArray[0][1] = 3; //3
  lightPinArray[0][2] = 4; //4
  lightPinArray[1][0] = 7; //7
  lightPinArray[1][1] = 6; //6
  lightPinArray[1][2] = 5; //5
  lightPinArray[2][0] = 12; //12
  lightPinArray[2][1] = 10; //10
  lightPinArray[2][2] = 9; //9

  Serial.println("Arrays have been initialized");
  initOutputs();
  
}

void initLightState()
{
    //this is where the randomization logic will live.  For now, it should just turn things on.
    randomSeed(analogRead(A0));
    for(int row=0;row<3;row++)
    {
      for(int column=0;column<3;column++)
      {
        Serial.print("Random value: ");
        Serial.println(random(100)%2);
//        lightStateArray[row][column] = true; //testing
        lightStateArray[row][column] = random(100)%2==1?true:false;
      }
    }
//    lightStateArray[2][0] = true;
//    lightStateArray[2][1] = true;
//    lightStateArray[2][2] = true;
}
void checkButtons(){
  //note: I should put some reset/diag logic in here
  for(int i=0;i<3;i++)
  {
    for(int j=0;j<3;j++)
    {
      if(inputArray[i][j] != 0)
      { 
     /*   Serial.print(i);
        Serial.print(":row ");
        Serial.print(j);
        Serial.print(": column ");
        Serial.print(analogRead(inputArray[i][j]));
        Serial.println(": analogRead");*/
        if(checkButton(i,j))
        {
          Serial.print("Button Flipped row: ");
          Serial.print(i);
          Serial.print(" column: ");
          Serial.println(j);
          flipLight(i,j); //button you hit
          flipLight(i-1,j); //button above
          flipLight(i+1,j); //button below
          flipLight(i,j-1); //button left
          flipLight(i,j+1); //button right
          if(checkWin())
          {
            delay(500);
            startupSequence();
            initButtons();
          } else {
            delay(500);
          }
        }
      }
    }
  }
}

boolean checkWin()
{
  for(int row = 0; row < 3; row++)
  {
    for (int column = 0; column < 3; column++)
    {
      if(!lightStateArray[row][column])
        return false;
    }
  }
  return true;
}

boolean checkButton(int row, int column)
{
  
  //has a button been pressed?
  if(inputArray[row][column] > 13) //is it an analog input?
  {
  /*  Serial.print("AnalogInput row: ");
    Serial.print(row);
    Serial.print(" column: ");
    Serial.print(column);
    Serial.print(" value: ");
    Serial.println(analogRead(inputArray[row][column]));
    */
    analogSmoothingArray[((row*2)+row)+column]+=analogRead(inputArray[row][column]);
    smoothingCounter++;
    if(smoothingCounter == SMOOTHINGAMOUNT)
    {
      int average = analogSmoothingArray[((row*2)+row)+column] / SMOOTHINGAMOUNT;
      analogSmoothingArray[((row*2)+row)+column] = 0;
      smoothingCounter = 0;
      return (average==1023);
    }
    return false;
    //return (digitalRead(inputArray[row][column])==HIGH);
  } 
  else
  {
    //return false;
    return (digitalRead(inputArray[row][column])==1);
  }
  Serial.println(digitalRead(4));
}

void flipLight(int row, int column)
{
  if(row<0 || row > 2 || column<0 || column>2)
    return;
    
  //this is where the fancy logic happens for the buttonny stuff
  lightStateArray[row][column] = !lightStateArray[row][column];
  Serial.print("row:");
  Serial.print(row);
  Serial.print(" column: ");
  Serial.print(column);
  Serial.print(" newState:");
  Serial.println(lightStateArray[row][column]);
  digitalWrite(lightPinArray[row][column], (lightStateArray[row][column])?HIGH:LOW);
  //modify the surrounding buttons
}

void loop(){
//  Serial.println(analogRead(A7));
  checkButtons();
  delay(10);
}
