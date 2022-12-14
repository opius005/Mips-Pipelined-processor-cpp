#include <bits/stdc++.h>
using namespace std;


class instuctionmemory
{
public:
    bitset<32> instruction;

    instuctionmemory()
    {
        INSMEMFILE.resize(1000);
        string line;
        int i = 0;
        ifstream im("instruction_memory.txt");
        if (im.is_open())
        {
            while (getline(im, line))
            {
                INSMEMFILE[i] = bitset<8>(line);
                i++;
            }
        }
        else
            cout << "Unable to open file";
        im.close();
    }

    bitset<32> Readmemory(bitset<32> instructionAdress)
    {
        unsigned long tempinstradress = instructionAdress.to_ulong();
        instruction = 0;
        string s = "";
        for (int i = 0; i < 4; i++)
        {
            s += INSMEMFILE[tempinstradress + i].to_string();
        }

        bitset<32> temp(s);

        instruction = temp;

        return instruction;

    }

protected:
    vector<bitset<8>> INSMEMFILE;
};

class IF_ID{
public:
    bitset<32> instr;
    bool no_operation;

    IF_ID(){
       no_operation=1;
    }
};

class registerfileunit
{
public:
    

    registerfileunit()
    {
        registers.resize(32);
        registers[0] = bitset<32> (0);
    }
    bitset<32> readdata1, readdata2;

    void readregister(bitset<5> rs, bitset<5> rt, bitset<5> rd, bitset<32> writedata, int enable)
    {
        if (enable == 0)
        {
            readdata1 = registers[rs.to_ulong()];
            readdata2 = registers[rt.to_ulong()];
        }
        else
        {

            registers[rd.to_ulong()] = writedata.to_ulong();
        }
    }

    void registerfileoutput()
    {
        ofstream rfout("FinalRegistersValue.txt");
        if (rfout.is_open())
        {
            
            for (int j = 0; j < 32; j++)
            {
                rfout << registers[j] << endl;
            }
        }
        else
            cout << "Unable to open file"
                 << "FinalRegistersValue.txt" << endl;
        rfout.close();
    }

protected:
    vector<bitset<32>> registers;
};

class ID_EX{
public:
    bitset<32>  Read_data1;
    bitset<32>  Read_data2;
    bitset<32>  Storedata;
    bitset<16>  Imm;
    bitset<5>   Rs;
    bitset<5>   Rt;
    bitset<5>   Rd;
    bitset<6>   oc;
   // bool        is_I_type;
    bool        rd_mem;
    bool        wrt_mem; 
    bitset<6>    funct;     //1 for addu, lw, sw, 0 for subu 
    bool        wrt_enable;
    bool no_operation;
    string      INS;  //instruction type

    ID_EX(){
        no_operation=1;
    }
};

class ALU
{

public:
    bitset<32> OutputOfAlu ;

    bitset<32> ALUoperation(bitset<6> function, bitset<32> aluinput1, bitset<32> aluinput2)
    {
        if (function == 32)
        { // addition
            OutputOfAlu  = aluinput1.to_ulong() + aluinput2.to_ulong();
        }

        if (function == 2)
        { //mul
            OutputOfAlu  = (aluinput1.to_ulong()) * (aluinput2.to_ulong());
        }

        else if (function == 34)
        { // subtraction
            OutputOfAlu  = aluinput2.to_ulong() - aluinput1.to_ulong();
        }

        else if (function == 36)
        { // and
            OutputOfAlu  = aluinput1 & aluinput2;
        }

        else if (function == 37)
        { // or
            OutputOfAlu  = aluinput1 | aluinput2;
        }

        else if (function == 7)
        { // nor
            OutputOfAlu  = ~(aluinput1 | aluinput2);
        }
        return OutputOfAlu ;
    }
};


class EX_MEM{
public:
    bitset<32>  ALUresult;
    bitset<32>  Storedata;
    bitset<5>   Rs;
    bitset<5>   Rt;    
    bitset<5>   Rd;
    bool        rd_mem;
    bool        wrt_mem; 
    bool        wrt_enable;    
    bool no_operation;   
    string      INS;

    EX_MEM(){
        no_operation=1;
    }

};

class DataMemoryUnit
{
public:
    bitset<32> readdatainmem;

    DataMemoryUnit()
    {
        Dmem.resize(1000);
        string line;
        int i=1;
        ifstream dm("DataMemoryInput.txt");
        if (dm.is_open())
        {
            while (getline(dm, line))
            {
                
                Dmem[i]=bitset<32>(line);
                i++;
            }
        }
        else
            cout << "Cannot Open the File "
                 << "DataMemoryInput.txt" << endl;
        dm.close();
    }

    void MemoryAccess(bitset<32> address, bitset<32> writedata, bitset<1> readmem, bitset<1> writemem)
    {
        readdatainmem = 0;
        if (readmem == 1)
        {
            readdatainmem = Dmem[address.to_ulong()];
        }
        if (writemem == 1)
        {
            Dmem[address.to_ulong()] = writedata;
        }
    }

    void OutputDataMem()
    {
        ofstream dmemout("Datamemresult.txt");
        if (dmemout.is_open())
        {
            for (int j = 1; j < 1000; j++)
            {
                dmemout << Dmem[j] << endl;
            }
        }
        else
            cout << "Cannot open the File";
        dmemout.close();
    }

protected:
    vector<bitset<32>> Dmem;
};


class MEM_WB{
public:
    bitset<32>  Wrt_data;
    bitset<5>   Rs;
    bitset<5>   Rt;     
    bitset<5>   Rd;
    bool        wrt_enable;
    bool        no_operation;
    
    string      INS;
    MEM_WB(){
        no_operation=1;
    }
    
    

};

class cycle :IF_ID,ID_EX,EX_MEM,MEM_WB{
public:
    IF_ID ifid;
    ID_EX idex;
    EX_MEM exmem;
    MEM_WB memwb;

};


/*class fetchins{
public:
    bitset<32>programcounter;
    bool stop;

    fetchins(){
        stop=0;
        programcounter=0;
    }

}*/

bitset<6> getopcode(bitset<32> ins)
{
    string opcodestr = ins.to_string();
    opcodestr = opcodestr.substr(0, 6);

    return bitset<6>(opcodestr);
}

bitset<5> getrs(bitset<32> curinstruction)
{
    string rsstr = curinstruction.to_string();
    rsstr = rsstr.substr(6, 5);

    return bitset<5>(rsstr);
}

bitset<5> getrt(bitset<32> curinstruction)
{
    string rtstr = curinstruction.to_string();
    rtstr = rtstr.substr(11, 5);

    return bitset<5>(rtstr);
}

bitset<5> getrd(bitset<32> curinstruction)
{
    string rdstr = curinstruction.to_string();
    rdstr = rdstr.substr(16, 5);

    return bitset<5>(rdstr);
}

bitset<6> getfunc(bitset<32> curinstruction)
{
    string funcstr = curinstruction.to_string();
    funcstr = funcstr.substr(26, 6);

    return bitset<6>(funcstr);
}

bitset<26> getjumpaddress(bitset<32> curinstruction)
{
    string jumpaddressstr = curinstruction.to_string();
    jumpaddressstr = jumpaddressstr.substr(6, 26);

    return bitset<26>(jumpaddressstr);
}

bitset<16> getoffset(bitset<32> curinstruction)
{
    string offsetstr = curinstruction.to_string();
    offsetstr = offsetstr.substr(16, 16);

    return bitset<16>(offsetstr);
}

bitset<32> signExtend(bitset<16> Imm)
{
    int j;
    bitset<32> temp;
    for (j = 0; j < 16; j++)
    {
        temp[j] = Imm[j];
        temp[j + 16] = Imm[15];
    }
    return temp;
}

int main(){

    instuctionmemory i;
    registerfileunit rf;
    ALU aluobject;
    DataMemoryUnit mydatamem;
    cycle presentcycle,nextcycle;
    bitset<32>programcounter(0);
    bool stop=0;
    bitset<32>instruction;
    bitset<6> opcode;
    bitset<5> rs, rt, rd, shamt;
    bitset<6> func;
    bitset<16> imm;
    bitset<6> aluop;
    bitset<16> offset;
    bitset<32> bit32offset;
    bitset<32> aluresult;
    bitset<26> jumpaddress;
    bitset<32> halt("11111111111111111111111111111111");




    while (true)
    {


        //**Write Back stage**//
        if(presentcycle.memwb.no_operation == 0){
            if(presentcycle.memwb.wrt_enable == 1){
                rf.readregister(presentcycle.memwb.Rs,presentcycle.memwb.Rt,presentcycle.memwb.Rd,presentcycle.memwb.Wrt_data,1);
            }
        }



        //***Data Memory stage******//
        if(presentcycle.exmem.no_operation == 0){
            mydatamem.MemoryAccess(presentcycle.exmem.ALUresult, presentcycle.exmem.Storedata, presentcycle.exmem.rd_mem, presentcycle.exmem.wrt_mem);
            if(presentcycle.exmem.rd_mem == 1){//for lw
                nextcycle.memwb.Wrt_data = mydatamem.readdatainmem;
            }
            else if(presentcycle.exmem.wrt_enable == 1){//for lw and R type
                nextcycle.memwb.Wrt_data = presentcycle.exmem.ALUresult;
            }
            nextcycle.memwb.wrt_enable = presentcycle.exmem.wrt_enable;
            nextcycle.memwb.Rd = presentcycle.exmem.Rd;
            nextcycle.memwb.Rs = presentcycle.exmem.Rs;
            nextcycle.memwb.Rt = presentcycle.exmem.Rt;


        }
        nextcycle.memwb.no_operation = presentcycle.exmem.no_operation;
        if(presentcycle.exmem.no_operation == 0){
            if(nextcycle.memwb.Rd == presentcycle.idex.Rs || nextcycle.memwb.Rd == presentcycle.idex.Rt){
                nextcycle.exmem.no_operation = presentcycle.idex.no_operation;
                nextcycle.idex = presentcycle.idex;
                nextcycle.ifid = presentcycle.ifid;
                presentcycle = nextcycle;
                continue;
            }
        }




        //****Execution stage*******//
        if(presentcycle.idex.no_operation == 0){
            if(presentcycle.idex.oc == 0 || presentcycle.idex.oc == 35 || presentcycle.idex.oc == 43 || presentcycle.idex.oc == 8 || presentcycle.idex.oc==28){// for R type
                nextcycle.exmem.ALUresult = aluobject.ALUoperation(presentcycle.idex.funct,presentcycle.idex.Read_data1,presentcycle.idex.Read_data2);
            }
            nextcycle.exmem.Storedata = presentcycle.idex.Storedata;
            nextcycle.exmem.rd_mem = presentcycle.idex.rd_mem;
            nextcycle.exmem.wrt_mem = presentcycle.idex.wrt_mem;
            nextcycle.exmem.wrt_enable = presentcycle.idex.wrt_enable;
            nextcycle.exmem.Rd = presentcycle.idex.Rd;
            nextcycle.exmem.Rs = presentcycle.idex.Rs;
            nextcycle.exmem.Rt = presentcycle.idex.Rt;



        }
        nextcycle.exmem.no_operation = presentcycle.idex.no_operation;
        
 


        //****Instruction Decode stage****//
        if(presentcycle.ifid.no_operation == 0){
            opcode = getopcode(presentcycle.ifid.instr);
            nextcycle.idex.oc = opcode;

        if (opcode == 0){   // for R type instructions
            rs = getrs(presentcycle.ifid.instr);
            rt = getrt(presentcycle.ifid.instr);
            rd = getrd(presentcycle.ifid.instr);
            func = getfunc(presentcycle.ifid.instr);

            rf.readregister(rs, rt, rd, 0, 0);
            nextcycle.idex.Read_data1 = rf.readdata1;
            nextcycle.idex.Read_data2= rf.readdata2;
            nextcycle.idex.Rs=rs;
            nextcycle.idex.Rt=rt;
            nextcycle.idex.Rd=rd;
            
            nextcycle.idex.wrt_enable=1;
            nextcycle.idex.rd_mem=0;
            nextcycle.idex.wrt_mem=0;
            nextcycle.idex.funct=func;

        }

        else if(opcode==28){ // for mul instructions
            rs = getrs(presentcycle.ifid.instr);
            rt = getrt(presentcycle.ifid.instr);
            rd = getrd(presentcycle.ifid.instr);
            func = getfunc(presentcycle.ifid.instr);

            rf.readregister(rs, rt, rd, 0, 0);
            nextcycle.idex.Read_data1 = rf.readdata1;
            nextcycle.idex.Read_data2= rf.readdata2;
            nextcycle.idex.Rs=rs;
            nextcycle.idex.Rt=rt;
            nextcycle.idex.Rd=rd;
            
            nextcycle.idex.wrt_enable=1;
            nextcycle.idex.rd_mem=0;
            nextcycle.idex.wrt_mem=0;
            nextcycle.idex.funct=func;


        }

        else if(opcode==35){   //for load word instructions
            rs = getrs(presentcycle.ifid.instr);
            rt = getrt(presentcycle.ifid.instr);
            offset=getoffset(presentcycle.ifid.instr);
            bit32offset = signExtend(offset);


            rf.readregister(rs, 0, rt, 0, 0);
            nextcycle.idex.Read_data1=rf.readdata1;
             nextcycle.idex.Read_data2= bit32offset;

             nextcycle.idex.rd_mem=1;
             nextcycle.idex.wrt_mem=0;
             nextcycle.idex.wrt_enable=1;
             nextcycle.idex.Rs=rs;
             nextcycle.idex.Rd=rt;
             nextcycle.idex.funct=32;

        }

        else if (opcode == 43){ //for store word instructions
            rs = getrs(presentcycle.ifid.instr);
            rt = getrt(presentcycle.ifid.instr);
            offset=getoffset(presentcycle.ifid.instr);
            bit32offset = signExtend(offset);

            rf.readregister(rs, rt, 0, 0, 0);
            nextcycle.idex.Read_data1=rf.readdata1;
             nextcycle.idex.Read_data2= bit32offset;
             nextcycle.idex.Imm=offset;
             nextcycle.idex.rd_mem=0;
             nextcycle.idex.wrt_mem=1;
             nextcycle.idex.wrt_enable=0;
             nextcycle.idex.Rs=rs;
             nextcycle.idex.Rt=rt;
             nextcycle.idex.funct=32;
             nextcycle.idex.Storedata = rf.readdata2;


         }

         else if(opcode==8){//for add immediate instructions
            rs = getrs(presentcycle.ifid.instr);
            rt = getrt(presentcycle.ifid.instr);
            offset=getoffset(presentcycle.ifid.instr);
            bit32offset = signExtend(offset);
            



            rf.readregister(rs, 0, rt, 0, 0);
            nextcycle.idex.Read_data1=rf.readdata1;
             nextcycle.idex.Read_data2= bit32offset;

             nextcycle.idex.rd_mem=0;
             nextcycle.idex.wrt_mem=0;
             nextcycle.idex.wrt_enable=1;
             nextcycle.idex.Rs=rs;
             nextcycle.idex.Rd=rt;
             nextcycle.idex.funct=32;
         }

         else if(opcode == 4){ // for branch equal instructions
            rs = getrs(presentcycle.ifid.instr);
            rt = getrt(presentcycle.ifid.instr);
            offset=getoffset(presentcycle.ifid.instr);
            bit32offset = signExtend(offset);


            nextcycle.idex.wrt_enable = 0;
            nextcycle.idex.rd_mem = 0;
            nextcycle.idex.wrt_mem = 0;
            nextcycle.idex.oc = opcode;
            nextcycle.idex.Rs = rs;
            nextcycle.idex.Rt = rt;


             rf.readregister(rs, rt, 0, 0, 0);
             //cout << rf.readdata1 << " " << rf.readdata2 << endl;
             
            if((presentcycle.idex.no_operation == 0)&&(presentcycle.idex.wrt_enable == 1)){ // for stalling
            if((presentcycle.idex.Rd == rs) || (presentcycle.idex.Rd == rt)){
                nextcycle.idex.no_operation = 1;
            
                nextcycle.ifid = presentcycle.ifid;
                presentcycle = nextcycle;
                continue;
            }
         }
         

            if(rf.readdata1 == rf.readdata2){
                nextcycle.idex.no_operation = 0;
                nextcycle.ifid.no_operation = 1;

                programcounter = bit32offset.to_ulong()*4;
                stop = 0;

                presentcycle = nextcycle ;
                continue;
            }
         }

         else if(opcode == 2){ // for jump instructions
            jumpaddress = getjumpaddress(presentcycle.ifid.instr);
           

            nextcycle.idex.no_operation = 0;
            nextcycle.ifid.no_operation = 1;

            programcounter = jumpaddress.to_ulong()*4;
            stop = 0;

            presentcycle = nextcycle;

            continue;
         }

         if((presentcycle.idex.no_operation == 0)&&(presentcycle.idex.wrt_enable == 1)){ // for stalling
            if((presentcycle.idex.Rd == rs) || (presentcycle.idex.Rd == rt)){
                nextcycle.idex.no_operation = 1;
            
                nextcycle.ifid = presentcycle.ifid;
                presentcycle = nextcycle;
                continue;
            }
         }
         if((presentcycle.idex.no_operation == 0)&&(presentcycle.idex.wrt_enable == 1)){ // for stalling
            if((presentcycle.exmem.Rd == rs) || (presentcycle.exmem.Rd == rt)){
                nextcycle.idex.no_operation = 1;

                nextcycle.ifid = presentcycle.ifid;
                presentcycle = nextcycle;
                continue;
            }
         }





    }

    nextcycle.idex.no_operation = presentcycle.ifid.no_operation;





        //******Instruction Fetch stage****//
        if(stop==0){

        instruction = i.Readmemory( programcounter);
        

        if (instruction == halt)
        {
            stop=1;
        }
        else{

            programcounter=programcounter.to_ulong()+4;

        }
        nextcycle.ifid.instr=instruction;
        

        }

        nextcycle.ifid.no_operation=stop;



        

        if(stop==1 && presentcycle.ifid.no_operation==1 && presentcycle.idex.no_operation==1 && presentcycle.exmem.no_operation==1 && presentcycle.memwb.no_operation==1){
            break;
        }

        presentcycle=nextcycle;


   }
    rf.registerfileoutput();
    mydatamem.OutputDataMem();



  return 0;
}