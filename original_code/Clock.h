#ifndef __CLOCK_H_
#define __CLOCK_H_
#include <iostream>
using namespace std;

class Clock{
      private:
              clock_t current;
              clock_t previous;    
              
      public:
             Clock(){previous = current = clock();}
             virtual ~Clock(){}
             
             void tick(){
                  previous = current;     
                  current = clock();
             }
             
             double getTime(){
                    return (((double)(current - previous))/CLOCKS_PER_SEC);
             }
};

#endif
