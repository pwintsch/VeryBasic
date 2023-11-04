
#include <iostream>
#include <string>
#include "console.h"

int main() {
    Console console;
    console.clear();
    console.print("Hello, world!");
    console.print("I am using my new console class! v2");
    
    std::cout << "Rows: "<< console.get_height() << "  Columns: " << console.get_width() <<std::endl;
//    std::cout << "I am learning C++ v2\n";
    return 0;
}
