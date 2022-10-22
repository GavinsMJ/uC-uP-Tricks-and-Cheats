String str = "13,45,21,40,23,108,233,432,109,567";
int values[10];


String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}


void setup () {

  Serial.begin (115200);

}
    
void loop () { 


for (int i = 0 ; i< 10; i++){
    String data = getValue(str,',',i);  
    values[i] = data.toInt();
  }

for (int i = 0 ; i< 10; i++){
    Serial.print(" Value " + (String)(i+1) + ": "); 
    Serial.print(values[i]);
  }

  Serial.println("");
  delay(2000);
  
}
