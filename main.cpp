#include "vector.hpp"


 
#include <iostream>
#include <iomanip>
#include <string>
 
 

int main()
{
    
     ft::vector<int> myvector;

    // myvector.push_back(3);
  // set some content in the vector:
  for (int i=0; i<100; i++) myvector.push_back(i);

  std::cout << "size: " << (int) myvector.size() << '\n';
  std::cout << "capacity: " << (int) myvector.capacity() << '\n';


    



}