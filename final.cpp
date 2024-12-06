#include<bits/stdc++.h>
#include<bitset>
using namespace std;

unordered_map<string,char> get_type;
vector<string>machinecode;


unordered_map<string, pair<string, string>> func_map = {
    {"ADD", {"000", "0000000"}},
    {"SUB", {"000", "0100000"}},
    {"AND", {"111", "0000000"}},
    {"OR",  {"110", "0000000"}},
    {"XOR", {"100", "0000000"}},
    {"SLL", {"001", "0000000"}},
    {"SRL", {"101", "0000000"}},
    {"SRA", {"101", "0100000"}},
    {"SLT", {"010", "0000000"}},
    {"SLTU", {"011", "0000000"}}
};


unordered_map<string, string> func_mapI = {
    {"ADDI",  "000"},
    {"SLTI",  "010"},  
    {"SLTIU", "011"},  
    {"ANDI",  "111"}, 
    {"ORI",   "110"},
    {"XORI",  "100"},
    {"SLLI",  "001"},
    {"SRLI",  "101"},
    {"SRAI",  "101"},  
    {"LB",    "000"},
    {"LH",    "001"},  
    {"LW",    "010"}, 
    {"LBU",   "100"},
    {"LHU",   "101"},  
     {"JALR",   "000"} 
};

// S-type instruction mapping
unordered_map<string, string> func_mapS = {
    {"SB",  "000"},  
    {"SH",  "001"},  
    {"SW",  "010"},  
    {"SD",  "011"}
};

// U-type instruction mapping
unordered_map<string, string> func_mapU = {
    {"LUI",   "0110111"},   
    {"AUIPC", "0010111"}    
};


// B-type instruction mapping
unordered_map<string, string> func_mapB = {
    {"BEQ",  "000"},
    {"BNE",  "001"},
    {"BLT",  "100"},
    {"BGE",  "101"},
    {"BLTU", "110"},
    {"BGEU", "111"}
};



class Rtype{
    string opcode="0110011";
    string rs1="";
    string rs2="";
    string rd="";
    string func3="";
    string func7="";
public:
    Rtype(const vector<string>&operands, const string &operation)
    {
        if(operands.size()<3)
        {
            cout<<"invalid operands for r-type"<<endl;
            return;
        }
        try{
            rd=bitset<5>(stoi(operands[0].substr(1))).to_string();
            rs1=bitset<5>(stoi(operands[1].substr(1))).to_string();
            rs2=bitset<5>(stoi(operands[2].substr(1))).to_string();

            if (func_map.find(operation) != func_map.end()) {
                func3 = func_map[operation].first;
                func7 = func_map[operation].second;
            } else {
                cout << "Unknown R-type operation: " << operation << endl;
                return;
            }
        } catch (const invalid_argument& e)
        {
            cout<<"invlaid register number in operands"<<endl;
        }
    }
    void display()
    {
        string ins;
        ins=func7+rs2+rs1+func3+rd+opcode;
        machinecode.push_back(ins);
    }
};

class Itype{
    string opcode="0010011";
    string rs1="";
    string rd="";
    string func3="";
    string imm="";
public:
    Itype(const vector<string>&operands, const string &operation)
    {
        if(operands.size()<3)
        {
            cout<<"invalid operands for i-type"<<endl;
            return;
        }
        try{
            rd=bitset<5>(stoi(operands[0].substr(1))).to_string();
            if(operation=="LB"||operation=="LH"||operation=="LW"||operation=="LBU"||operation=="LHU")
            {
                rs1=bitset<5>(stoi(operands[2].substr(1))).to_string();
                imm=bitset<12>(stoi(operands[1])).to_string(); 
            }
            else{
                 rs1=bitset<5>(stoi(operands[1].substr(1))).to_string();
                 imm=bitset<12>(stoi(operands[2])).to_string();
            }
          
           
            if(operation=="SLLI"||operation=="SRLI"||operation=="SRAI")
            {
                  
               int imm_value = stoi(operands[2]);

                if (imm_value > 31)
                {
                    cout << "Shift amount should be less than  32." << endl;
                    exit(0);
                }
                if(operation=="SRAI") imm[1]='1';
            }
            if (func_mapI.find(operation) != func_mapI.end()) {
                func3 = func_mapI[operation];
            } else {
                cout << "Unknown I-type operation: " << operation << endl;
                return;
            }
           if(operation=="LB"||operation=="LH"||operation=="LW"||operation=="LBU"||operation=="LHU") opcode="0000011";
           if(operation=="JALR")
           {
             opcode="1100111";
           } 

        } catch (const invalid_argument& e)
        {
            cout<<"invlaid register number in operands"<<endl;
        }
    }
    void display()
    {
        string ins;
        ins=imm+rs1+func3+rd+opcode;
        machinecode.push_back(ins);
    }
};

class Stype{
    string opcode="0100011";
    string rs1="";
    string rs2="";
    string imm="";
    string func3="";
public:
    Stype(const vector<string>&operands, const string &operation)
    {
        if(operands.size()<3)
        {
            cout<<"invalid operands for s-type"<<endl;
            return;
        }
        try{
            rs2=bitset<5>(stoi(operands[0].substr(1))).to_string();
            rs1=bitset<5>(stoi(operands[2].substr(1))).to_string();
            imm=bitset<12>(stoi(operands[1])).to_string();

            if (func_mapS.find(operation) != func_mapS.end()) {
                func3 = func_mapS[operation];
            } else {
                cout << "Unknown S-type operation: " << operation << endl;
                return;
            }
        } catch (const invalid_argument& e)
        {
            cout<<"invlaid register number in operands"<<endl;
        }
    }
    void display()
    {
        string ins;
        ins=imm.substr(0,7)+rs2+rs1+func3+imm.substr(7,5)+opcode;
        machinecode.push_back(ins);
    }
};

class Btype{
    string opcode="1100011";
    string rs1="";
    string rs2="";
    string imm="";
    string func3="000";
public:
    Btype(const vector<string>&operands, const string &operation)
    {
        if(operands.size()<3)
        {
            cout<<"invalid operands for b-type"<<endl;
            return;
        }
        try{
            rs1=bitset<5>(stoi(operands[0].substr(1))).to_string();
            rs2=bitset<5>(stoi(operands[1].substr(1))).to_string();
            imm=bitset<12>(stoi(operands[2])).to_string();

             if (func_mapB.find(operation) != func_mapB.end()) {
                func3 = func_mapB[operation];
            } else {
                cout << "Unknown B-type operation: " << operation << endl;
                return;
            }
        } catch (const invalid_argument& e)
        {
            cout<<"invlaid register number in operands"<<endl;
        }
    }
    void display()
    {
        string ins;
        ins=imm[0]+imm.substr(2,6)+rs2+rs1+func3+imm[1]+imm.substr(8,4)+opcode;
        machinecode.push_back(ins);
    }
};

class Utype{
    string opcode="";
    string rd="";
    string imm="";
public:
    Utype(const vector<string>&operands, const string &operation)
    {
        if(operands.size()<2)
        {
            cout<<"invalid operands for u-type"<<endl;
            return;
        }
        try{
            rd=bitset<5>(stoi(operands[0].substr(1))).to_string();
            imm=bitset<20>(stoi(operands[1])).to_string();
             if (func_mapU.find(operation) != func_mapU.end()) {
              opcode = func_mapU[operation];
            } else {
                cout << "Unknown U-type operation: " << operation << endl;
                return;
            }
        } catch (const invalid_argument& e)
        {
            cout<<"invlaid register number in operands"<<endl;
        }
    }
    void display()
    {
        string ins;
        ins=imm+rd+opcode;
        machinecode.push_back(ins);
    }
};

class Jtype{
    string opcode="1101111";
    string rd="";
    string imm="";
public:
    Jtype(const vector<string>&operands)
    {
        if(operands.size()<2)
        {
            cout<<"invalid operands for j-type"<<endl;
            return;
        }
        try{
            rd=bitset<5>(stoi(operands[0].substr(1))).to_string();
            imm=bitset<20>(stoi(operands[1])).to_string();
        } catch (const invalid_argument& e)
        {
            cout<<"invlaid register number in operands"<<endl;
        }
    }
    void display()
    {
        string ins;
        ins=imm[0]+imm.substr(10,10)+imm[9]+imm.substr(1,8)+rd+opcode;
        machinecode.push_back(ins);
    }
};


class Parser{
private:
    vector<string> operations;
    vector<vector<string>> operands;
public:
    Parser(const string & instruction)
    {
        parseInstruction(instruction);
    }
   void parseInstruction(const string &instruction)
{
    string temp = "";
    bool isoperation = true;
    vector<string> currentoperands;

    for (size_t i = 0; i < instruction.size(); i++)
    {
        char p = instruction[i];

        if (p == '\\' && i + 1 < instruction.size() && instruction[i + 1] == 'n')
        {
            if (!temp.empty())
            {
                currentoperands.push_back(temp);
                temp = "";
            }
            operands.push_back(currentoperands);
            currentoperands.clear();
            isoperation = true;
            i++;
        }
        else if (p == ' ' && isoperation && temp.size())
        {
            operations.push_back(temp);
            temp = "";
            isoperation = false;
        }
        else if (p == ',')
        {
            if (temp.size())
            {
                currentoperands.push_back(temp);
                temp = "";
            }
        }
        else if (p == '[') // Start of indexed addressing
        {
            if (!temp.empty()) // push the base operand, e.g., r1
            {
                currentoperands.push_back(temp);
                temp = "";
            }
            string index_operand = "";
            i++; // skip '['
            while (i < instruction.size() && instruction[i] != ']')
            {
                index_operand += instruction[i];
                i++;
            }
            currentoperands.push_back(index_operand); // push index part, e.g., r2
        }
        else
        {
            if (p == ' ') continue;
            temp += p;
        }
    }
    if (temp.size())
    {
        currentoperands.push_back(temp);
    }
    if (currentoperands.size())
    {
        operands.push_back(currentoperands);
    }
}

    
    string getParsedInstruction()
    {
        string output;
        
        for(size_t i=0;i<operations.size();i++)
        {
            string result=operations[i];
            for(auto op:operands[i])
            {
                result+="|"+op;
            }
            output+= result+"\n";
        }
        
        return output;
    }
    
    void processInstructions()
    {
        for(size_t i=0;i<operations.size();i++)
        {
            string operation=operations[i];
            if(get_type.find(operation)!=get_type.end())
            {
                char type=get_type[operation];
                switch (type)
                {
                    case 'R':{
                        Rtype rtype(operands[i],operation);
                        rtype.display();
                        cout<<endl;
                        break;
                    }
                    case 'I':{
                        Itype itype(operands[i],operation);
                        itype.display();
                        cout<<endl;
                        break;
                    }
                    case 'B':{
                        Btype btype(operands[i],operation);
                        btype.display();
                        cout<<endl;
                        break;
                    }
                     case 'U':{
                        Utype utype(operands[i],operation);
                        utype.display();
                        cout<<endl;
                        break;
                    }
                    case 'J':{
                        Jtype jtype(operands[i]);
                        jtype.display();
                        cout<<endl;
                        break;
                    }
                    case 'S':{
                        Stype stype(operands[i],operation);
                        stype.display();
                        cout<<endl;
                        break;
                    }
                    default:
                      cout<<"unknown type for instruction:"<<operation<<endl;
                }
            }
            else
            {
                cout<<"unknown instruction:"<<operation<<endl;
            }
        }
    }
    
};



void initializeGetType() {
    // R-type instructions
    get_type["ADD"] = 'R';
    get_type["SUB"] = 'R';
    get_type["AND"] = 'R';
    get_type["OR"] = 'R';
    get_type["XOR"] = 'R';
    get_type["SLL"] = 'R';
    get_type["SRL"] = 'R';
    get_type["SRA"] = 'R';
    get_type["SLT"] = 'R';
    get_type["SLTU"] = 'R';

    // I-type instructions
    get_type["ADDI"] = 'I';
    get_type["SLTI"] = 'I';
    get_type["SLTIU"] = 'I';
    get_type["ANDI"] = 'I';
    get_type["ORI"] = 'I';
    get_type["XORI"] = 'I';
    get_type["SLLI"] = 'I';
    get_type["SRLI"] = 'I';
    get_type["SRAI"] = 'I';
    get_type["LB"] = 'I';
    get_type["LH"] = 'I';
    get_type["LW"] = 'I';
    get_type["LBU"] = 'I';
    get_type["LHU"] = 'I';
    get_type["JALR"] = 'I';

    // S-type instructions
    get_type["SB"] = 'S';
    get_type["SH"] = 'S';
    get_type["SW"] = 'S';
    get_type["SD"] = 'S';

    // B-type instructions
    get_type["BEQ"] = 'B';
    get_type["BNE"] = 'B';
    get_type["BLT"] = 'B';
    get_type["BGE"] = 'B';
    get_type["BLTU"] = 'B';
    get_type["BGEU"] = 'B';

    // U-type instructions
    get_type["LUI"] = 'U';
    get_type["AUIPC"] = 'U';

    // J-type instructions
    get_type["JAL"] = 'J';
}

struct CW
{
    bool RegRead;
    bool AluSrc;
    bool RegWrite;
    bool MemRead;
    bool MemWrite;
    bool Mem2Reg;
    bool Branch;
    bool Jump;
    int AluOp;

    // Default constructor
    CW() : RegRead(false), AluSrc(false), RegWrite(false), MemRead(false), MemWrite(false), Mem2Reg(false), Branch(false), Jump(false), AluOp(0)
    {
    }

    // Parameterized constructor
    CW(bool aluSrc, bool regRead, bool regWrite, int aluOp, bool mem2Reg, bool memRead, bool memWrite, bool branch, bool jump)
        : AluSrc(aluSrc), RegRead(regRead), RegWrite(regWrite), Mem2Reg(mem2Reg), MemRead(memRead), MemWrite(memWrite), Branch(branch), Jump(jump), AluOp(aluOp)
    {
    }
};

struct MOWB
{
    CW cw;
    int LDOUT;
    int ALLOUT;
    int rdl;
    MOWB() : rdl(-1)
    {
    }
};

struct EXMO
{
    CW cw;
    int rs2;
    int ALLOUT;
    int rdl;
    EXMO() : rdl(-1)
    {
    }
};

struct IDEX
{
    CW cw;
    int JPC;
    int DPC;
    int imm;
    int rs1;
    int rs2;
    int rsl1;
    int rsl2;
    int rdl;
    int func;
    IDEX() : rdl(-1)
    {
    }
};

struct IFID
{
    string IR;
    int DPC;
    IFID() : IR("") {}
};

class CPU
{
public:
    vector<int> GPR;
    vector<int> memory;
    vector<string> instructionSet;
    int NPC, PC, wait;
    EXMO exmo;
    IDEX idex;
    IFID ifid;
    MOWB mowb;
    MOWB prevmowb;
    vector<int> state;
    unordered_set<int> usingReg;

    CPU(vector<string> instructionSet)
    {
        GPR.resize(32, 0);
        memory.resize(20, 0);
        state.resize(5, 0);
        this->instructionSet = instructionSet;
        PC = 0;
        NPC = 0;
        wait = 0;
    }

private:
    int extractBits(int number, int start, int end)
    {
        int mask = ((1 << (start - end + 1)) - 1) << end;
        return (number & mask) >> end;
    }

    int binaryStrToInt(string &binaryStr)
    {
        return stoi(binaryStr, nullptr, 2);
    }

    string numToBinary(const string &num, int size)
    {
        if (size == 4)
            return bitset<12>(stoi(num)).to_string();
        else
            return bitset<20>(stoi(num)).to_string();
    }

    int signExtend(int imm, int bitLength)
    {
        int mask = 1 << (bitLength - 1);
        return (imm ^ mask) - mask;
    }

    int immGen(string I, string format)
    {
        string imm;
        int bitLength = 0;

        if (format == "I" || format == "L")
        {
            imm = I.substr(0, 12); // 12-bit immediate
            bitLength = 12;
        }
        else if (format == "S")
        {
            imm = I.substr(0, 7) + I.substr(20, 5); // S-type 12-bit immediate
            bitLength = 12;
        }
        else if (format == "B")
        {
            imm = string(1, I[0]) + string(1, I[24]) + I.substr(1, 6) + I.substr(20, 4); // B-type 12-bit immediate
            bitLength = 12;
        }
        else if (format == "U")
        {
            imm = I.substr(0, 20); // U-type 20-bit immediate
            return binaryStrToInt(imm) << 12;
        }
        else if (format == "J")
        {
            imm = string(1, I[0]) + I.substr(12, 8) + string(1, I[11]) + I.substr(1, 10); // J-type 21-bit immediate
            bitLength = 21;
        }

        int immValue = binaryStrToInt(imm);
        return signExtend(immValue, bitLength);
    }

    pair<CW, string> controller(string opcode)
    {
        bool aluSrc = 0, regRead = 1, regWrite = 0;
        int aluOP = 0;
        bool mem2Reg = 0, memRead = 0, memWrite = 0, branch = 0, jump = 0;
        string format;
        if (opcode == "0110011") // R-type
        {
            regWrite = 1;
            aluOP = 2;
            format = "R";
            // return {CW(false, true, true, 2, false, false, false, false, false), "R"};
        }
        else if (opcode == "0010011") // I-Type
        {
            aluSrc = 1;
            regWrite = 1;
            aluOP = 2;
            format = "I";
            // return {CW(true, true, true, 2, false, false, false, false, false), "I"};
        }
        else if (opcode == "0000011") // L-type
        {
            aluSrc = 1;
            regWrite = 1;
            mem2Reg = 1;
            memRead = 1;
            format = "L";
            // return {CW(true, true, true, 0, true, true, false, false, false), "L"};
        }
        else if (opcode == "0100011") // S-type
        {
            aluSrc = 1;
            mem2Reg = 1;
            memWrite = 1;
            format = "S";
            // return {CW(true, true, false, 0, true, false, true, false, false), "S"};
        }
        else if (opcode == "1100011") // B-type
        {
            aluOP = 1;
            branch = 1;
            format = "B";
            // return {CW(false, true, false, 1, false, false, false, true, false), "B"};
        }
        else
        {
            cout << "error in CW" << endl;
        }
        return {CW(aluSrc, regRead, regWrite, aluOP, mem2Reg, memRead, memWrite, branch, jump), format};
    }

 int forwordingUnit(int rsl, int rs)
    {
        int res = 0, input;
        if (exmo.rdl == rsl)
        {
            if (exmo.cw.MemRead)
            {
                res = 1;
            }
            else if (!exmo.cw.MemRead && !exmo.cw.MemWrite && exmo.cw.RegWrite)
            {
                res = 2;
            }
        }
        else if (prevmowb.rdl == rsl && prevmowb.cw.RegWrite)
        {
            res = 3;
        }
        switch (res)
        {
        case 0:
            input = rs;
            break;
        case 1:
            input = mowb.LDOUT; // memory to register
            break;
        case 2:
            input = mowb.ALLOUT;
            break;
        case 3:
            input = GPR[rsl];
            break;
        default:
            cout << "error in forwarding unit" << endl;
            break;
        }
        return input;
    }

    int ALUControl(int ALUop, int func)
    {
        // changed slt -->mux, sltu-->mod
        map<string, int> Aluaction = {{"and", 0b0000}, {"or", 0b0001}, {"add", 0b0010}, {"sub", 0b0110}, {"xor", 0b0011}, {"sll", 0b0100}, {"srl", 0b0101}, {"sra", 0b0111}, {"mux", 0b1000}, {"mod", 0b1001}};
        map<int, string> funcToOp = {{0b000, "add"}, {0b1000, "sub"}, {0b001, "sll"}, {0b010, "mux"}, {0b011, "mod"}, {0b100, "xor"}, {0b101, "srl"}, {0b110, "or"}, {0b111, "and"}, {0b1101, "sra"}};
        switch (ALUop)
        {
        case 0: // load, store
            return Aluaction["add"];
            break;
        case 1: // branch
            return Aluaction["sub"];
            break;
        case 2: // r-type ,I-type
            return Aluaction[funcToOp[func]];
            break;
        case 3:
            cout << "Aluop error" << endl;
            break;
        }
    }

    int ALU(int Aluselect, int rs1, int rs2)
    {
        switch (Aluselect)
        {
        case 0:
            return rs1 & rs2;
            break;
        case 1:
            return rs1 | rs2;
            break;
        case 2:
            return rs1 + rs2;
            break;
        case 3:
            return rs1 ^ rs2;
            break;
        case 4:
            return rs1 << rs2;
            break;
        case 5:
            return rs1 >> rs2;
            break;
        case 6:
            return rs1 - rs2;
            break;
        case 8:
            return rs1 * rs2;
            break;
        case 9:
            return rs1 % rs2;
            break;
        default:
            cout << "alu error" << endl;
            break;
        }
    }

    void fetch()
    {
        cout << "Fetch:" << endl;
        if (state[0] == 0 || PC >= instructionSet.size())
        {
            state[1] = 0;
            return;
        }
        else
        {
            string instruction = instructionSet[PC];
            ifid.IR = instruction;
            state[1] = 1;
        }
        ifid.DPC = PC;
        NPC = PC + 1;
        // if (NPC == instructionSet.size())
        // {
        //     clocks += 4;
        // }
        // else
        // {
        //     clocks++;
        // }
        // Debug output for Stage 1
        cout << " IR = " << ifid.IR << endl;
        cout << " DPC = " << ifid.DPC << endl;
        cout << endl;
        return;
    }

    void decode()
    {
        cout << "Decode:" << endl;
        if (state[1] == 0)
        {
            state[2] = 0;
            return;
        }
        state[2] = 1;
        idex.DPC = ifid.DPC;
        string opcode = ifid.IR.substr(25, 7);
        string format;
        // control unit
        auto res = controller(opcode);
        idex.cw = res.first;
        format = res.second;
        cout << " Opcode = " << opcode << endl;
        cout << " Format = " << format << endl;

        // imm gen
        if (format != "R")
            idex.imm = immGen(ifid.IR, format);

        // func
        string funct7, funct3;
        if (format == "R")
        {
            funct7 = ifid.IR.substr(0, 7);
            funct3 = ifid.IR.substr(17, 3);
            string res = string(1, funct7[1]) + funct3;
            idex.func = binaryStrToInt(res);
        }
        else
        {
            funct3 = ifid.IR.substr(17, 3);
            idex.func = binaryStrToInt(funct3);
        }

        if (idex.cw.RegRead)
        {
            string strReg = ifid.IR.substr(12, 5);
            idex.rsl1 = binaryStrToInt(strReg);
            idex.rs1 = GPR[idex.rsl1];
        }
        cout << " rsl1 = " << idex.rsl1 << endl;
        cout << " rs1 = " << idex.rs1 << endl;

        string strrs2 = ifid.IR.substr(7, 5);
        idex.rsl2 = binaryStrToInt(strrs2);
        idex.rs2 = GPR[idex.rsl2];
        cout << " rsl2 = " << idex.rsl2 << endl;
        cout << " rs2 = " << idex.rs2 << endl;

        string rdl = ifid.IR.substr(20, 5);
        idex.rdl = binaryStrToInt(rdl);
        cout << " rdl = " << idex.rdl << endl;

        // Debug output for Stage 2
        cout << " imm = " << idex.imm << endl;
        cout << " func = " << idex.func << endl;
        return;
    }

    void execute()
    {
        if (state[2] == 0)
        {
            state[3] = 0;
            PC = NPC;
            return;
        }
        state[3] = 1;
        int input1, input2;
        if (idex.cw.RegRead)
        {
            input1 = forwordingUnit(idex.rsl1, idex.rs1);
        }

        if (idex.cw.AluSrc)
        {
            input2 = idex.imm;
        }
        else
        {
            input2 = forwordingUnit(idex.rsl2, idex.rs2);
        }

        if (idex.cw.MemWrite)
            exmo.rs2 = forwordingUnit(idex.rsl2, idex.rs2);

        int Aluselect = ALUControl(idex.cw.AluOp, idex.func);

        // ALU
        exmo.ALLOUT = ALU(Aluselect, input1, input2);
        exmo.cw = idex.cw;
        exmo.rdl = idex.rdl;

        // branch type
        bool branchTaken = false;
        if (idex.cw.Branch)
        {
            int value = exmo.ALLOUT;
            switch (idex.func)
            {
            case 0b000: // BEQ (Branch if Equal)
                branchTaken = value == 0 ? 1 : 0;
                break;
            case 0b001: // BNQ (Branch if Not Equal)
                branchTaken = value != 0 ? 1 : 0;
                break;
            case 0b101: // BGE (Branch if Greater or Equal)
                branchTaken = value >= 0 ? 1 : 0;
                break;
            case 0b100: // BLT (Branch if Less Than)
                branchTaken = value < 0 ? 1 : 0;
                break;

            default:
                cout << "Unknown branch function code: " << idex.func << endl;
                break;
            }

            if (branchTaken)
            {
                state[1] = 0;
                state[0] = 0;
                wait = 1;
                NPC = idex.imm + idex.DPC;
            }
            else
            {
                PC = NPC;
            }
        }
        else
        {
            PC = NPC;
        }

        if (idex.cw.Jump)
            PC = idex.JPC;

        return;
    }

    void memoryOperation()
    {
        cout << "Memory Operation:" << endl;
        prevmowb = mowb;
        if (state[3] == 0)
        {
            state[4] = 0;
            return;
        }
        state[4] = 1;
        if (exmo.cw.MemRead)
        {
            mowb.LDOUT = memory[exmo.ALLOUT];
        }
        if (exmo.cw.MemWrite)
        {
            memory[exmo.ALLOUT] = exmo.rs2;
        }
        mowb.ALLOUT = exmo.ALLOUT;
        mowb.cw = exmo.cw;
        mowb.rdl = exmo.rdl;
        // Debug output for Stage 4

        cout << " LDOUT = " << mowb.LDOUT << endl;
        cout << " ALLOUT = " << mowb.ALLOUT << endl;
        return;
    }

    void writeBack()
    {
        cout << "Write Back:" << endl;
        if (state[4] == 0)
            return;
        if (mowb.cw.RegWrite)
            if (mowb.cw.Mem2Reg)
                GPR[mowb.rdl] = mowb.LDOUT;
            else
                GPR[mowb.rdl] = mowb.ALLOUT;

        // Debug output for Stage 5

        cout << " GPR[" << mowb.rdl << "] = " << GPR[mowb.rdl] << endl;
        return;
    }

public:
    int processing()
    {
        int clocks = 0;
        state[0] = 1; // state=1 (valid) state=0 (invalid)
        while (state[0] == 1 || state[1] == 1 || state[2] == 1 || state[3] == 1 || state[4] == 1)
        {
            state[0] = 1;
            writeBack();
            memoryOperation();
            execute();
            decode();
            fetch();
            state[0] = 0;
            clocks++;
        }
        // Print final GPR state
        cout << "Final GPR State: ";
        for (int i = 0; i < GPR.size(); i++)
        {
            cout << GPR[i] << " ";
        }
        cout << endl;
        cout << "Final Memory Values: ";
        for (int i = 0; i < memory.size(); i++)
        {
            cout << memory[i] << " ";
        }
        cout << endl;
        cout << "clocks=" << clocks << endl;
        return 0;
    }
};

int main() {
    // Initialize the get_type map
    initializeGetType();

    int n;
    cout << "Enter the number of instructions: ";
    cin >> n;
    cin.ignore(); // To ignore the newline character after the number

    vector<string> instructions(n);

    // Input 'n' instructions
    for (int i = 0; i < n; i++) {
        cout << "Enter instruction " << (i + 1) << ": ";
        getline(cin, instructions[i]);
    }

    // Process each instruction
    for (int i = 0; i < n; i++) {
        cout << "\nProcessing instruction " << (i + 1) << ": " << instructions[i] << endl;
        
        Parser instructionSet(instructions[i]);
        string parsedcontent = instructionSet.getParsedInstruction();
        cout << "Parsed instruction: " << parsedcontent << endl;
        
        instructionSet.processInstructions();
    }
    cout << "Binary output: " << endl;
        for (auto code : machinecode) {
            cout << code << endl;
        }
    CPU cpu(machinecode);
    cpu.processing();
    return 0;
}



