#ifndef _PROCESS_WANGHONGTAO_2015_09_25_
#define _PROCESS_WANGHONGTAO_2015_09_25_

#include <iostream>   
#if defined(_WINDOWS)
#include<windows.h>   
#pragma comment(lib, "Kernel32.lib")
#else

#endif

class process
{
public:
	process();
	~process();

#if defined(_WINDOWS)
	static void run_process(std::string pro_name){
		pro_name += ".exe";

		STARTUPINFO si;
		memset(&si, 0, sizeof(STARTUPINFO));  
		si.cb = sizeof(STARTUPINFO);  
		si.dwFlags = STARTF_USESHOWWINDOW;  
		si.wShowWindow = SW_SHOW;  
		PROCESS_INFORMATION pi;
		if(!CreateProcess(NULL,(LPSTR)(LPCTSTR)pro_name.c_str(),NULL,NULL,FALSE,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi)) // 
		{  
			std::cout<<"Create Fail!"<<std::endl;  
			//SLEEP(10*1000);
		}  
		else  
		{
			std::cout<<"Sucess!"<<std::endl;
		}
	};
#else
	static void run_process(std::string pro_name){
		std::string cmd = "./";
		cmd += pro_name;
		cmd += " shared &";
	    system(cmd.c_str());
	    std::cout<<"process :"<<pro_name<<" run"<<std::endl;
	};
#endif

protected:
private:
};



#endif//_PROCESS_WANGHONGTAO_2015_09_25_
