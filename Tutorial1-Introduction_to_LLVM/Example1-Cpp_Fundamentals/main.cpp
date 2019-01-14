/**
 * C++ Standard-Template-Library (STL) Data Structure and Variable Reference
 * Please refer to the following websites for more detailed documentations regarding:
 * 	- `std::vector`       : http://www.cplusplus.com/reference/vector/vector/
 * 	- `std::unordered_map`: http://www.cplusplus.com/reference/unordered_map/unordered_map/
 * 	- More C++ Containers : http://www.cplusplus.com/reference/stl/
 */ 	
#include <vector>
#include <unordered_map>

#include <string>   // C++ Strings
#include <iostream> // Print Statements

int main()
{
	// =========================================================================================
	// std::vector
	// =========================================================================================
	/**
	 * `vector` is the list-like data structure in STL.
	 * 
	 * `a` is an array of unsigned integer values.
	 * 
	 * `std` is a namespace. It is used to avoid name clashes.
	 * Functions/Data structures that are supported by the C++ standard belong to this namespace.
	 * 
	 * `< unsigned (<- typename) >` is an instantiation of the template.
	 * We can create another array of floating values using the statement:
	 * 	std::vector < float > b = { 1.0, 2.0, 3.0 };
	 */
	std::vector < unsigned > a = { 1, 2, 3, 4, 5 };

	/**
	 * Traverse through `a` using iterator.
	 * 
	 * `std::vector < unsigned > ::iterator` is the typename for the iterator.
	 * If you find it tedious, you can let the compiler automatically infer the typename for you: 
	 * 	for (auto iter = a.begin(); ...)
	 */
	for (std::vector < unsigned > ::iterator iter = a.begin(); iter != a.end(); ++iter)
	{
		// Dereference the iterator the obtain the array value.
		std::cout << *iter << ", ";
	}
	std::cout << std::endl; // newline character
	// =========================================================================================
	// std::unordered_map
	// =========================================================================================
	/**
	 * `unordered_map` is the dictionary-like data structure in STL.
	 * 
	 * `b` is a dictionary that uses string as the key and unsigned integer as the value.
	 */
	std::unordered_map < std::string, unsigned > b = 
		{
			{"Red"  , 0}, 
			{"Green", 1},
			{"Blue" , 2}
		};
	
	for (std::unordered_map < std::string, unsigned > ::iterator iter = b.begin(); iter != b.end(); ++iter)
	{
		// When we dereference the iterator, what we get is a `pair`.
		// The first element is the constant, and the second element is the value.
		std::pair < const std::string, unsigned > & key_value_pair = *iter;

		std::cout << "(" << key_value_pair.first << ", " << key_value_pair.second << "), ";
		// We can collapse the above two statements in one:
		// 	std::cout << ... iter->first ... iter->second ...
	}
	std::cout << std::endl; // newline character

	// =========================================================================================
	// Variable Reference
	// =========================================================================================

	int   x = 100; 
	int & y = x; // `y` is a reference to `x`
	int   z = x; // `z` is a copy      of `x`

	x = 50;

	std::cout << "x: " << x << ", y: " << y << ", z: " << z << std::endl;

	return 0;
}
