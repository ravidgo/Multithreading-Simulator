/* 046267 Computer Architecture - HW #4 */

#include "core_api.h"
#include "sim_api.h"
#include <vector>
#include <iostream>
#include <stdio.h>
using namespace std;

#define EMPTY 0

typedef struct {
    tcontext registers;
    uint32_t line = 0;
    int delay = 0;
    bool halt = false;
} thread;

///global veriables
vector <thread> threads;
uint32_t cycles = 0;
int NumOfThreads = 0;
double NumOfCommands = 0;
int NumOfHalts = 0;
int NumOfSwitchCycles = 0;



void delays_update(int NumOfCycles) {
    int delay_update = 0;
    for (int i = 0; i < NumOfThreads; i++) {
        if (threads[i].delay != EMPTY) {
            delay_update = threads[i].delay - NumOfCycles;
            threads[i].delay = (delay_update > 0) ? delay_update : EMPTY ;
        }

    }
}

/*if we are at Blocked MT we will prefer to continue with curr_thread and we check its 
 availabilty first*/
/*if we are at fine grained we want to check curr_thread last that is why we call to 
 this func with curr_thread+1 (that way we check curr_thread last iteration last loop)*/
int FindAvailableThread(int curr_thread) {
    
    for (int i = curr_thread; i < NumOfThreads; i++) {
        if (!threads[i].halt && threads[i].delay == EMPTY)
            return i;
    }
    for (int i = 0; i < curr_thread; i++) {
        if (!threads[i].halt && threads[i].delay == EMPTY)
            return i;
    }
    return 0;
}
void GlobalVariables_init() {
    cycles = 0;
    NumOfCommands = 0;
    NumOfHalts = 0;
    NumOfThreads = SIM_GetThreadsNum();
    NumOfSwitchCycles = SIM_GetSwitchCycles();

    //initializing struct
    threads.resize(NumOfThreads);
    for (int i = 0; i < NumOfThreads; i++) {
        for (int j = 0; j < REGS_COUNT; j++) {
            threads[i].registers.reg[j] = 0;
        }
    threads[i].line = 0;
    threads[i].delay = 0;
    threads[i].halt = false;

    }
    



}

void ActCommand(Instruction Command, int curr_thread) {

    if (Command.opcode == CMD_ADD) {
        threads[curr_thread].registers.reg[Command.dst_index] =
            threads[curr_thread].registers.reg[Command.src1_index] +
            threads[curr_thread].registers.reg[Command.src2_index_imm];
	
    }
    else if (Command.opcode == CMD_ADDI) {
        threads[curr_thread].registers.reg[Command.dst_index] =
            threads[curr_thread].registers.reg[Command.src1_index] +
            Command.src2_index_imm;
	
    }
    else if (Command.opcode == CMD_SUB) {
        threads[curr_thread].registers.reg[Command.dst_index] =
            threads[curr_thread].registers.reg[Command.src1_index] -
            threads[curr_thread].registers.reg[Command.src2_index_imm];
	
    }
    else if (Command.opcode == CMD_SUBI) {
        threads[curr_thread].registers.reg[Command.dst_index] =
            threads[curr_thread].registers.reg[Command.src1_index] -
            Command.src2_index_imm;
	
    }
    else if (Command.opcode == CMD_LOAD) {
        threads[curr_thread].delay = SIM_GetLoadLat();
	
        uint32_t read_addr;
        if (Command.isSrc2Imm)
            read_addr = threads[curr_thread].registers.reg[Command.src1_index] +
            Command.src2_index_imm;
        else
            read_addr = threads[curr_thread].registers.reg[Command.src1_index] +
            threads[curr_thread].registers.reg[Command.src2_index_imm];

        SIM_MemDataRead(read_addr, &threads[curr_thread].registers.reg[Command.dst_index]);
	
    }
    else if (Command.opcode == CMD_STORE) {
        threads[curr_thread].delay = SIM_GetStoreLat();
        uint32_t write_addr;

        if (Command.isSrc2Imm)
            write_addr = threads[curr_thread].registers.reg[Command.dst_index] +
            Command.src2_index_imm;
        else
            write_addr = threads[curr_thread].registers.reg[Command.dst_index] +
            threads[curr_thread].registers.reg[Command.src2_index_imm];

        SIM_MemDataWrite(write_addr, threads[curr_thread].registers.reg[Command.src1_index]);

    }
    else if (Command.opcode == CMD_HALT) {
        NumOfHalts++;
        threads[curr_thread].halt = true;

    }

}

void CORE_BlockedMT() {

    GlobalVariables_init();

    Instruction curr_command;
    int curr_thread = 0;
    int available_thread = 0 ;



    while (NumOfThreads != NumOfHalts) {
	
        available_thread = FindAvailableThread(curr_thread);

	//available is 0 whan there is no available thread for the most part except when the free thread is 0, make sure that 0 is not available before entering idle
        if (!available_thread && (threads[0].delay != 0 || threads[0].halt)) {
            cycles++;

            delays_update(1);
            continue; //all threads are either idle or halted
        }

        else if (available_thread != curr_thread) {//thread switch
            cycles = cycles + NumOfSwitchCycles;
            delays_update(NumOfSwitchCycles);
            curr_thread = available_thread;

        }

      //next command
        cycles++;
        delays_update(1);
        SIM_MemInstRead(threads[curr_thread].line, &curr_command, curr_thread);
        NumOfCommands++;
        threads[curr_thread].line++;
        ActCommand(curr_command, curr_thread);

    } 
}

void CORE_FinegrainedMT() {

    GlobalVariables_init();

    Instruction curr_command;
    int curr_thread = 0;
    int available_thread = 0;




    while (NumOfThreads != NumOfHalts) {
		
	//available is 0 whan there is no available thread for the most part except when the free thread is 0, make sure that 0 is not available before entering idle
	if (!available_thread && (threads[0].delay != 0 || threads[0].halt)) {
    		cycles++;

    		delays_update(1);
		available_thread = FindAvailableThread(curr_thread+1);
    		continue; //all threads are either idle or halted
	}
		 curr_thread = available_thread;		

        //next command
      
        cycles++;
        delays_update(1);
        SIM_MemInstRead(threads[curr_thread].line, &curr_command, curr_thread);
        NumOfCommands++;
        threads[curr_thread].line++;
        ActCommand(curr_command, curr_thread);
	available_thread = FindAvailableThread(curr_thread+1);


    }

}

double CORE_BlockedMT_CPI(){

    double cpi_val = (double)(cycles) / NumOfCommands;
	GlobalVariables_init();
    
	return cpi_val;
}

double CORE_FinegrainedMT_CPI(){
	double cpi_val = (double)(cycles) / NumOfCommands;
	GlobalVariables_init();
    
	return cpi_val;
}

void CORE_BlockedMT_CTX(tcontext* context, int threadid) {
    for (int i = 0; i < REGS_COUNT; i++)
        context[threadid].reg[i] = threads[threadid].registers.reg[i];
}

void CORE_FinegrainedMT_CTX(tcontext* context, int threadid) {
    for (int i = 0; i < REGS_COUNT; i++)
        context[threadid].reg[i] = threads[threadid].registers.reg[i];
}
