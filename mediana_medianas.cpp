#include <iostream>
#include <vector>

inline void ordenar_5(std::vector<int>& arr) {
    auto const size = arr.size();
    for (size_t i = 1; i < size; ++i) {
        int clave = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > clave) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = clave;
    }
}


int mediana_medianas(size_t SmallArraySize, std::vector<int>& nums){
  auto const size{nums.size()};
  
  if(size <= SmallArraySize){
    ordenar_5(nums);
    return nums[size / 2];
  }

  std::vector<int> medians;
  medians.reserve((size + SmallArraySize - 1) / SmallArraySize);

  for(int inicio{0};inicio<size; inicio+=SmallArraySize){
    size_t limite_indice = (inicio + SmallArraySize < size) ? (inicio + SmallArraySize) : size;
        
    std::vector<int> sub(nums.begin() + inicio, nums.begin() + limite_indice);
    medians.push_back(mediana_medianas(SmallArraySize, sub));
  }

  return mediana_medianas(SmallArraySize, medians);
}


int main() {
   constexpr size_t SmallArraySize{5};
    //input
    int tamano;
    std::cin>>tamano;

    std::vector<int> nums(tamano);
    for(int i=0;i<tamano;i++){
        std::cin>>nums[i];
    }
    
    auto const mediana = mediana_medianas(SmallArraySize, nums);
    std::cout <<"Pivote aproximado: "<< mediana << '\n';

  return 0;
}