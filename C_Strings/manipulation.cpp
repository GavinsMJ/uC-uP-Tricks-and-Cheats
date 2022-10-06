// Problems with strings on the Arduino:
// 1) Mysterious crashes after the program has run for many days. Possibly due to memory fragmentation
// 2) Precious ram used to do very simple things like print "hello world". 
// 3) If output to the debug screen with Serial.println and then later port a program to a different display, need to change lots of code
// To solve these problems:
// 1) Remove all references to the String class. 
// 2) Move all fixed strings (eg debug messages, error messages) from ram to flash. 
// 3) Third, move all serial output to just one line in one function, so this can be changed for different displays.
// and 
// 4), learn do code using old fashioned C string manipulation, which is actually quite fun, and can do most anything the String class can do
// First step - compile an empty program to an arduino, and take a look at the message at the bottom of the screen 'Global variables use xx bytes'.
// then aim is to keep this number low, and for any temporary string space to only use the stack, so it is recovered fully when a function ends
// Experiments inspired by https://hackingmajenkoblog.wordpress.com/2016/02/04/the-evils-of-arduino-strings/
// By way of example, this is the syntax for printing Hello World which ends up being two lines
// 
//  const static char s1[] PROGMEM = "Hello World"; // store the text in flash
//  printStringln(getString(s1)); // move to temporary string and then print
// 
// and then the prototype code for most of the functions, move a string from inputString to outputString
//
//  const static char s1[] PROGMEM = "Parse Test"; // move fixed text into temporary array
//  strcpy_P(inputString1, s1); // copy to one of the temp strings, must use strcpy_P not strcpy
//  char* outputString = parseString(inputString1); // move to outputString, note the char* before outputString. parseString function is strcpy()
//  printStringln(outputString); // print out
//
// So now can avoid using the String class completely - uses far less ram, no mysterious bugs due to memory fragmentation
// also minimise use of Serial. class, only use it in one place to output one character. Makes it easy to port code to different displays.
// looking at https://www.arduino.cc/en/Reference/PROGMEM  then const char messageOne[] PROGMEM = "Here is a big hello string" 
// must be at the start, however, adding the 'static' means it can be in a function, and still not use any ram
// putting it in a function is more convenient as can see what the message is rather than having all the messages in one place.
// see the progmem link above, not using this, but could be useful with setting up string arrays for lots of messages, then store in progmem
// removed declaring constant strings at the start of the program, easier to have the actual message local in a function so can read what it is
// start with a completely blank program - uses 9 bytes (or 2048 on a Uno). Add Serial.begin and it uses 182 bytes
// add  Serial.println(""); and it uses 200 bytes. Change to   Serial.println("0123456789"); and this uses 210 bytes, and an extra byte for every debug message like this
// change to   Serial.println(F("0123456789"));  and it is back to 200 bytes, however can't use F when creating strings that are not printed straight away
// and also can't use F for other custom print routines, eg printing to an LCD screen
// add in inputString arrays and outputstring array, and is now 422 bytes, aim to stay with this number while adding multiple further test routines
// even with just these three universal strings, the ram used is 20% of a uno. Howwever, on a uno there is 32k of flash so plenty of room for storing strings.
// https://stackoverflow.com/questions/43917979/return-the-address-of-a-global-string-variable so can return outputString
// https://en.wikibooks.org/wiki/C_Programming/Strings  for more strings
// Microsoft Basic by Ken Knecht 1983, back when basic was much simpler. Copied string examples from this as can do a lot with just a few functions.
// added multiple examples in both basic syntax and c syntax. 
// more things to think about, reading in a text file from an SD card, processing each text line one at a time. Or decoding a page of HTML
#include <Arduino.h>
#include <stdio.h>
#include <string.h>

char inputString1[80]; // general purpose input string, 80 bytes
char inputString2[80]; // general purpose input string, 80 bytes
char outputString[80]; // general output string, 80 bytes

void setup() {
    Serial.begin(9600);    // start serial port
    messageTestExample();  // print Hello World
    parseTest();           // create a string in flash, move to inputstring, move to outputstring, print on the screen
    printNumber(19);       // print a number on the screen
    stringCompareTest();   // create two strings, compare if equal. Also shows how debug messages are in flash, saving precious ram
    integerToStringTest(); // integer to a string
    stringToIntegerTest(); // string to an ingeter
    longToStringTest();    // long to a string 1234
    longToBinaryTest();    // long to a binary string 110101
    longToHexTest();       // long to a hex string 123ABC
    // old Basic functions
    leftStringTest();      // get the left characters from a string
    midStringTest();       // get the middle characters from a string
    instrTest();           // find where a string is in another string
    lenTest();             // length of a string
    ascTest();             // character to a number
    chrTest();             // number to a character
    hexTest();             // number to a hex string
    basicStrTest();        // number to a string
    basicValTest();        // string to a number
    basicStringAddTest();  // add two strings together
    finishMessage();       // for debugging, string errors tend to corrupt code so it never gets to here, so make sure this prints
}

void loop() 
{

}

//******************* Test examples *********************
void messageTestExample()
{
  const static char s1[] PROGMEM = "Hello World"; // store the text in flash
  printStringln(getString(s1));
}

void parseTest() 
{
  const static char s1[] PROGMEM = "Parse Test"; // move fixed text into temporary array
  strcpy_P(inputString1, s1); // copy to one of the temp strings, must use strcpy_P not strcpy
  char* outputString = parseString(inputString1); // move to outputString, note the char* before outputString
  printStringln(outputString); // print out
}


void stringCompareTest() // compare two strings
{
  uint8_t a; // a byte
  const static char s1[] PROGMEM = "Long string of text"; // move fixed text into temporary array
  strcpy_P(inputString1, s1); // copy to one of the temp strings, must use strcpy_P not strcpy
  const static char s2[] PROGMEM = "Long string of text"; // move fixed text into temporary array
  strcpy_P(inputString2, s2); // copy to the other input string
  a = stringCompare(inputString1,inputString2); // 0 is a match
  if (a == 0) {
    const static char s3[] PROGMEM = "String compare - Strings match"; // store the text in flash
    printStringln(getString(s3));
  }else{
    const static char s4[] PROGMEM = "String compare - Strings do not match"; // store the text in flash
    printStringln(getString(s4)); 
  }
}

void leftStringTest()
{
  const static char s1[] PROGMEM = "Test left string routine"; // move fixed text into temporary array
  strcpy_P(inputString1, s1); // copy to inputString1
  char* outputString = basicLeftString(inputString1, 14); // left most characters, 1 returns one character, 0 not allowed
  printStringln(outputString); 
}

void midStringTest()
{
  const static char s1[] PROGMEM = "Test mid string routine"; // move fixed text into temporary array
  strcpy_P(inputString1, s1); // copy to inputString1
  char* outputString = basicMidString(inputString1, 6, 10); // mid characters, first is 1, start at 6, and return 10 characters
  printStringln(outputString); 
}

void stringToIntegerTest() 
{
  int i;
  const static char s1[] PROGMEM = "500"; 
  strcpy_P(inputString1, s1); 
  i = stringToInteger(inputString1); // value is in i
  i = i + 100; // do something to this number
  printNumber(i);
}

void longToStringTest()
{
    char* outputString = longToString(-12345678); // long to string
    printStringln(outputString);
}

void longToBinaryTest()
{
    char* outputString = longToBinaryString(65535);
    printStringln(outputString);
}

void longToHexTest()
{
    char* outputString = longToHexString(65535);
    printStringln(outputString);
}

void integerToStringTest()
{
    char* outputString = integerToString(1234); // convert integer to a string
    printStringln(outputString);
}  

void instrTest() // uses strstr, see also strchr to find a character in a string
{
  int index;
  const static char s1[] PROGMEM = "Find a needle in a haystack"; 
  strcpy_P(inputString1, s1); 
  const static char s2[] PROGMEM = "needle"; 
  strcpy_P(inputString2, s2); 
  index = basicInstr(inputString1,inputString2); // find where needle is
  printNumber(index);
}

void lenTest() 
{
  int i;
  const static char s1[] PROGMEM = "Hello World"; 
  strcpy_P(inputString1, s1); 
  i = basicLen(inputString1);
  printNumber(i);
}

void ascTest()
{
  uint8_t n;
  const static char s1[] PROGMEM = "A Hello World"; // should print 65
  strcpy_P(inputString1, s1); 
  n = basicAsc(inputString1);
  printNumber(n);
}

void chrTest()
{
  char* outputString = basicChr(66); // ascii B
  printStringln(outputString);
}

void hexTest()
{
  char* outputString = basicHex(254);
  printStringln(outputString);
}

void basicStrTest() // number to string
{
    char* outputString = basicStr(120); // same as long to string
    printStringln(outputString);
}

void basicStringAddTest()
{
  const static char s1[] PROGMEM = "String "; // move fixed text into temporary array
  strcpy_P(inputString1, s1); // copy to one of the temp strings, must use strcpy_P not strcpy
  const static char s2[] PROGMEM = "Add"; // move fixed text into temporary array
  strcpy_P(inputString2, s2); // copy to the other input string
  char* outputString = basicStringAdd(inputString1,inputString2);
  printStringln(outputString);
}

void finishMessage() 
{
 const static char s1[] PROGMEM = "Finished"; // print a finish message, bugs tend to corrupt the final message so end with this
 printString(getString(s1));
}

//******************** String Routines ******************

char* getString(const char* str) // String replacement - move string from flash to local buffer
{
  strcpy_P(outputString, (char*)str);
  return outputString;
}

void printString(const char *str) // all output directed through this one routine, so can change Serial.print to whatever display is being used
{ 
    const char *p;
    p = str;
    while (*p) {
        Serial.print(*p); // explanation in majenko's webpage
        p++;
    }
}

void printStringln(const char *str) // print line with crlf
{
    printString(str); 
    crlf(); // carriage return, line feed
}

void crlf() // carriage return and linefeed
{
  const static char crlf[] PROGMEM = "\r\n"; // carriage return, then line feed, maybe not the most efficient way to do this but works
  printString(getString(crlf)); // print out, using one central function for output so easier to change destination with different displays
}

void printNumber(long n)
{
  char* outputString = integerToString(n); // convert to number
  printStringln(outputString);
}

int stringCompare(const char *str1, const char *str2) 
{ 
  uint8_t a;
  a = strcmp(str1, str2); // 0 is a match
  return a;
}

char* basicLeftString(const char *str, int i) 
{
    strncpy(outputString, (char*)str, i); // copy to outputString
    outputString[i] = '\0'; // put in new null terminator
    return outputString;
}

char* basicMidString(const char *str, int stringStart, int stringLength) 
{
    strncpy(outputString, (char*)str + stringStart - 1, stringLength); // copy to outputString using Basic nomenclature where 1 is the first character
    outputString[stringLength] = '\0'; // put in new null terminator
    return outputString;
}

char* integerToString(int n) // returns outputString
{
  itoa(n, outputString, 10); // itoa is for integers, 10 is for base 10 (could use 2 for binary, 16 for hex)
  return outputString;
}

char* longToString(long n)
{
  ltoa(n, outputString, 10); // base 10
  return outputString;
}

char* longToBinaryString(long n)
{
  ltoa(n, outputString, 2); // base 2 is binary
  return outputString;
}

char* longToHexString(long n)
{
  ltoa(n, outputString, 16); // base 16 is binary, returns lower case A-F
  return outputString;
}

int stringToInteger(const char *str1) 
{
  int i;
  i = atoi(inputString1);
  return i;
}

char* parseString(const char *str1) 
{
  strcpy(outputString, str1); // copy string
  return outputString;
}

int basicInstr(const char *haystack, const char *needle) // same as Basic instr
{
  char *e;
  int index;
  e = strstr(haystack, needle); // get pointer to the string. See also strchr which looks for just a single character
  index = (int) (e - inputString1); // find where the substring is
  index = index + 1; // add one so same as Basic
  return index;
}

int basicLen(const char *str1)
{
  int i;
  i = strlen(str1);
  return i;
}

uint8_t basicAsc(const char *str1) // returns ascii value of the left most character in a string. 
{
  uint8_t c;
  c = str1[0]; // get the value
  return c;
}

char* basicChr(uint8_t c) // convert c to a string of length 1 character
{
  outputString[0] = c;
  outputString[1] = '\0'; // null terminator
  return outputString;
}

char* basicHex(uint32_t n) // can be any sort of number, long, byte, uint8,16,32 etc
{
  ltoa(n, outputString, 16); // base 16 is binary, returns lower case A-F
  return outputString;
}

char* basicStr(long n)
{
  ltoa(n, outputString, 10); // base 10
  return outputString;
}

long basicVal(const char *str1) 
{
  long i;
  i = atol(inputString1); // returns a long but can cast into other types
  return i;
}

void basicValTest() 
{
  long i;
  const static char s1[] PROGMEM = "4567"; 
  strcpy_P(inputString1, s1); 
  i = basicVal(inputString1); // value is in i
  printNumber(i);
}

char* basicStringAdd(const char *str1, const char *str2) // adds string 2 to the end of string 1
{
    strcat((char*) str1,(char*) str2); // add strings together, answer in str1
    strcpy(outputString, str1); // copy string to outputstring
    return outputString;
}


