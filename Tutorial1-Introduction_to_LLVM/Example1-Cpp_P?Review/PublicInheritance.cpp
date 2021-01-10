/*!
 * @file Public Inheritance
 */
/*! @brief Abstract Base Class @p Animal
 */
#include <iostream>
#include <typeinfo>  // std::bad_cast


class Animal {
 public:
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
  Fox &rfox = dynamic_cast<Fox &>(animal);
  try {
    Dog &rdog = dynamic_cast<Dog &>(animal);
  } catch (std::bad_cast &except) {
    std::cerr << except.what() << std::endl;
  }
  return 0;
}
