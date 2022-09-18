# BoostFibers
Fibers are similar to threads, assuming a single core machine, fibers are to threads, what threads are to a process

All fibers share a single thread

A fiber can handover cpu time to another fiber whenver it does an IO or thinks it did too much work, below are two ways using which a fiber can pause(suspend/yield)

1. Suspend a fiber whenever you want it to sleep or when it makes a I/O operation, suspend will 
put the fiber to sleep and the IO call will wake it up, appropriately when you use futures and promises
    You can suspend a fiber using 
	 1. fiber::Mutex - this is a mutex created which lies in the heap memory of the process.
     2. fiber Futures and fiber promises and async io - www.youtube.com/watch?v=XDZkyQVsbDY -
        refer to this, it explains how they work in threads, it works, similarly in Fibers
     3. fiber sleep/timeout
    
2. Yield, if we think the fiber has done too much work, and it needs to hand over the thread's CPU time to another fiber


A general model while working with fibers which make async calls

Within a fiber
1. Create a promise
2. Get the future object of the promise
3. Call the async API by passing the promise in the call back handler of the Async API
4. Wait on async response by doing future.get() ----> This will suspend the existing fiber and handsover the resources to another fiber. 
5. The fiber will be ready for the scheduler when the async callback function, sets the promise.


For first timers, while compiling boost, make sure you link the library at the end 

i.e g++ boostAsyncTest.cpp -lboost_fiber -lboost_context


