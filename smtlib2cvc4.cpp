#include <iostream>
#include <fstream>
#include <string>

#include "api/cvc4cpp.h"
#include "smt/smt_engine.h"
#include "parser/parser.h"
#include "parser/parser_builder.h"
#include "parser/smt2/smt2.h"
#include "smt/command.h"

using namespace CVC4;
using namespace CVC4::kind;
using namespace CVC4::parser;
using namespace CVC4::language::input;
using namespace std;


string file_to_string(string path) {
  string STRING;
	ifstream infile;
	infile.open (path);
  string result = "";
        while(!infile.eof()) // To get you all the lines.
        {
	        getline(infile,STRING); // Saves the line in STRING.
          result += "\n" + STRING;
        }
	infile.close();
  return result;
}

Command* get_command(const string smtlib, string logic) {
  std::unique_ptr<api::Solver> d_solver;
  d_solver.reset(new api::Solver());
  InputLanguage d_lang;
  d_lang = LANG_SMTLIB_V2;
      Parser* parser = ParserBuilder(d_solver.get(), "test")
                           .withStringInput(smtlib)
                           .withInputLanguage(d_lang)
                           .build();
   std::unique_ptr<Command> cmd(
            static_cast<Smt2*>(parser)->setLogic(logic));
   Command* c = parser->nextCommand();
   return c;
}

//extract expr from define-fun command
Expr get_expr(DefineFunctionCommand* c) {
  vector<Expr> arguments = c->getFormals();
  Expr body = c->getFormula();
  return body;
}

string get_code(Expr expr) {
  return "";
}

int main(int argc, char *argv[]) {
  string smtlib = file_to_string(argv[1]);
  Command* c = get_command(smtlib, argv[2]);
  DefineFunctionCommand* dc = static_cast<DefineFunctionCommand*>(c);
  Expr e = get_expr(dc);
  cout << e << endl;

  return 0;
}

