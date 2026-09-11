#include <iostream>
#include <vector>


void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> L(n1);
    std::vector<int> R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0; 
    int j = 0; 
    int k = left; 

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {        
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
    
        merge(arr, left, mid, right);
    }
}


int main() {
    //input
    int tamano;
    std::cin>>tamano;

    std::vector<int> nums(tamano);
    for(int i=0;i<tamano;i++){
        std::cin>>nums[i];
    }

    mergeSort(nums,0,nums.size()-1);

    int tam_aux = nums.size();
    int mediana;

    if(tam_aux%2==0){
      mediana = (nums[(tam_aux/2)-1] + nums[(tam_aux/2)])/2;
    }
    else{
      mediana = nums[((tam_aux+1)/2)-1];
    }

    std::cout<<mediana<<'\n';

    

  return 0;
}