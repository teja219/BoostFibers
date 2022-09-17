#include<iostream>
#include<thread>
#include <boost/fiber/all.hpp>

//boost::fibers::fiber::join()
void fun(int x){
	
	if(x==1){
		boost::this_fiber::sleep_for(std::chrono::seconds(5));	
	}
	if(x==2){
		boost::this_fiber::sleep_for(std::chrono::seconds(2));	
	}

	std::cout<<x<<std::endl;
	std::cout<<"Hello world"<<std::endl;
	std::cout<<"Current thread:"<<std::this_thread::get_id()<<std::endl;
	std::cout<<"Note that the threads are the same!"<<std::endl;
}
//boost::context::stack_traits::default_size(
int main(int argc, char const *argv[])
{
  
  boost::fibers::fiber fb1(fun,1);
  boost::fibers::fiber fb2(fun,2);

  // Possible CPU computations in the middle. Could change shared_state as well.
  // ....
  fb1.join();
  fb2.join();

}