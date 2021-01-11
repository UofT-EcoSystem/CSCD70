/*!
 * @file Public Inheritance
 */
#include <iostream>
#include <typeinfo>  // std::bad_cast


/*! @brief Abstract Base Class @p Animal
 */
class Animal {
 public:
  /*! @note If a base class method is marked as *virtual*, then all the
   *        inherited methods will also be virtual. Furthermore, when invoking a
   *        virtual method from a base class pointer/reference, the decision on
   *        which to call is made based on the type that the pointer/reference
   *        is pointing to, rather than the pointer/reference itself. Clearly,
   *        an abstract method should always be virtual.
   */
  virtual void Run() = 0;
};

class Fox : public Animal {
 public:
  Fox() = default;  // Constructor
  virtual ~Fox() = default;  // Destructor

  virtual void Run() override {
    std::cout << "Fox is running" << std::endl;
  }
};

class Dog : public Animal {
 public:
  Dog() = default;  // Constructor
  virtual ~Dog() = default;  // Destructor

  virtual void Run() override {
    std::cout << "Dog is running" << std::endl;
  }
};


int main() {
  Fox fox = Fox();
  Animal &animal = fox;
  animal.Run();
  Fox &fox_ref = dynamic_cast<Fox &>(animal);
  try {
    Dog &dog_ref = dynamic_cast<Dog &>(animal);
  } catch (std::bad_cast &except) {
    std::cerr << except.what() << std::endl;
  }
  return 0;
}
