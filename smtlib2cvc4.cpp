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

Command* get_command(const string smtlib, string logic, unique_ptr<api::Solver> & solver) {
  InputLanguage d_lang;
  d_lang = LANG_SMTLIB_V2;
      Parser* parser = ParserBuilder(solver.get(), "test")
                           .withStringInput(smtlib)
                           .withInputLanguage(d_lang)
                           .build();
   unique_ptr<Command> cmd(
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

string get_var_name(Expr e) {
  static int counter = 0;
  counter++;
  string result;

  std::stringstream ss;
  ss << e.getKind() << "_" << counter;
  result = ss.str(); 
  return result;
}

string mk_node(Kind k, vector<string> defined_children) {
  std::stringstream ss;
  ss <<  "nm->mkNode(" << k;
  for (string child : defined_children) {
    ss <<  ", " << child;
  }
  ss << ");";
  return ss.str();
}

string get_def(Expr e, unordered_map<Expr, pair<string, string>, ExprHashFunction> cache) {
  int num_of_children = e.getNumChildren();
  if (num_of_children == 0) {
    return "";
  }
  else {
    vector<string> defined_children;
    for (Expr child : e) {
      defined_children.push_back(cache[child].first);
    }
    return mk_node(e.getKind(), defined_children);
  }

}

string cache_to_code(unordered_map<Expr, pair<string, string>, ExprHashFunction> cache, Expr e) {
  list<string> definitions;
  vector<Expr> toVisit;
  toVisit.push_back(e);
  while (! toVisit.empty()) {
    Expr current = toVisit.back();
    toVisit.pop_back();
    for (Expr child : current) {
      toVisit.push_back(child);
    }
    string name = cache[current].first;
    string def = cache[current].second;
    string line = "Expr " + name + " = " + def + ";";
    definitions.push_front(line);
  }
  string result = "";
  for (string def : definitions) {
    result += "\n" + def;
  }
  return result;
}

string get_code(Expr expr) {
  vector<Expr> toVisit;
  //map each Expr to its intended c++ variable name as well as its c++ definition.
  unordered_map<Expr,pair<string, string>, ExprHashFunction> cache;
  toVisit.push_back(expr);
  while (! toVisit.empty()) {
    Expr current = toVisit.back();
    if (cache.find(current) == cache.end()) {
      cache[current] = make_pair("", "");
      for (Expr child : current) {
        toVisit.push_back(child);
      }
    } else {
      toVisit.pop_back();
      if (cache[current].first == "" && cache[current].second == "") {
        string name = get_var_name(current);
        string def = get_def(current, cache);
        pair<string, string> p = make_pair(name, def);
        cache[current] = p;
      }
    }
  }
  string result = cache_to_code(cache, expr);
  return result;
}

int main(int argc, char *argv[]) {
  string smtlib = file_to_string(argv[1]);
  unique_ptr<api::Solver> solver;
  solver.reset(new api::Solver());
  Command* c = get_command(smtlib, argv[2], solver);
  DefineFunctionCommand* dc = static_cast<DefineFunctionCommand*>(c);
  Expr e = get_expr(dc);
  string code = get_code(e);
  cout << code << endl;
  

  return 0;
}

