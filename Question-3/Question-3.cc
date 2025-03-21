#include <iostream>
#include <fstream>
#include <string>

// Question 3: This is an extension task that requires you to decode sensor data from a CAN log file.
// CAN (Controller Area Network) is a communication standard used in automotive applications (including Redback cars)
// to allow communication between sensors and controllers.
//
// Your Task: Using the definition in the Sensors.dbc file, extract the "WheelSpeedRR" values
// from the candump.log file. Parse these values correctly and store them in an output.txt file with the following format:
// (<UNIX_TIME>): <DECODED_VALUE>
// eg:
// (1705638753.913408): 1234.5
// (1705638754.915609): 6789.0
// ...
// The above values are not real numbers; they are only there to show the expected data output format.
// You do not need to use any external libraries. Use the resources below to understand how to extract sensor data.
// Hint: Think about manual bit masking and shifting, data types required,
// what formats are used to represent values, etc.
// Resources:
// https://www.csselectronics.com/pages/can-bus-simple-intro-tutorial
// https://www.csselectronics.com/pages/can-dbc-file-database-intro

//###### PLANNING AND RESEARCH #############################################
// step 1 create txt file
// step 2 iterate through candump.log
// step 3 if ID matches (id = 705(hex), 1797(decimal))
// step 4 decode value
// step 5 write timestamp and decoded value to file

// to create file:
// ofstream FileName('filename.txt");
// to write to file:
// FileName << "Hello World!";
// Don't forget to close the file... FileName.close()

// To read from a file:
// ifstream ReadFile("filename.txt")
// *must also close
// note that getline() returns 0 at eof so can use in while loop to iterate

// notes for decoding data
// big endianness so no need to swap
// 57 72 75 65 03 5D A4 40
//   FR    FL    RR    RL
// we can bit mask to isolate RR
// perform bitwise AND with:
// 00 00 00 00 FF FF 00 00
// *** or just take a substring of data corresponding to the part we need
// *** we can do this as the data for WheelSpeed is always 8 bytes
// convert from hex to decimal 
// multiply by scale 0.1

// string handling functions:
// sustr(pos, len)
// find(substr) returns pos in str of first char in substr
// 
//##########################################################################
using namespace std;

int main(void) {
    string data_line;
    //not sure whether to write file in build or Question-3
    ofstream rr_wheel_speed("../Question-3/output.txt");
    ifstream raw_data("../Question-3/candump.log");
    if (raw_data.is_open()) {
        while (getline(raw_data, data_line)) {
            //checking if id of data_line matches that of WheelSpeed
            if (data_line.substr(26,3) == "705") {
                string timestamp = data_line.substr(0, 19); 
                string rr_data = data_line.substr(38,4);
                // converting hex into decimal and multiplying by the scale
                float val = stoi(rr_data, nullptr, 16) * 0.1;
                rr_wheel_speed << timestamp << ": " << val << endl;
            }
        }
        raw_data.close();
    }
    rr_wheel_speed.close();
    return 0;
}
