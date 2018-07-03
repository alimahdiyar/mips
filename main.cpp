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
                Register reg;
                registers.insert(make_pair(registerName, reg));
            }
            return registers.at(registerName);
        }
    };


long getUnsignedValue(char b) {
    if (b >= 0) { return b; }

    return 256 + b;
}
class ProgramCounter :  public Register{
public:
    ProgramCounter() : Register(0x1000){} //initial value

    void increment(){
        newValue = getValue() + 4;
    }


};
class PipelineRegister : public RegisterFile{
public:
    PipelineRegister () : RegisterFile() {
        Register reg(0xFF);
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

class Decode{
public:
    static const int NOP = 0xFF;
private:
// values for decoding the instruction
    static const int OPCODE_MASK = 0xFC000000;
    static const int OPCODE_SHIFT = 26;
    static const int RS_MASK = 0x3E00000;
    static const int RS_SHIFT = 21;
    static const int RT_MASK = 0x1F0000;
    static const int RT_SHIFT = 16;
    static const int RD_MASK = 0xF800;
    static const int RD_SHIFT = 11;
    static const int SHAMT_MASK = 0x7C0;
    static const int SHAMT_SHIFT = 6;
    static const int FUNCT_MASK = 0x3F;
    static const int FUNCT_SHIFT = 0;
    static const int IMMEDIATE_MASK = 0xFFFF;
    static const int IMMEDIATE_SHIFT = 0;
    static const int ADDRESS_MASK = 0x3FFFFFF;
    static const int ADDRESS_SHIFT = 0;
// values for opcodes and funct codes
    static const int ARITH_OP_CODE = 0;
    static const int ADD_FUNCT = 0x20;
    static const int SUB_FUNCT = 0x22;
    static const int AND_FUNCT = 0x24;
    static const int OR_FUNCT = 0x25;
    static const int NOR_FUNCT = 0x27;
    static const int SLT_FUNCT = 0x2a;
    static const int ADDI = 0x8;
    static const int ANDI = 0xC;
    static const int ORI = 0xD;
    static const int SLTI = 0xA;
    static const int BEQ = 0x4;
    static const int BNE = 0x5;
    static const int J = 0x2;
    static const int JR_FUNCT = 0x8;
    static const int LW = 0x23;
    static const int SW = 0x2B;
    static const int HLT = 0x3F;


    const PipelineRegister if_id;
    const PipelineRegister id_ex;
    const RegisterFile registerFile;
    const ProgramCounter pc;
public:
    Decode(const PipelineRegister &if_id, const PipelineRegister &id_ex, const RegisterFile &registerFile,
           const ProgramCounter &pc) : if_id(if_id), id_ex(id_ex), registerFile(registerFile), pc(pc) {}

 void run() {
     //TODO: bugs
     /*
     long instruction = if_id.getValue(RegisterName.INSTRUCTION);
     long opCode = (instruction & OPCODE_MASK) >> OPCODE_SHIFT;
     long rs = (instruction & RS_MASK) >> RS_SHIFT;
     long rt = (instruction & RT_MASK) >> RT_SHIFT;
     long rd = (instruction & RD_MASK) >> RD_SHIFT;
     long shamt = (instruction & SHAMT_MASK) >> SHAMT_SHIFT;
     long funct = (instruction & FUNCT_MASK) >> FUNCT_SHIFT;
     long immediate = (instruction & IMMEDIATE_MASK) >> IMMEDIATE_SHIFT;
     long address = (instruction & ADDRESS_MASK) >> ADDRESS_SHIFT;

     id_ex.setValue(RegisterName.R_S, rs);
     id_ex.setValue(RegisterName.R_T, rt);
     id_ex.setValue(RegisterName.R_D, rd);
     id_ex.setValue(RegisterName.SHAMT, shamt);
     id_ex.setValue(RegisterName.IMMEDIATE, immediate);
     id_ex.setValue(RegisterName.ADDRESS, address);
     id_ex.setValue(RegisterName.OP_CODE, opCode);
     */
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
    cout << RegisterName::Names::INSTRUCTION ;
    return 0;
}