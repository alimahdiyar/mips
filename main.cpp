#include <iostream>
#include <vector>
#include <map>

//for file reading
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

class Register {
public:
    bool enable = true; //value jadid biyad dakhel reg ya na

    Register() {
        Register::value = 0;
    }

    Register(long value) {
        Register::value = value;
    }

    long getValue() const {
        return value;
    }

    void setValue(long newValue) {
        Register::newValue = newValue;
    }

    void tick() {
        if (enable) {
            if (newValue != -1) {
                value = newValue;
                newValue = -1;
            } else {
                enable = true;
            }
        }
    }

private:
    long value;// meghdar register
protected:
    long newValue = -1;//value jadid ke mikhad rekhte beshe dakhel reg
};

class RegisterName {
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

class MemoryStore {
private:
    vector<long> memory;
public:

    MemoryStore(char filename[]) {
        char f[4];
        FILE *pFile;
        pFile = fopen(filename, "r");
        while (fscanf(pFile, "%c", &f[0]) != EOF) {
            fscanf(pFile, "%c", &f[1]);
            fscanf(pFile, "%c", &f[2]);
            fscanf(pFile, "%c", &f[3]);

            long word = getUnsignedValue(f[0]);
            word += getUnsignedValue(f[1]) << 8;
            word += getUnsignedValue(f[2]) << 16;
            word += getUnsignedValue(f[3]) << 24;

            // add the word to our memory store
            memory.push_back(word);
        }
        fclose(pFile);
    }

    long getValue(long location) {
        long memAddress = location >> 2;
        if (memAddress >= memory.size()) {
            throw "Memory address out of bounds";
        }
        return memory[(int) memAddress];
    }

    void storeValue(long location, long value) {
        long memoryAddress = location >> 2;

        if (memoryAddress >= memory.size()) {
            throw "Memory address out of bounds";
        }
        memory[(int) memoryAddress] = value;
    }

    // age 1000 masalan bod adade manfi hast ba in mosbat mishe
    long getUnsignedValue(char b) {
        if (b >= 0) { return b; }

        return 256 + b;
    }

};


class RegisterFile {
    protected:
        map<RegisterName::Names, Register> registers;
    private:
        bool enable = true;
    public:
        void setValue(RegisterName::Names registerName, long value) {
            Register reg = getRegister(registerName);
            reg.setValue(value);
        }

        long getValue(RegisterName::Names registerName) {
            return getRegister(registerName).getValue();
        }

        void disableWrite() {
            enable = false;
        }

        void tick() {
            if (enable) {
                for(map<RegisterName::Names , Register>::iterator k = registers.begin(); k != registers.end(); k++){
                    (*k).second.tick();
                }
            } else {
                enable = true;
            }
        }


    protected:
        bool validRegister(RegisterName::Names registerName) {
            //TODO: write isPrimitive()
            //return registerName.isPrimitive();
        }

    private:
        Register getRegister(RegisterName::Names registerName) {
            if (!validRegister(registerName)) {
                throw "Invalid register";
            }
            if (registers.find(registerName) == registers.end()) {
                Register reg();
                registers.insert(make_pair(registerName, reg));
            }
            return registers.at(registerName);
        }
    };


long getUnsignedValue(char b) {
    if (b >= 0) { return b; }

    return 256 + b;
}

class Decode{
public:
    static const int NOP = 0xFF;
};

class PipelineRegister : public RegisterFile{
public:
    PipelineRegister () : RegisterFile() {
        Register reg(Decode::NOP);
        //TODO: registers.put(RegisterName.OP_CODE, register);
    }

    void forwardValues(PipelineRegister target) {


        for(map<RegisterName::Names , Register>::iterator k = registers.begin(); k != registers.end(); k++){
            target.setValue((*k).first, (*k).second.getValue());
        }


    }
protected:
    bool validRegister(RegisterName::Names registerName) {
         return true;
     }

};

class PipelineStage{
public:
    virtual void run();
};

int main() {
    //TODO: delete this
    /*
    map<string, string> x;
    x.insert(make_pair("maryam", "sofi"));
    x.insert(make_pair("ali", "camel"));
    for(map<string, string>::iterator k = x.begin(); k != x.end(); k++){
        cout << (*k).second << endl;
    }
    */

    return 0;
}