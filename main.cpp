#include <iostream>
#include <vector>
using namespace std;


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
    const int REG_0 = 0;
    const int REG_1 = 1;
    const int REG_2 = 2;
    const int REG_3 = 3;
    const int REG_4 = 4;
    const int REG_5 = 5;
    const int REG_6 = 6;
    const int REG_7 = 7;
    const int REG_8 = 8;
    const int REG_9 = 9;
    const int REG_10 = 10;
    const int REG_11 = 11;
    const int REG_12 = 12;
    const int REG_13 = 13;
    const int REG_14 = 14;
    const int REG_15 = 15;
    const int REG_16 = 16;
    const int REG_17 = 17;
    const int REG_18 = 18;
    const int REG_19 = 19;
    const int REG_20 = 20;
    const int REG_21 = 21;
    const int REG_22 = 22;
    const int REG_23 = 23;
    const int REG_24 = 24;
    const int REG_25 = 25;
    const int REG_26 = 26;
    const int REG_27 = 27;
    const int REG_28 = 28;
    const int REG_29 = 29;
    const int REG_30 = 30;
    const int REG_31 = 31;
    bool INSTRUCTION = false;
    bool R_S = false;
    bool R_T =false;
    bool R_D = false;
    bool IMMEDIATE = false;
    bool SHAMT = false;
    bool ADDRESS = false;
    bool REG_DST = false;
    bool ALU_SRC = false;
    bool MEM_TO_REG = false;
    bool REG_WRITE = false;
    bool MEM_READ = false;
    bool MEM_WRITE = false;
    bool BRANCH = false;
    bool BRANCH_NE = false;
    bool ALU_OP = false;
    bool JUMP = false;
    bool JUMP_SRC = false;
    bool READ_DATA_1 = false;
    bool READ_DATA_2 = false;
    bool ALU_RESULT = false;
    bool MEM_RESULT = false;
    bool WRITE_DATA = false;
    bool HALT = false;
    bool OP_CODE = false;
    bool PC = false;

    bool getIsPrimitive() {
        return isPrimitive;
    }

private:

    RegisterName(){
        RegisterName::isPrimitive = false;
    }

    RegisterName(bool isPrimitive){
        RegisterName::isPrimitive = isPrimitive;
    }

    bool isPrimitive;

};

class MemoryStore{

};



int main() {
    Register a(5);
    Register b(6);
    cout << a.getValue() << endl;

    return 0;
}