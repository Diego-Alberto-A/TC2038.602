#include <iostream>
#include <vector>


int main() {
    int tamano;
    std::cin>>tamano;

    std::vector<int> nums(tamano);
    for(int i=0;i<tamano;i++){
        std::cin>>nums[i];
    }
    for(auto a : nums){
        std::cout<<a<<'\n';
    }

  return 0;
}