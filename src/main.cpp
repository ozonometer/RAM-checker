// Mykola Gutsaliuk 1/1/2022
// Script to check old static ram chips
#include <Arduino.h>
#include <math.h>
// address pins
const byte AD0 = 22, AD1 = 23, AD2 = 24, AD3 = 25, AD4 = 26, AD5 = 27, AD6 = 28, AD7 = 29, AD8 = 30, AD9 = 31, AD10 = 32, AD11 = 33, AD12 = 34, AD13 = 35, AD14 = 36;
// Data pins
const byte DD0 = 40, DD1 = 41, DD2 = 42, DD3 = 43, DD4 = 44, DD5 = 45, DD6 = 46, DD7 = 47;
// control pins
const byte OE = 50, WE = 51;
// set max memory address
int maxAddress = 32768; // AS7C256 - Organization: 32,768 words × 8 bits

const int nibbles = 4;                          // used to create array to store hex conversion
const int addressPins = 15;                     // size of max address, AS7C256 has address pins A0-A14 so max size of address is 15 bytes
boolean pass = true;                            // test result pass
int testRuns = 1;                               // number of thest runs
int loops = 0;                                  // do not change, loop counter

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
void convertInToHexPrint(int address);
void convertInToHexPrint(int address, int maxAddress);
int test(int writeData[], int maxAddress);

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
    delay(5000);
    if(loops < testRuns) {
        int testPassed = 0;
        // test 1
        int writeData1[8] = {0, 0, 0, 0, 0, 0, 0, 0};    // data to write
        testPassed = test(writeData1, maxAddress);
        if(testPassed == 0) {
            pass = false;
        }
        // test 2
        int writeData2[8] = {1, 1, 1, 1, 1, 1, 1, 1};    // data to write
        testPassed = test(writeData2, maxAddress);
        if(testPassed == 0) {
            pass = false;
        }
        // test 3
        int writeData3[8] = {1, 0, 1, 0, 1, 0, 1, 0};    // data to write
        testPassed = test(writeData3, maxAddress);
        if(testPassed == 0) {
            pass = false;
        }
        if(pass) {
            SerialUSB.println("\nTEST PASSED");
        } else {
            SerialUSB.println("\nTEST FAILED!");
        }
    }
    loops++;
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

/**
 * Test which writes 
 * @param writeData 8 bit data patern to write and then read back
 * @return int, if test passed then return 1, if fails then return 0
 */
int test(int writeData[], int maxAddress) {
    int result = 1; // test result
    //write to each memory cell
    pinsOutput();
    for(int address = 0; address < maxAddress; address++) {
        setAddresIntValue(address);
        //delay(2);
        digitalWrite(WE, 0); // enable write, pulling WE low
        //delay(2);
        writeToRam(writeData[0], writeData[1], writeData[2], writeData[3], writeData[4], writeData[5], writeData[6], writeData[7]);
        //delay(2);
        digitalWrite(WE, 1); // disable write
        //delay(2);
        SerialUSB.print("\nWriting to memory address ");
        convertInToHexPrint(address);
    }
    SerialUSB.println("Write completed!");
    //read from each memory cell
    int binaryResultArr[8];       // array to store int result
    int *binaryRpointer;          // pointer to start of array
    pinsInput();
    for(int address = 0; address < maxAddress; address++) {
        // set address
        setAddresIntValue(address);
        // read
        //delay(2);
        digitalWrite(OE, 0);        // enable read, pulling OE low 
        //delay(2);
        binaryRpointer = &binaryResultArr[0];
        readData(binaryRpointer);
        //delay(2);
        digitalWrite(OE, 1);        // desable read
        //delay(2);
        //address convert to hex
        SerialUSB.print("\nReading from memory address ");
        convertInToHexPrint(address);
        boolean dataSame = true;
        SerialUSB.print(", data = ");
        for(int in = 0; in < 8; in++) {
            SerialUSB.print(binaryResultArr[in]);
            if(binaryResultArr[in] != writeData[in]) {
                dataSame = false;
            }
        }
        if(dataSame) {
            SerialUSB.print(" pass");
        } else {
            SerialUSB.print(" fail");
            result = 0; // this cell fail, entire test fail
        }
    } 
    return result;
}

/**
 * This will convert int to hex and print to serial
 * @param address integer to convert to hex
 */
void convertInToHexPrint(int address){
    char hexArr[nibbles];         // array to store address hex
    char *pChar;                  // pointer to start of array
    //address convert to hex
    pChar = &hexArr[0];         // set pointer to address of the first array cell
    intToHex(pChar, address, 15, 4);
    for(int ik = 0; ik < 4; ik++) {
        SerialUSB.print(hexArr[ik]);  
    }
}

void setAddresIntValue(int address){
  int binaryArr[addressPins];       // array to store result
    int *binaryP;                   // pointer to start of array
    binaryP = &binaryArr[0];        // set pointer to address of the first array cell
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
 * This method converts integer to bytes array, then to be used to write it to data pins
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
 * This method converts an integer to hex, used to display data in human-readable form
 * @param hex pointer to the address array 0 index used to store result
 * @param integer integer to convert
 * @param bytes size of integer in bytes, AS7C256 has address pins A0-A14 so max size of address is 15 bytes (7FFF)
 * @param nibbles size of nibbles to store hex result, 1 nibble = 4 bites
 */
void intToHex(char *hex, int integer, int bytes, int nibbles) {
    int binary[bytes];      // store binary representation
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

    int power = pow (2, nibbles - 1);   // largest bite value
    int multiply = power;
    int value = 0;
    for(int j = 0; j <= bytes + 1; j++) {
        if(multiply == 0) {             // when reached zero, calculate next nibble value
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
            value = 0;                  // reset value for the next nibble
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
  *binaryRpointer++ = digitalRead(DD0);
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