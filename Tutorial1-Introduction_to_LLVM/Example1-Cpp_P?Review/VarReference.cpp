/*!
 * @file Variable Reference
 */
#include <iostream>  // Print


int main() {
  int x = 100; 
  int * ptr_x = &x;  // `ptr_x` is a pointer to `x`.
  int & ref_x =  x;  // `ref_x` is a *reference* to `x`
  int copy_x = x;    // `copy_x` is a copy of `x`.

  x = 50;  // When we modify the value of `x`, we are modifying the value of 
           //` ref_x` and the value that `ptr_x` is pointing to and at the same
           // time, but NOT the value of `copy_x`.
  std::cout << "x: " << x << ", "
            << "ptr_x: " << *ptr_x << ", "
            << "ref_x: " <<  ref_x << ", "
            << "copy_x: " << copy_x << std::endl;  // `endl` is the end-line.
  /**
   * @note We do NOT need to "take the address of" and/or "dereference" when
   * dealing when references, which make them a cleaner solution when compared
   * with pointers.
   */
}
