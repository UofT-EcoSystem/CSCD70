/*!
 * @file Variable Reference
 */
#include <iostream>  // Print


int main() {
  int x = 100; 
  int *x_ptr = &x;  // `x_ptr` is a pointer to `x`.
  int &x_ref =  x;  // `x_ref` is a *reference* to `x`
  int x_copy =  x;  // `x_copy` is a copy of `x`.

  x = 50;  // When we modify the value of `x`, we are modifying the value of 
           //` x_ref` and the value that `x_ptr` is pointing to and at the same
           // time, but NOT the value of `x_copy`.
  std::cout << "x: " << x << ", "
            << "x_ptr: " << *x_ptr << ", "
            << "x_ref: " <<  x_ref << ", "
            << "x_copy: " << x_copy << std::endl;  // 'endl' is the newline character
  /**
   * @note We do NOT need to "take the address of" and/or "dereference" when
   *       dealing when references, which make them a cleaner solution compared
   *       with pointers.
   */
}
