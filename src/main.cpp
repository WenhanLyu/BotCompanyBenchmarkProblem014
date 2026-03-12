#include "Evalvisitor.h"
#include "Python3Lexer.h"
#include "Python3Parser.h"
#include "antlr4-runtime.h"
#include <iostream>
#include <pthread.h>
using namespace antlr4;

struct RunArgs {
    int argc;
    const char** argv;
    int result;
};

static void* run_interpreter(void* arg) {
    RunArgs* args = static_cast<RunArgs*>(arg);
    
    ANTLRInputStream input(std::cin);
    Python3Lexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    tokens.fill();
    Python3Parser parser(&tokens);
    tree::ParseTree *tree = parser.file_input();
    EvalVisitor visitor;
    try {
        visitor.visit(tree);
    } catch (const std::runtime_error& e) {
        std::string msg = e.what();
        std::cout << "Traceback (most recent call last):" << std::endl;
        std::cout << msg << std::endl;
    }
    
    args->result = 0;
    return nullptr;
}

int main(int argc, const char *argv[]) {
    RunArgs args;
    args.argc = argc;
    args.argv = argv;
    args.result = 0;
    
    pthread_t thread;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    // Set stack size to 64 MB to support up to 2000 levels of recursion
    pthread_attr_setstacksize(&attr, 64 * 1024 * 1024);
    
    pthread_create(&thread, &attr, run_interpreter, &args);
    pthread_join(thread, nullptr);
    pthread_attr_destroy(&attr);
    
    return args.result;
}
