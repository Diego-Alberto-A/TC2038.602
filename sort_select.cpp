#include <iostream>
#include <vector>


int main() {
    //input
    int tamano;
    std::cin>>tamano;

    std::vector<int> nums(tamano);
    for(int i=0;i<tamano;i++){
        std::cin>>nums[i];
    }
    

  return 0;
}