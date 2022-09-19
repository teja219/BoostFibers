#include<iostream>
#include<thread>
#include <boost/fiber/all.hpp>
#include <time.h>

enum state_type {CANDIDATE,FOLLOWER,LEADER};
namespace states{ 
	struct cmState
	{
			int timeout = 3;
			state_type S = FOLLOWER;
			boost::fibers::mutex mtx;
			bool killMainThread = false;
			static void restartElectionCounter(states::cmState* cm){
				  std::cout<<"In fiber2 will kill fiber1 in 4 seconds"<<std::endl;
					boost::this_fiber::sleep_for(std::chrono::seconds(4));
					cm->mtx.lock();
					cm->S = CANDIDATE;
					cm->mtx.unlock();

					std::cout<<"In fiber2 will exit main thread in 4 seconds"<<std::endl;
					boost::this_fiber::sleep_for(std::chrono::seconds(4));
					cm->mtx.lock();
					cm->killMainThread = true;
					cm->mtx.unlock();
					return;
			}
	};
}


//This basically gets killed whenver the clock times out(the timeout is some random number between 10 and 20) or someone updates 
//the module state to something other than follower
void electionTimerFiber(states::cmState* cm){

	  //I will spawn a fiber which will kill me
		boost::fibers::fiber fb2(cm->restartElectionCounter,cm);
		fb2.detach();
		std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
		while(1){
				boost::this_fiber::sleep_for(std::chrono::seconds(1));
				
				std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
				std::chrono::seconds timeElapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - start);
				std::cout<<"tick:"<<timeElapsed.count()<<std::endl;
				cm->mtx.lock();
				if(cm->S != FOLLOWER){
					  cm->mtx.unlock();
						return;
				}
				cm->mtx.unlock();
				if(timeElapsed >= std::chrono::seconds(rand()%10+10)){
						return;
				}
		}	
}

// void restartElectionCounter(states::cmState* cm){
// 	  std::cout<<"In fiber2 will kill fiber1 in 4 seconds"<<std::endl;
// 		boost::this_fiber::sleep_for(std::chrono::seconds(4));
// 		cm->mtx.lock();
// 		cm->S = CANDIDATE;
// 		cm->mtx.unlock();

// 		return;
// }


int main(int argc, char const *argv[])
{
	states::cmState cm;
	boost::fibers::fiber fb1(electionTimerFiber,&cm);
	// boost::fibers::fiber fb2(restartElectionCounter,&cm);
	//Don't detach your fibers, unless you are sure your main thread runs for a very long long time.
	fb1.join();
	
	while(!cm.killMainThread){
			boost::this_fiber::sleep_for(std::chrono::seconds(1));
			std::cout<<"In main thread"<<std::endl;
	}
	return 0;
}