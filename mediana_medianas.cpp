#include <iostream>
#include <vector>

inline void intercambiar(int& a, int& b) {
    int aux = a;
    a = b;
    b = aux;
}

void ordenar_ventana(std::vector<int>& arr, int izq, int der) {
    for (int i = izq + 1; i <= der; ++i) {
        int clave = arr[i];
        int j = i;
        while (j > izq && arr[j - 1] > clave) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = clave;
    }
}


int bfprt_pivote(std::vector<int>& arr, int izq, int der) {
    int n = der - izq + 1;
    
    if (n <= 5) {
        ordenar_ventana(arr, izq, der);
        return arr[izq + n / 2];
    }

    int num_bloques = 0;
    
    for (int i = izq; i <= der; i += 5) {
        int limite = (i + 4 < der) ? (i + 4) : der;
        ordenar_ventana(arr, i, limite);
        
        int mediana_indice = i + (limite - i) / 2;
        intercambiar(arr[izq + num_bloques], arr[mediana_indice]);
        num_bloques++;
    }

    return bfprt_pivote(arr, izq, izq + num_bloques - 1);
}


int quickselect(std::vector<int>& nums, int izq, int der, int k){
  return 0;
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
    
    auto const pivote = bfprt_pivote(nums, 0, tamano-1);
    std::cout <<"Pivote aproximado: "<< pivote << '\n';

  return 0;
}