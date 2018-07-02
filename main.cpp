#include <iostream>
#include <vector>
#include <map>

//for file reading
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

/*
class Register{
public:
    bool enable= true; //value jadid biyad dakhel reg ya na

    Register(){
        Register::value = 0;
    }

    Register(long value){
        Register::value = value;
    }

    long getValue() const {
        return value;
    }

    void setValue(long newValue) {
        Register::newValue = newValue;
    }
    void regClockcycle(){
        if(enable){
            if(newValue != -1){
                value=newValue;
                newValue=-1;
            } else{
                enable=true;
            }
        }
    }
private:
    long value;// meghdar register
protected:
    long newValue=-1;//value jadid ke mikhad rekhte beshe dakhel reg
};
class RegisterName{
public:
    enum Names {
        REG_0,
        REG_1,
        REG_2,
        REG_3,
        REG_4,
        REG_5,
        REG_6,
        REG_7,
        REG_8,
        REG_9,
        REG_10,
        REG_11,
        REG_12,
        REG_13,
        REG_14,
        REG_15,
        REG_16,
        REG_17,
        REG_18,
        REG_19,
        REG_20,
        REG_21,
        REG_22,
        REG_23,
        REG_24,
        REG_25,
        REG_26,
        REG_27,
        REG_28,
        REG_29,
        REG_30,
        REG_31,
        INSTRUCTION = false,
        REG_S = false,
        REG_T = false,
        REG_D = false,
        IMMEDIATE = false,
        SHAMT = false,
        ADDRESS = false,
        REG_DST = false,
        ALU_SRC = false,
        MEM_TO_REG = false,
        REG_WRITE = false,
        MEM_READ = false,
        MEM_WRITE = false,
        BRANCH = false,
        BRANCH_NE = false,
        ALU_OP = false,
        JUMP = false,
        JUMP_SRC = false,
        READ_DATA_1 = false,
        READ_DATA_2 = false,
        ALU_RESULT = false,
        MEM_RESULT = false,
        WRITE_DATA = false,
        HALT = false,
        OP_CODE = false,
        PC = false
    };

};

class MemoryStore{
private:
    vector<long> memory;
public:

    long getValue(long location){
        long memAddress = location >> 2;
        if(memAddress >= memory.size()){
            throw "Memory address out of bounds";
        }
        return memory[(int)memAddress];
    }

    void storeValue(long location, long value) {
        long memoryAddress = location >> 2;

        if (memoryAddress >= memory.size()) {
            throw "Memory address out of bounds";
        }
        memory[(int)memoryAddress] =  value;
    }
    // age 1000 masalan bod adade manfi hast ba in mosbat mishe
    long getUnsignedValue(char b) {
        if (b >= 0) { return b; }

        return 256 + b;
    }

};
class RegisterFile {
protected:
    map<RegisterName::Names , Register> x;
};
 */
int main()
{
    char str [80];
    float f;
    FILE * pFile;

    pFile = fopen ("myfile.txt","w+");
    fprintf (pFile, "%f %s", 3.1416, "PI");
    rewind (pFile);
    fscanf (pFile, "%f", &f);
    fscanf (pFile, "%s", str);
    fclose (pFile);
    printf ("I have read: %f and %s \n",f,str);
    return 0;
}