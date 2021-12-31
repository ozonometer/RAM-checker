#include <Arduino.h>
#include <math.h>
// address pins
const byte AD0 = 0, AD1 = 1, AD2 = 2, AD3 = 3, AD4 = 4, AD5 = 5, AD6 = 6, AD7 = 7, AD8 = 8, AD9 = 9, AD10 = 10, AD11 = 11, AD12 = 12, AD13 = 13, AD14 = 23;
// DDta pins
const byte DD0 = 30, DD1 = 31, DD2 = 32, DD3 = 33, DD4 = 34, DD5 = 35, DD6 = 36, DD7 = 37;
// controll pins
const byte OE = 38, WE = 39;

int b0 = 0, b1 = 0, b2 = 0, b3 = 0, b4 = 0, b5 = 0, b6 = 0, b7 = 0;

const int nibbles = 4;            // used to create array to store hex conversion
const int addressPins = 15;             // size of max address, AS7C256 has address pins A0-A14 so max size of address is 15 bytes

int binStringToInt(const char binary[], int bytes);
void intToBinaryChars(int *binaryArr, int integer, int bytes);
void intToHex(char *hex, int integer, int bytes, int nibbles);
void pinsInput();
void pinsOutput();
void writeToRam(int b0, int b1, int b2, int b3, int b4, int b5, int b6, int b7);
void readData(int *binaryRpointer);
int binIntArrayToInt(int *binaryP, int bytes);
void setAddresIntValue(int address);
int binIntArrayToInt(int *binaryP, int bytes);
void setAddresIntValue(int address);



void setup() {
  // address pins always outputs
  pinMode(AD0, OUTPUT);
  pinMode(AD1, OUTPUT);
  pinMode(AD2, OUTPUT);
  pinMode(AD3, OUTPUT);
  pinMode(AD4, OUTPUT);
  pinMode(AD5, OUTPUT);
  pinMode(AD6, OUTPUT);
  pinMode(AD7, OUTPUT);
  pinMode(AD8, OUTPUT);
  pinMode(AD9, OUTPUT);
  pinMode(AD10, OUTPUT);
  pinMode(AD11, OUTPUT);
  pinMode(AD12, OUTPUT);
  pinMode(AD13, OUTPUT);
  pinMode(AD14, OUTPUT);
  pinMode(OE, OUTPUT);
  pinMode(WE, OUTPUT);
  digitalWrite(OE, 1);
  digitalWrite(WE, 1);
  SerialUSB.begin(9600);
  SerialUSB.println("Test memory begins!");
}

void loop() {

  int maxAddress = 32768; // AS7C256 - Organization: 32,768 words × 8 bits
  //write to each memory cell
  pinsOutput();
  for(int address = 0; address < maxAddress; address++) {
    setAddresIntValue(address);
    digitalWrite(WE, 0); // enable write, pulling WE low
    delayMicroseconds(5);
    writeToRam(1, 1, 1, 1, 1, 1, 1, 1);
    digitalWrite(WE, 1); // disable write
    SerialUSB.print("\nWriting cell # ");
    SerialUSB.print(address);
  }
  SerialUSB.println("Write completed!");
  //read from each memory cell
  int binaryResultArr[8];       // array to store int result
  int *binaryRpointer;               // pointer to start of array
  char hexArr[nibbles];       // array to store address hex
  char *pChar;                // pointer to start of array
  pinsInput();
  for(int address = 0; address < maxAddress; address++) {
    // set address
    setAddresIntValue(address);
    // read
    binaryRpointer = &binaryResultArr[0];
    digitalWrite(OE, 0); // enable read, pulling OE low 
    delayMicroseconds(5);
    readData(binaryRpointer);
    digitalWrite(OE, 1); // desable read
    //address convert to hex
    pChar = &hexArr[0];         // set pointer to address of the first array cell
    intToHex(pChar, address, 15, 4);
    SerialUSB.print("\nReading memory address ");
    for(int ik = 0; ik < 4; ik++) {
     SerialUSB.print(hexArr[ik]);  
    }
    SerialUSB.print(", data = ");
    for(int in = 0; in < 8; in++) {
     SerialUSB.print(binaryResultArr[in]);  
    }
  }

 }

void pinsInput() {
  pinMode(DD0, INPUT);
  pinMode(DD1, INPUT);
  pinMode(DD2, INPUT);
  pinMode(DD3, INPUT);
  pinMode(DD4, INPUT);
  pinMode(DD5, INPUT);
  pinMode(DD6, INPUT);
  pinMode(DD7, INPUT);
}

void pinsOutput() {
  pinMode(DD0, OUTPUT);
  pinMode(DD1, OUTPUT);
  pinMode(DD2, OUTPUT);
  pinMode(DD3, OUTPUT);
  pinMode(DD4, OUTPUT);
  pinMode(DD5, OUTPUT);
  pinMode(DD6, OUTPUT);
  pinMode(DD7, OUTPUT);
}

void writeToRam(int b0, int b1, int b2, int b3, int b4, int b5, int b6, int b7) {
  digitalWrite(DD0, b0);
  digitalWrite(DD1, b1);
  digitalWrite(DD2, b2);
  digitalWrite(DD3, b3);
  digitalWrite(DD4, b4);
  digitalWrite(DD5, b5);
  digitalWrite(DD6, b6);
  digitalWrite(DD7, b7);
}

void setAddresIntValue(int address){
  int binaryArr[addressPins];       // array to store result
    int *binaryP;               // pointer to start of array
    binaryP = &binaryArr[0];    // set pointer to address of the first array cell
    intToBinaryChars(binaryP, address, addressPins);
    // set address
    digitalWrite(AD0, binaryArr[0]);
    digitalWrite(AD1, binaryArr[1]);
    digitalWrite(AD2, binaryArr[2]);
    digitalWrite(AD3, binaryArr[3]);
    digitalWrite(AD4, binaryArr[4]);
    digitalWrite(AD5, binaryArr[5]);
    digitalWrite(AD6, binaryArr[6]);
    digitalWrite(AD7, binaryArr[7]);
    digitalWrite(AD8, binaryArr[8]);
    digitalWrite(AD9, binaryArr[9]);
    digitalWrite(AD10, binaryArr[10]);
    digitalWrite(AD11, binaryArr[11]);
    digitalWrite(AD12, binaryArr[12]);
    digitalWrite(AD13, binaryArr[13]);
    digitalWrite(AD14, binaryArr[14]);
    //setAddres(binaryArr[0], binaryArr[1], binaryArr[2], binaryArr[3], binaryArr[4], binaryArr[5], binaryArr[6], binaryArr[7], 
    //          binaryArr[8], binaryArr[9], binaryArr[10], binaryArr[11], binaryArr[12], binaryArr[13], binaryArr[14]);
}

void setAddres(int b0, int b1, int b2, int b3, int b4, int b5, int b6, int b7, int b8, int b9, int b10, int b11, int b12, int b13, int b14) {
  digitalWrite(AD0, b0);
  digitalWrite(AD1, b1);
  digitalWrite(AD2, b2);
  digitalWrite(AD3, b3);
  digitalWrite(AD4, b4);
  digitalWrite(AD5, b5);
  digitalWrite(AD6, b6);
  digitalWrite(AD7, b7);
  digitalWrite(AD8, b8);
  digitalWrite(AD9, b9);
  digitalWrite(AD10, b10);
  digitalWrite(AD11, b11);
  digitalWrite(AD12, b12);
  digitalWrite(AD13, b13);
  digitalWrite(AD14, b14);
}

/**
 * This method converts bytes string into integer
 * @param binary string in binary form
 * @param bytes byte size of the number being converted
 * @return integer value to convert
 */
int binStringToInt(const char binary[], int bytes) {
    int multiply = pow (2, bytes - 1);
    int total = 0;
    for(int i = 0; binary[i] != '\0'; ++i) {
        if(binary[i] == '1') {
          total = total + multiply;
        }
        multiply = multiply / 2;
    }
    return total;
}

/**
 * This method converts integer to bytes array
 * @param binaryArr pointer to the address array 0 index used to store result
 * @param integer integer to convert
 * @param bytes size of the integer in binary representation
 */
void intToBinaryChars(int *binaryArr, int integer, int bytes) {
    for(int m = 0; m < bytes; m++) {
        *binaryArr++ = 0;
    }
    int num = integer;
    while (num > 0) {
        if(num % 2 == 1) {
            *binaryArr-- = 1;
        } else {
            *binaryArr-- = 0;
        }
        num = num / 2;
    }
}

/**
 * This method converts integer to hex
 * @param hex pointer to the address array 0 index used to store result
 * @param integer integer to convert
 * @param bytes size of integer in bytes, AS7C256 has address pins A0-A14 so max size of address is 15 bytes (7FFF)
 * @param nibbles size of nibbles to store hex result, 1 nibble = 4 bites
 */
void intToHex(char *hex, int integer, int bytes, int nibbles) {
    int binary[bytes]; // store binary representation
    int i = bytes;          // counter to loop through binary array and store values
    int num = integer;
    // clear the array of junk values
    for(int m = 0; m <= bytes; m++) {
        binary[m] = 0;
    }
    // loop backwards
    while (num > 0) {
        if(num % 2 == 1) {
            binary[i] = 1;
        }
        i--;
        num = num / 2;
    }

    int power = pow (2, nibbles - 1); // largest bite value
    int multiply = power;
    int value = 0;
    for(int j = 0; j <= bytes + 1; j++) {
        if(multiply == 0) {     // when reached zero, calculate next nibble value
            multiply = power;
            // convert value to hex
            switch (value) {
                case 10:
                    *hex++ = 'A';
                    break;
                case 11:
                    *hex++ = 'B';
                    break;
                case 12:
                    *hex++ = 'C';
                    break;
                case 13:
                    *hex++ = 'D';
                    break;
                case 14:
                    *hex++ = 'E';
                    break;
                case 15:
                    *hex++ = 'F';
                    break;
                default:
                    *hex++ = value + 48; // to display num char
            }
            value = 0; // reset value for the next nibble
        }
        // if byte is 1 then calculate by its value
        if (binary[j] == 1) {
            value = value + multiply; 
        }
        multiply = multiply / 2;
    }
}

/**
 * This method reads data from data pins and stores it in array 
 * @param binaryRpointer pointer to the address array 0 index used to store result
 */
void readData(int *binaryRpointer) {
  *binaryRpointer = digitalRead(DD0);
  *binaryRpointer++ = digitalRead(DD1);
  *binaryRpointer++ = digitalRead(DD2);
  *binaryRpointer++ = digitalRead(DD3);
  *binaryRpointer++ = digitalRead(DD4);
  *binaryRpointer++ = digitalRead(DD5);
  *binaryRpointer++ = digitalRead(DD6);
  *binaryRpointer++ = digitalRead(DD7);
}

/**
 * This method converts binary int array to integer
 * @param binaryP pointer to the address array 0 index used to store result
 * @param bytes size of the integer in binary representation, 8 for data bus
 * @return int value
 */
int binIntArrayToInt(int *binaryP, int bytes) {
    int multiply = pow (2, bytes - 1);
    int total = 0;
    for(int i = 0; i < bytes; i++) {
        int value = *binaryP++;
        if(value == 1) {
            total = total + multiply;
        }
        multiply = multiply / 2;
    }
    return total;
}