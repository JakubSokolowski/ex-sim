//
// Created by jakub on 17.06.18.
//

#include "Printer.h"
#include <iostream>
#include <thread>

std::mutex ThreadSafePrinter::cout_lock_m;

void ThreadSafePrinter::Print(std::string const &text) {
    std::lock_guard<std::mutex> lock_guard(cout_lock_m);
    std::cout << text << std::endl;
}
