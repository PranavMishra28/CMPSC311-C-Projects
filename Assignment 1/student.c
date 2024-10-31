#include "student.h"

// Finds out the largest element in the array
int largest(int array[], int length){
  int max = array[0];

  if (length == 1){
    return array[0];
  }
  else{
    for(int i=0; i<length; i++){
      if(array[i] > max){
        max = array[i];
      } 
    }
  }
  return max;
}

// Calculates sum
int sum(int array[], int length){
  int sum = 0;
  for(int i=0; i<length; i++){
    sum += array[i];
  }
  return sum;
}

// Swaps value of all integers
void swap(int *a, int *b){
  int temp = *a;
  *a = *b;
  *b = temp;
}

void rotate(int *a, int *b, int *c){
  //  c becomes b, b becomes a, a becomes c
  int temp = *c;
  *c = *b;
  *b = *a;
  *a = temp;
}

// Sorts the array
void sort(int array[], int length){
  for(int i = 1; i < length; ++i){   
      for(int j = 0; j <= (length - i - 1); ++j)  
        {   
            if(array[j] > array[j + 1]){
                swap(&array[j], &array[j + 1]); 
          }
      }
    }
  }


int isPrime(int num){   // Helper function for double_primes to find out whether element is prime.
  if(num <= 1){
    return 0;
  }

  for(int j=2; j <= (num/2); j++){
    if(num % j == 0){
      return 0;
    }
  }
  return 1;
}

void double_primes(int array[], int length){ // Multiplying primes in an array by 2
  for(int i = 0; i<length; i++){
    if(isPrime(array[i])){
      array[i] = 2*array[i];
    }
  }
}

// Calculates exponential power of inputted element according to the length of the element.
int power(int r, int t){
  double result = 1;
  while(t > 0){
    result *= r;
    t--;
  }
  return result;
}

// Calculates length of the integer.
int lengthOfInteger(int n){
  int len = 0;
  while(n != 0){
    len++;
    n /= 10;
  }
  return len;
}

int isArmstrong(int num){
  int remainder, result = 0, originalNum = num, numLength = lengthOfInteger(num),temp = lengthOfInteger(num);
  if(num < 0){
    return 0;
  }
  // Finding whether the array's element is an Armstrong or not.
  while(numLength > 0){
    
    remainder = originalNum % 10;
    result += power(remainder,temp);
    originalNum = originalNum / 10;
    numLength -= 1;
  }
  
  if(result == num){
    return 1;
  }
  else{
    return 0;
  }
}

void negate_armstrongs(int array[], int length){
  for (int i = 0; i < length; i++)
  {
    // If the helper function returns True it negates it.
    if ((isArmstrong(array[i])) == 1){
      array[i] = -array[i];
    }
    else{
      array[i] = array[i];
    }
  }
}