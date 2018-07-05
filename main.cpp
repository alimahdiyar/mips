#include <iostream>
#include <vector>
#include <map>

//for file reading
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;
string getBinary(unsigned int value){
    string str(64, '0');

    for(unsigned int i = 0; i < 64; i++)
    {
        if( (1ll << i) & value)
            str[63-i] = '1';
    }

    return str;
}
string getBinary(int value){
    string str(64, '0');

    for(unsigned int i = 0; i < 64; i++)
    {
        if( (1ll << i) & value)
            str[63-i] = '1';
    }

    return str;
}

string getBinary(long value){
    string str(64, '0');

    for(unsigned int i = 0; i < 64; i++)
    {
        if( (1ll << i) & value)
            str[63-i] = '1';
    }

    return str;
}

string getBinary(unsigned long value){
    string str(64, '0');

    for(unsigned int i = 0; i < 64; i++)
    {
        if( (1ll << i) & value)
            str[63-i] = '1';
    }

    return str;
}

class Register {
public:
    bool enable = true; //value jadid biyad dakhel reg ya na
//************constructor*****************
    Register() {
        Register::value = 0;
    }

    Register(unsigned int value) {
        Register::value = value;
    }
//*******getter and setter************
    unsigned int getValue() const {
        return value;
    }

    void setValue(unsigned int newValue) {
        Register::newValue = newValue;
    }
//*************cheack mikone age value jadid + besh ********************
    void tick() {
        if (enable) {
            if (newValue != -1) {
                value = newValue;
                newValue = -1;
            }
        } else {
            enable = true;
        }
    }

    void disableWrite(){
        enable = false;
    }
    void enableWrite(){
        enable = true;
    }

    unsigned int newValue = -1;
    unsigned int value;
    // meghdar register
//value jadid ke mikhad rekhte beshe dakhel reg
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
        INSTRUCTION,
        REG_S,
        REG_T,
        REG_D,
        IMMEDIATE,
        SHAMT,
        ADDRESS,
        REG_DST,
        ALU_SRC,
        MEM_TO_REG,
        REG_WRITE,
        MEM_READ,
        MEM_WRITE,
        BRANCH,
        BRANCH_NE,
        ALU_OP,
        JUMP,
        JUMP_SRC,
        READ_DATA_1,
        READ_DATA_2,
        ALU_RESULT,
        MEM_RESULT,
        WRITE_DATA,
        HALT,
        OP_CODE,
        PC
    };
//***************1 add midim reg name k mal in index hast barmigardone*********
    static RegisterName::Names valueOf(unsigned int ordinal) {
        return static_cast<RegisterName::Names>(ordinal);
    }

};
class MemoryStore {
private:
//**********mem include instruction and data mem**********
//*********array************
    vector<unsigned int> memory;
public:
//********constructor************
    //MemoryStore(){}

    // HALT Command for test
    MemoryStore(){
        unsigned int x =
                ((unsigned int) 0x20)
                +(((unsigned int) 0x40) << 8)
                +(((unsigned int) 0x32) << 16)
                +(((unsigned int) 0x02) << 24);
        memory.push_back(x);
        x =
                ((unsigned int) 0b00100010)
                +(((unsigned int) 0b00010000) << 8)
                +(((unsigned int) 0b00010011) << 16)
                +(((unsigned int) 0b00000001) << 24);
        memory.push_back(x);
        memory.push_back((int)(((unsigned int) 0xFC) << 24));
    }

//*********in mips data on word***********
    void readFile(char filename[]) {
        char f[4];
        FILE *pFile;
        pFile = fopen(filename, "r");
        while (fscanf(pFile, "%c", &f[0]) != EOF) {
            fscanf(pFile, "%c", &f[1]);
            fscanf(pFile, "%c", &f[2]);
            fscanf(pFile, "%c", &f[3]);
 // ********create a word from the 4 bytes**********
            unsigned int word = getUnsignedValue(f[3]);
            word += getUnsignedValue(f[2]) << 8;
            word += getUnsignedValue(f[1]) << 16;
            word += getUnsignedValue(f[0]) << 24;

 //********** add the word to our memory store*********
            memory.push_back(word);
        }
        fclose(pFile);
    }

// *********we want to access the memory on a word boundary *******************
    unsigned int getValue(unsigned int location) {
        unsigned int memAddress = location >> 2;
        if (memAddress >= memory.size()) {
            throw "Memory address out of bounds";
        }
        return memory[(int) memAddress];
    }

// *********get the word address from the byte address *******************
    void storeValue(unsigned int location, unsigned int value) {
        unsigned int memoryAddress = location >> 2;

        if (memoryAddress >= memory.size()) {
            throw "Memory address out of bounds";
        }
        memory[(int) memoryAddress] = value;
    }

// ********age 1000 masalan bod adade manfi hast ba in mosbat mishe******************
    unsigned int getUnsignedValue(char b) {
        if (b >= 0) { return b; }

        return 256 + b;
    }

};
class RegisterFile {
private:
        bool enable = true;
    public:
// reg migere meghdar reg ba on esm mikone value k behesh dadim
        void setValue(RegisterName::Names registerName, unsigned int value) {
            if (!validRegister(registerName)) {
                throw "Invalid register";
            }
            if (registers.find(registerName) == registers.end()) {
                registers.insert(make_pair(registerName, Register()));
            }
            registers.at(registerName).setValue(value);
        }

// meghdar reg ba in esm bar migardone
        unsigned int getValue(RegisterName::Names registerName) {
            return getRegister(registerName).getValue();
        }

        void disableWrite() {

            enable = false;
        }
// 1 clock cycle harekat mikone
        void tick() {
            if (enable) {
                for(map<RegisterName::Names , Register>::iterator k = registers.begin(); k != registers.end(); k++){
                    (*k).second.tick();
                }
            } else {
                enable = true;
            }
        }


    map<RegisterName::Names, Register> registers;
protected:
        virtual bool validRegister(RegisterName::Names registerName) {
            if(
            registerName == RegisterName::Names::REG_0 ||
            registerName == RegisterName::Names::REG_1 ||
            registerName == RegisterName::Names::REG_2 ||
            registerName == RegisterName::Names::REG_3 ||
            registerName == RegisterName::Names::REG_4 ||
            registerName == RegisterName::Names::REG_5 ||
            registerName == RegisterName::Names::REG_6 ||
            registerName == RegisterName::Names::REG_7 ||
            registerName == RegisterName::Names::REG_8 ||
            registerName == RegisterName::Names::REG_9 ||
            registerName == RegisterName::Names::REG_10 ||
            registerName == RegisterName::Names::REG_11 ||
            registerName == RegisterName::Names::REG_12 ||
            registerName == RegisterName::Names::REG_13 ||
            registerName == RegisterName::Names::REG_14 ||
            registerName == RegisterName::Names::REG_15 ||
            registerName == RegisterName::Names::REG_16 ||
            registerName == RegisterName::Names::REG_17 ||
            registerName == RegisterName::Names::REG_18 ||
            registerName == RegisterName::Names::REG_19 ||
            registerName == RegisterName::Names::REG_20 ||
            registerName == RegisterName::Names::REG_21 ||
            registerName == RegisterName::Names::REG_22 ||
            registerName == RegisterName::Names::REG_23 ||
            registerName == RegisterName::Names::REG_24 ||
            registerName == RegisterName::Names::REG_25 ||
            registerName == RegisterName::Names::REG_26 ||
            registerName == RegisterName::Names::REG_27 ||
            registerName == RegisterName::Names::REG_28 ||
            registerName == RegisterName::Names::REG_29 ||
            registerName == RegisterName::Names::REG_30 ||
            registerName == RegisterName::Names::REG_31
            ){
                return true;
            }
            return false;
        }

    private:
 //*********1 reg name migere va reg k be on esm hast barmigardone************
        Register getRegister(RegisterName::Names registerName) {
            if (!validRegister(registerName)) {
                throw "Invalid register";
            }
//********mige reg ba in name age vojod nadasht besaz vasam********
            if (registers.find(registerName) == registers.end()) {
                registers.insert(make_pair(registerName, Register()));
            }
            return registers.at(registerName);
        }
    };
class ProgramCounter :  public Register{

/**
 * The program counter that keeps track of what instruction in memory should
 * be executed next.
 */
public:
    ProgramCounter() : Register(0){} //initial value

//***********pc+4 for next instruction (worde 4 ta 4 ta)********
    void increment(){
        newValue = getValue() + 4;
    }


};
class PipelineRegister : public RegisterFile{
public:
    PipelineRegister () : RegisterFile() {
        Register reg(0xFF);

        registers.insert(make_pair(RegisterName::Names::OP_CODE, reg));
    }
// meghdar reg in pipeline mifreste ro target
    void forwardValues(PipelineRegister &target) {

// k yek iterator hast ke ro map harekat mikone
        for(map<RegisterName::Names , Register>::iterator k = registers.begin(); k != registers.end(); k++){
            target.setValue((*k).first, (*k).second.getValue());
        }


    }
protected:
    // function pedaresho over write mikoneh
    bool validRegister(RegisterName::Names registerName) override {
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

//***pipeline Reg*************
    PipelineRegister *if_id;
    PipelineRegister *id_ex;
    RegisterFile *registerFile;
    ProgramCounter *pc;
public:
 //***************decode constructor*******************
    Decode(PipelineRegister *if_id, PipelineRegister *id_ex, RegisterFile *registerFile, ProgramCounter *pc) {
        Decode::if_id = if_id;
        Decode::id_ex = id_ex;
        Decode::registerFile = registerFile;
        Decode::pc = pc;
    }

    Decode() {}

    void run() {
     // (*if_id).getValue    =    if_id->getValue
     unsigned int instruction = if_id->getValue(RegisterName::Names::INSTRUCTION);
     unsigned int opCode = (instruction & OPCODE_MASK) >> OPCODE_SHIFT;
     unsigned int rs = (instruction & RS_MASK) >> RS_SHIFT;
     unsigned int rt = (instruction & RT_MASK) >> RT_SHIFT;
     unsigned int rd = (instruction & RD_MASK) >> RD_SHIFT;
     unsigned int shamt = (instruction & SHAMT_MASK) >> SHAMT_SHIFT;
     unsigned int funct = (instruction & FUNCT_MASK) >> FUNCT_SHIFT;
     unsigned int immediate = (instruction & IMMEDIATE_MASK) >> IMMEDIATE_SHIFT;
     unsigned int address = (instruction & ADDRESS_MASK) >> ADDRESS_SHIFT;

     id_ex->setValue(RegisterName::Names::REG_S, rs);
     id_ex->setValue(RegisterName::Names::REG_T, rt);
     id_ex->setValue(RegisterName::Names::REG_D, rd);
     id_ex->setValue(RegisterName::Names::SHAMT, shamt);
     id_ex->setValue(RegisterName::Names::IMMEDIATE, immediate);
     id_ex->setValue(RegisterName::Names::ADDRESS, address);
     id_ex->setValue(RegisterName::Names::OP_CODE, opCode);
     map<RegisterName::Names , unsigned int> controlLines =
             setControlLines(opCode, funct);
     unsigned int readData1 = registerFile->getValue(RegisterName::valueOf(rs));
     unsigned int readData2 = registerFile->getValue(RegisterName::valueOf(rt));
     (*id_ex).setValue(RegisterName::Names::READ_DATA_1, readData1);
     (*id_ex).setValue(RegisterName::Names::READ_DATA_2, readData2);

     if (id_ex->getValue(RegisterName::Names ::MEM_READ) == 1 &&
         ((id_ex->getValue(RegisterName::Names::REG_T) == rs) ||
          (id_ex->getValue(RegisterName::Names::REG_T) == rt)))
     {
         stallPipeline();
     } else if (controlLines.at(RegisterName::Names ::BRANCH) == 1 &&
                ((controlLines.at(RegisterName::Names ::BRANCH_NE) == 1 &&
                  readData1 != readData2) ||
                 (controlLines.at(RegisterName::Names ::BRANCH_NE) == 0 &&
                  readData1 == readData2)))
     {
         takeBranch(immediate);
     } else if (controlLines.at(RegisterName::Names ::JUMP) == 1) {
         unsigned int jumpAddress = address;

         if (controlLines.at(RegisterName::Names ::JUMP_SRC) == 1) {
             jumpAddress = readData1;

             if (jumpAddress % 4 != 0) {
                 throw ("jump register value not divisible by four");
             }

             jumpAddress /= 4;
         }

         takeJump(jumpAddress);
     }
 }
private :
    map<RegisterName::Names , unsigned int> setControlLines(
            unsigned int opCode, unsigned int funct
    ) {
        map<RegisterName::Names , unsigned int> values ;


        values.insert(make_pair(RegisterName::Names::REG_DST, 0l));
        id_ex->setValue(RegisterName::Names::REG_DST, 0l);
        values.insert(make_pair(RegisterName::Names::ALU_SRC, 0l));
        id_ex->setValue(RegisterName::Names::ALU_SRC, 0l);
        values.insert(make_pair(RegisterName::Names::MEM_TO_REG, 0l));
        id_ex->setValue(RegisterName::Names::MEM_TO_REG, 0l);
        values.insert(make_pair(RegisterName::Names::REG_WRITE, 0l));
        id_ex->setValue(RegisterName::Names::REG_WRITE, 0l);
        values.insert(make_pair(RegisterName::Names::MEM_READ, 0l));
        id_ex->setValue(RegisterName::Names::MEM_READ, 0l);
        values.insert(make_pair(RegisterName::Names::MEM_WRITE, 0l));
        id_ex->setValue(RegisterName::Names::MEM_WRITE, 0l);
        values.insert(make_pair(RegisterName::Names::BRANCH, 0l));
        id_ex->setValue(RegisterName::Names::BRANCH, 0l);
        values.insert(make_pair(RegisterName::Names::BRANCH_NE, 0l));
        id_ex->setValue(RegisterName::Names::BRANCH_NE, 0l);
        values.insert(make_pair(RegisterName::Names::JUMP, 0l));
        id_ex->setValue(RegisterName::Names::JUMP, 0l);
        values.insert(make_pair(RegisterName::Names::JUMP_SRC, 0l));
        id_ex->setValue(RegisterName::Names::JUMP_SRC, 0l);
        values.insert(make_pair(RegisterName::Names::ALU_OP, 0l));
        id_ex->setValue(RegisterName::Names::ALU_OP, 0l);
        values.insert(make_pair(RegisterName::Names::HALT, 0l));
        id_ex->setValue(RegisterName::Names::HALT, 0l);

        if (if_id->getValue(RegisterName::Names::OP_CODE) == NOP) {
            return values;
        }

        switch ((int)opCode) {
            case ARITH_OP_CODE:
                values.insert(make_pair(RegisterName::Names::REG_DST, 1l));
                id_ex->setValue(RegisterName::Names::REG_DST, 1l);

                values.insert(make_pair(RegisterName::Names::REG_WRITE, 1l));
                id_ex->setValue(RegisterName::Names::REG_WRITE, 1l);

                switch ((int)funct) {
                    case SUB_FUNCT:
                        values.insert(make_pair(RegisterName::Names::ALU_OP, 1l));
                        id_ex->setValue(RegisterName::Names::ALU_OP, 1l);
                        break;
                    case AND_FUNCT:
                        values.insert(make_pair(RegisterName::Names::ALU_OP, 2l));
                        id_ex->setValue(RegisterName::Names::ALU_OP, 2l);
                        break;
                    case OR_FUNCT:
                        values.insert(make_pair(RegisterName::Names::ALU_OP, 3l));
                        id_ex->setValue(RegisterName::Names::ALU_OP, 3l);
                        break;
                    case NOR_FUNCT:
                        values.insert(make_pair(RegisterName::Names::ALU_OP, 4l));
                        id_ex->setValue(RegisterName::Names::ALU_OP, 4l);
                        break;
                    case SLT_FUNCT:
                        values.insert(make_pair(RegisterName::Names::ALU_OP, 5l));
                        id_ex->setValue(RegisterName::Names::ALU_OP, 5l);
                        break;
                    case JR_FUNCT:
                        values.insert(make_pair(RegisterName::Names::JUMP, 1l));
                        id_ex->setValue(RegisterName::Names::JUMP, 1l);

                        values.insert(make_pair(RegisterName::Names::JUMP_SRC, 1l));
                        id_ex->setValue(RegisterName::Names::JUMP_SRC, 1l);
                        break;
                }
                break;

            case ADDI:
                values.insert(make_pair(RegisterName::Names::ALU_SRC, 1l));
                id_ex->setValue(RegisterName::Names::ALU_SRC, 1l);

                values.insert(make_pair(RegisterName::Names::REG_WRITE, 1l));
                id_ex->setValue(RegisterName::Names::REG_WRITE, 1l);

                break;

            case ANDI:
                values.insert(make_pair(RegisterName::Names::ALU_SRC, 1l));
                id_ex->setValue(RegisterName::Names::ALU_SRC, 1l);

                values.insert(make_pair(RegisterName::Names::REG_WRITE, 1l));
                id_ex->setValue(RegisterName::Names::REG_WRITE, 1l);

                values.insert(make_pair(RegisterName::Names::ALU_OP, 2l));
                id_ex->setValue(RegisterName::Names::ALU_OP, 2l);

                break;

            case ORI:
                values.insert(make_pair(RegisterName::Names::ALU_SRC, 1l));
                id_ex->setValue(RegisterName::Names::ALU_SRC, 1l);

                values.insert(make_pair(RegisterName::Names::REG_WRITE, 1l));
                id_ex->setValue(RegisterName::Names::REG_WRITE, 1l);

                values.insert(make_pair(RegisterName::Names::ALU_OP, 3l));
                id_ex->setValue(RegisterName::Names::ALU_OP, 3l);

                break;

            case SLTI:
                values.insert(make_pair(RegisterName::Names::ALU_SRC, 1l));
                id_ex->setValue(RegisterName::Names::ALU_SRC, 1l);

                values.insert(make_pair(RegisterName::Names::REG_WRITE, 1l));
                id_ex->setValue(RegisterName::Names::REG_WRITE, 1l);

                values.insert(make_pair(RegisterName::Names::ALU_OP, 5l));
                id_ex->setValue(RegisterName::Names::ALU_OP, 5l);

                break;

            case BEQ:
                values.insert(make_pair(RegisterName::Names::BRANCH, 1l));
                id_ex->setValue(RegisterName::Names::BRANCH, 1l);

                values.insert(make_pair(RegisterName::Names::ALU_OP, 1l));
                id_ex->setValue(RegisterName::Names::ALU_OP, 1l);

                break;

            case BNE:
                values.insert(make_pair(RegisterName::Names::BRANCH, 1l));
                id_ex->setValue(RegisterName::Names::BRANCH, 1l);

                values.insert(make_pair(RegisterName::Names::BRANCH_NE, 1l));
                id_ex->setValue(RegisterName::Names::BRANCH_NE, 1l);

                values.insert(make_pair(RegisterName::Names::ALU_OP, 1l));
                id_ex->setValue(RegisterName::Names::ALU_OP, 1l);

                break;

            case J:
                values.insert(make_pair(RegisterName::Names::JUMP, 1l));
                id_ex->setValue(RegisterName::Names::JUMP, 1l);

                break;

            case LW:
                values.insert(make_pair(RegisterName::Names::ALU_SRC, 1l));
                id_ex->setValue(RegisterName::Names::ALU_SRC, 1l);

                values.insert(make_pair(RegisterName::Names::MEM_TO_REG, 1l));
                id_ex->setValue(RegisterName::Names::MEM_TO_REG, 1l);

                values.insert(make_pair(RegisterName::Names::REG_WRITE, 1l));
                id_ex->setValue(RegisterName::Names::REG_WRITE, 1l);

                values.insert(make_pair(RegisterName::Names::MEM_READ, 1l));
                id_ex->setValue(RegisterName::Names::MEM_READ, 1l);

                break;

            case SW:
                values.insert(make_pair(RegisterName::Names::ALU_SRC, 1l));
                id_ex->setValue(RegisterName::Names::ALU_SRC, 1l);

                values.insert(make_pair(RegisterName::Names::MEM_WRITE, 1l));
                id_ex->setValue(RegisterName::Names::MEM_WRITE, 1l);

                break;

            case HLT:
                values.insert(make_pair(RegisterName::Names::HALT, 1l));
                id_ex->setValue(RegisterName::Names::HALT, 1l);

                break;
        }

        return values;
    }
private:
    void stallPipeline() {
        zeroOutRegister(*id_ex);

        if_id->disableWrite();
        pc->disableWrite();
    }

   void takeBranch(unsigned int addressOffset) {
        zeroOutRegister(*if_id);

        pc->setValue(if_id->getValue(RegisterName::Names::PC) + 4 + 4 * addressOffset);
        pc->enableWrite();
    }

    void takeJump(unsigned int address) {
        zeroOutRegister(*if_id);

        pc->setValue(address * 4);
        pc->enableWrite();
    }

     void zeroOutRegister(PipelineRegister rp) {
        rp.setValue(RegisterName::Names::REG_DST, 0);
        rp.setValue(RegisterName::Names::ALU_SRC, 0);
        rp.setValue(RegisterName::Names::MEM_TO_REG, 0);
        rp.setValue(RegisterName::Names::REG_WRITE, 0);
        rp.setValue(RegisterName::Names::MEM_READ, 0);
        rp.setValue(RegisterName::Names::MEM_WRITE, 0);
        rp.setValue(RegisterName::Names::BRANCH, 0);
        rp.setValue(RegisterName::Names::BRANCH_NE, 0);
        rp.setValue(RegisterName::Names::JUMP, 0);
        rp.setValue(RegisterName::Names::JUMP_SRC, 0);
        rp.setValue(RegisterName::Names::ALU_OP, 0);
        rp.setValue(RegisterName::Names::OP_CODE, NOP);
    }
};
class Execute{
private:
    PipelineRegister *ex_mem;
    PipelineRegister *id_ex;
    PipelineRegister *mem_wb;
public:
    Execute() {}

    Execute(
            PipelineRegister *id_ex,
            PipelineRegister *ex_mem,
            PipelineRegister *mem_wb
    ) {
        Execute::id_ex = id_ex;
        Execute::ex_mem = ex_mem;
        Execute::mem_wb = mem_wb;
    }

    void run() {
        unsigned int aluArg1, aluArg2, writeData;

        unsigned int dest = ex_mem->getValue(RegisterName::Names::REG_DST) == 1 ?
                    ex_mem->getValue(RegisterName::Names::REG_D) :
                    ex_mem->getValue(RegisterName::Names::REG_T);

        unsigned int dest2 = mem_wb->getValue(RegisterName::Names::REG_DST) == 1 ?
                     mem_wb->getValue(RegisterName::Names::REG_D) :
                     mem_wb->getValue(RegisterName::Names::REG_T);

        if (ex_mem->getValue(RegisterName::Names::REG_WRITE) == 1 &&
            dest != 0 &&
            dest == id_ex->getValue(RegisterName::Names::REG_S))
        {
            aluArg1 = ex_mem->getValue(RegisterName::Names::ALU_RESULT);
        } else if (mem_wb->getValue(RegisterName::Names::REG_WRITE) == 1 &&
                   dest2 != 0 &&
                   dest2 == id_ex->getValue(RegisterName::Names::REG_S)) {
            if (mem_wb->getValue(RegisterName::Names::MEM_TO_REG) == 1) {
                aluArg1 = mem_wb->getValue(RegisterName::Names::MEM_RESULT);
            } else {
                aluArg1 = mem_wb->getValue(RegisterName::Names::ALU_RESULT);
            }
        } else {
            aluArg1 = id_ex->getValue(RegisterName::Names::READ_DATA_1);
        }


        if (ex_mem->getValue(RegisterName::Names::REG_WRITE) == 1 &&
            dest != 0 &&
            dest == id_ex->getValue(RegisterName::Names::REG_T))
        {
            writeData = ex_mem->getValue(RegisterName::Names::ALU_RESULT);
        } else if (mem_wb->getValue(RegisterName::Names::REG_WRITE) == 1 &&
                   dest2 != 0 &&
                   dest2 == id_ex->getValue(RegisterName::Names::REG_T)) {
            if (mem_wb->getValue(RegisterName::Names::MEM_TO_REG) == 1) {
                writeData = mem_wb->getValue(RegisterName::Names::MEM_RESULT);
            } else {
                writeData = mem_wb->getValue(RegisterName::Names::ALU_RESULT);
            }
        } else {
            writeData = id_ex->getValue(RegisterName::Names::READ_DATA_2);
        }

        if (id_ex->getValue(RegisterName::Names::ALU_SRC) == 0) {
            aluArg2 = writeData;
        } else {
            aluArg2 = id_ex->getValue(RegisterName::Names::IMMEDIATE);
        }

        id_ex->forwardValues(*ex_mem);

        unsigned int result = 0;

        switch((int)id_ex->getValue(RegisterName::Names::ALU_OP)) {
            case (0):
                result = aluArg1 + aluArg2;
                break;
            case (1):
                result = aluArg1 - aluArg2;
                break;
            case (2):
                result = aluArg1 & aluArg2;
                break;
            case (3):
                result = aluArg1 | aluArg2;
                break;
            case (4):
                result = ~(aluArg1 | aluArg2) & 0xFFFFFFFFl;
                break;
            case (5):
                result = aluArg1 < aluArg2 ? 1 : 0;
                break;
        }

        ex_mem->setValue(RegisterName::Names ::ALU_RESULT, result);
        ex_mem->setValue(RegisterName::Names ::WRITE_DATA, writeData);
    }
};


class Fetch {
private:
    static const int HALT_INSTRUCTION = 0xFC000000;
    PipelineRegister *if_id;
    MemoryStore *memory;
    ProgramCounter *pc;
public:
    Fetch(
            PipelineRegister *if_id,
            MemoryStore *memory,
            ProgramCounter *pc
    ) {
        Fetch::if_id = if_id;
        Fetch::memory = memory;
        Fetch::pc = pc;
    }

    Fetch() {}

    void run() {
        unsigned int instruction = memory->getValue((*pc).getValue());
        //System.out.println(Integer.toHexString(instruction));
        (*if_id).setValue(RegisterName::Names::INSTRUCTION, instruction);


        (*if_id).setValue(RegisterName::Names::OP_CODE, 0);
        (*if_id).setValue(RegisterName::Names::PC, (*pc).getValue());

        if (instruction != HALT_INSTRUCTION) {
            (*pc).increment();
        }
    }
};
class Memory{
    private:
        PipelineRegister *ex_mem;
        PipelineRegister *mem_wb;
        MemoryStore *memory;
    public:
        Memory(
                PipelineRegister *ex_mem,
                PipelineRegister *mem_wb,
                MemoryStore *memory
        ) {
            Memory::ex_mem = ex_mem;
            Memory::mem_wb = mem_wb;
            Memory::memory = memory;
        }

    Memory() {}

    void run() {
            if (ex_mem->getValue(RegisterName::Names ::MEM_READ) == 1) {
                mem_wb->setValue(
                        RegisterName::Names::MEM_RESULT,
                        memory->getValue(ex_mem->getValue(RegisterName::Names::ALU_RESULT))
                );
            }

            if (ex_mem->getValue(RegisterName::Names::MEM_WRITE) == 1) {
                memory->storeValue(
                        ex_mem->getValue(RegisterName::Names::ALU_RESULT),
                        ex_mem->getValue(RegisterName::Names::WRITE_DATA)
                );
            }

            ex_mem->forwardValues(*mem_wb);
        }
    };
class Writeback{
    PipelineRegister *mem_wb;
    RegisterFile *registerFile;
public :
    Writeback(
            PipelineRegister *mem_wb,
            RegisterFile *registerFile
    ) {
       Writeback:: mem_wb = mem_wb;
       Writeback:: registerFile = registerFile;
    }

    Writeback() {}

    bool done() {
        return mem_wb->getValue(RegisterName::Names::HALT) == 1;
    }
    bool isNop() {
        return mem_wb->getValue(RegisterName::Names::REG_DST) == 0 &&
               mem_wb->getValue(RegisterName::Names ::ALU_SRC) == 0 &&
               mem_wb->getValue(RegisterName::Names::MEM_TO_REG) == 0 &&
               mem_wb->getValue(RegisterName::Names ::REG_WRITE) == 0 &&
               mem_wb->getValue(RegisterName::Names::MEM_READ) == 0 &&
               mem_wb->getValue(RegisterName::Names::MEM_WRITE) == 0 &&
               mem_wb->getValue(RegisterName::Names::BRANCH) == 0 &&
               mem_wb->getValue(RegisterName::Names::BRANCH_NE) == 0 &&
               mem_wb->getValue(RegisterName::Names::JUMP) == 0 &&
               mem_wb->getValue(RegisterName::Names::JUMP_SRC) == 0 &&
               mem_wb->getValue(RegisterName::Names::ALU_OP) == 0;
    }


public:
    void run() {
        if (mem_wb->getValue(RegisterName::Names::REG_WRITE) == 1) {
            unsigned int dest = mem_wb->getValue(RegisterName::Names::REG_DST) == 1 ?
                        mem_wb->getValue(RegisterName::Names::REG_D) :
                        mem_wb->getValue(RegisterName::Names::REG_T);

            unsigned int value = mem_wb->getValue(RegisterName::Names::MEM_TO_REG) == 1 ?
                         mem_wb->getValue(RegisterName::Names::MEM_RESULT) :
                         mem_wb->getValue(RegisterName::Names::ALU_RESULT);

            if (dest != 0) {
                registerFile->setValue(RegisterName::valueOf(dest), value);
                registerFile->tick();
            }
        }
    }
};
class Mips {
    /**
     * The pipeline register between the fetch and decode stages
     */
private :
    PipelineRegister if_id;

    /**
     * The pipeline register between the decode and execute stages
     */
    PipelineRegister id_ex;

    /**
     * The pipeline register between the execute and memory stages
     */
    PipelineRegister ex_mem;

    /**
     * The pipeline register between the memory and writeback stages
     */
    PipelineRegister mem_wb;

    /**
     * The program counter
     */
    ProgramCounter programCounter;

    /**
     * The register file for the CPU
     */
    RegisterFile registerFile;

    /**
     * The fetch pipeline stage
     */
    Fetch fetch;

    /**
     * The decode pipeline stage
     */
    Decode decode;

    /**
     * The execute pipeline stage
     */
    Execute execute;

    /**
     * The memory pipeline stage
     */
    Memory memory;

    /**
     * The writeback pipeline stage
     */
    Writeback writeback;

    MemoryStore memoryStore;

    /**
     * Tick all the various registers of the program.  This is called
     * once per cycle.
     */
public:
    /**
        * The main launching point for the program.  This should take as input
        * a single string that is the path to the input binary file.
        *
        * @param args The path to the input file.
        */


    Mips(char filename[]){
            // create the memory store
            //memoryStore.readFile(filename);

        registerFile.setValue(RegisterName::Names::REG_17, 3);
        registerFile.registers.at(RegisterName::Names::REG_17).tick();
        registerFile.setValue(RegisterName::Names::REG_18, 4);
        registerFile.registers.at(RegisterName::Names::REG_18).tick();
        registerFile.setValue(RegisterName::Names::REG_19, 4);
        registerFile.registers.at(RegisterName::Names::REG_19).tick();
        // create the various pipeline stages
            fetch = Fetch(&if_id, &memoryStore, &programCounter);
            decode = Decode(&if_id, &id_ex, &registerFile, &programCounter);
            execute = Execute(&id_ex, &ex_mem, &mem_wb);
            memory = Memory(&ex_mem, &mem_wb, &memoryStore);
            writeback = Writeback(&mem_wb, &registerFile);
    }

/**
 * Runs the MIPS simulator and prints the results out after completion.
 */
 void run() {
    // keep track of the instructions run and the cycles run
    int instructionCount = 0;
    int cycleCount = 0;

    // while we haven't halted
    while(!writeback.done()) {
        // run all the pipeline stages
        fetch.run();
        // note: writeback has to run before decode to
        // avoid data hazards
        writeback.run();
        decode.run();
        execute.run();
        memory.run();

        // if we completed a non-stall instruction, increment
        // the instruction counter
        if (!writeback.isNop()) {
            instructionCount++;
        }

        // increment the cycle counter
        cycleCount++;

        // tick over all our register values
        tick();
    }
        cout << registerFile.registers.at(RegisterName::Names::REG_2).getValue() << endl;
        cout << cycleCount;
}

private:
    void tick() {
        programCounter.tick();
        if_id.tick();
        id_ex.tick();
        ex_mem.tick();
        mem_wb.tick();
    }

};




int main() {
    char filename[50] = "test.txt";
    //cin >> filename;

    try {
        Mips mips(filename);
        mips.run();
    }
    catch(char const* ex) {
        cout << ex;
    }
    return 0;
}