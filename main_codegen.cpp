#include <iostream>
#include <string>

#include "driver.hpp"
#include "llvm_printer.hpp"

#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/raw_ostream.h"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: paracl_codegen <input-file>\n";
        return 1;
    }

    Driver drv;
    int status = drv.parse(argv[1]);

    if (status != 0 || drv.ast.globalScope == nullptr)
    {
        std::cerr << "Parsing failed\n";
        return status != 0 ? status : 1;
    }

    try
    {
        llvm::LLVMContext context;
        LLVMPrinter printer(context);

        printer.generate(drv.ast);
        printer.dump(llvm::outs());
    }
    
    catch (const std::exception& e)
    {
        std::cerr << "Codegen error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}