#include <iostream>
#include "StablePot.h"

int main() {
    StablePot pot;
    pot.configureGlobalSettings();
    
    std::cout << "StablePot configured with global settings." << std::endl;
    
    return 0;
}