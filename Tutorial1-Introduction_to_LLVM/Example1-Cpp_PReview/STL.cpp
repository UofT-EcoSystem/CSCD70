/*!
 * @file C++ Standard-Template-Library (STL) Data Structure and Variable Reference
 * 
 *       Please refer to the following websites for more detailed documentations regarding:
 * 	   - `std::vector`       : http://www.cplusplus.com/reference/vector/vector/
 * 	   - `std::unordered_map`: http://www.cplusplus.com/reference/unordered_map/unordered_map/
 * 	   - More C++ Containers : http://www.cplusplus.com/reference/stl/
 */
#include <iostream>       // Print
#include <string>         // String
#include <vector>         // List
#include <unordered_map>  // Dictionary


int main()
{
  /**
   * 'std' is the standard namespace. It is used to avoid name clashes of
   * classes, variables, and functions. Classes/Variables/Functions that are
   * supported by the C++ standard library belong to this namespace.
   *
   * 'vector' is the list-like data structure in STL. It is a *template class*.
   * A template class can be *instantiated* for various types. In the example
   * below, '<unsigned>' is an instantiation of the 'vector' template class (for
   * unsigned integer). Similarly, we can create another array of floating point
   * values using the statement:
   *
   *     std::vector<float> b = { 1.0, 2.0, 3.0 };
   */
  std::vector<unsigned> a = {1, 2, 3, 4, 5};
  std::unordered_map<std::string, unsigned> b = {
        {"Red", 0}, {"Green", 1}, {"Blue", 2}
      };
  /**
   * Traverse through 'a' using *iterators*. 'std::vector<unsigned>::iterator'
   * is the typename for the iterators. If you find it tedious, you can let the
   * compiler automatically infer the typename for you:
   *
   *     for (auto iter = a.begin(); ...)
   */
  for (std::vector<unsigned>::iterator iter = a.begin();
       iter != a.end(); ++iter) {
    // dereference the iterator the obtain the array value
    std::cout << *iter << ", ";
  }
  std::cout << std::endl;

  for (std::unordered_map<std::string, unsigned>::iterator iter = b.begin();
       iter != b.end(); ++iter) {
    /**
     * When we dereference the iterator, what we get is a *reference* to a
     * *pair* object. The first element of the pair reference is the *constant*
     * key, and the second element is the value corresponding to the key.
     */
    std::pair<const std::string, unsigned> &key_value_pair = *iter;

    std::cout << "(" << key_value_pair.first << ", "
              << key_value_pair.second << "), ";
    // Alternatively, we can also write:
    //
    //     std::cout << ... iter->first ... iter->second ...
  }
  std::cout << std::endl; // newline character
  return 0;
}
