#include<iostream>
#include<thread>
#include <boost/fiber/all.hpp>

struct cmState
{
		int timeout = 3;
};
//This basically tries to run for 10 seconds, checking every 1 sec, if someone responded
void electionTimer(int x){
	
	

}