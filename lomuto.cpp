#include <iostream>
#include <vector>


int main() {
    std::vector<int> nums;
    int x;

    while (std::cin >> x) {
        nums.push_back(x);
    }

    for(auto a : nums){
        std::cout<<a<<'\n';
    }
    
  return 0;
}