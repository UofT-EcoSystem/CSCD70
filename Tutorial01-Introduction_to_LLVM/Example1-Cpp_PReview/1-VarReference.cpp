/**
 * @file Variable Reference
 */
#include <iostream> // Print

int main() {
  int X = 100;
  int *XPtr = &X; // `XPtr` is a pointer to `X`.
  int &XRef = X;  // `XRef` is a *reference* to `X`.
  int XCopy = X;  // `XCopy` is a copy of `X`.

  X = 50; // When we modify the value of `X`, we are modifying the value of
          //` XRef` and the value that `XPtr` is pointing to and at the same
          // time, but NOT the value of `XCopy`.
  std::cout << "X: " << X << ", "
            << "XPtr: " << *XPtr << ", "
            << "XRef: " << XRef << ", "
            << "XCopy: " << XCopy
            << std::endl; // 'endl' is the newline character
  /**
   * @note We do NOT need to "take the address of" and/or "dereference" when
   *       dealing when references, which make them a cleaner solution compared
   *       with pointers.
   */
}
