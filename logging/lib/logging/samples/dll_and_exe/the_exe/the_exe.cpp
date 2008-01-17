// the_exe.cpp : Defines the entry point for the console application.

#include "dll_log.h"
#include "log.h"


int main(int argc, char* argv[])
{
    int i = 0;
    L_EXE_ << "beginning of exe log" << ++i;
    L_EXE_ << "message from exe - before init_logs on EXE" << ++i;
    init_logs();
    L_EXE_ << "message from exe - after init_logs on EXE" << ++i;
    L_DLL_ << "message from exe on log from DLL " << ++i;
    L_DLL_ << "message from exe on log from DLL (2) " << ++i;

    write_to_dll_logs();

    L_EXE_ << "end of exe log " << ++i;
	return 0;
}

