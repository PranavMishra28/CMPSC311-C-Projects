#ifndef STUDENT_H_
#define STUDENT_H_

int largest(int array[], int length);
int sum(int array[], int length);
void swap(int *a, int *b);
void rotate(int *a, int *b, int *c);
void sort(int array[], int length);
int isPrime(int num); // Helper function for double_primes
void double_primes(int array[], int length);
int power(int r, int t); // Helper function for negate_armstrongs
int lengthOfInteger(int n); // Helper function for negate_armstrongs
int isArmstrong(int num); // Helper function for negate_armstrongs
void negate_armstrongs(int array[], int length);

#endif
