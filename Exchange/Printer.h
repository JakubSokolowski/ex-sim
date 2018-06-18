//
// Created by jakub on 17.06.18.
//

#ifndef HESOYAM_PRINTER_H
#define HESOYAM_PRINTER_H


#include <mutex>

class ThreadSafePrinter {
public:
    static void Print(std::string const &text);
private:
    static std::mutex cout_lock_m;
};


#endif //HESOYAM_PRINTER_H
