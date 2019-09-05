#include <iostream>
#include <fstream>
#include <string>
#include <locale> 

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

bool is_in(Expr e, vector<Expr> vec) {
  bool result = false;
  for (Expr elem : vec) {
    if (elem == e) {
      result = true;
      break;
    }
  }
  return result;

}

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

vector<DefineFunctionCommand*> get_commands(const string smtlib, unique_ptr<api::Solver> & solver) {
  InputLanguage d_lang;
  d_lang = LANG_SMTLIB_V2;
      Parser* parser = ParserBuilder(solver.get(), "test")
                           .withStringInput(smtlib)
                           .withInputLanguage(d_lang)
                           .build();
   unique_ptr<Command> cmd(static_cast<Smt2*>(parser)->setLogic("ALL"));
   vector<DefineFunctionCommand*> result;

   for (Command* c = parser->nextCommand(); c != NULL; c = parser->nextCommand() ) {
     DefineFunctionCommand* dc = static_cast<DefineFunctionCommand*>(c);
     result.push_back(dc);
   }
   return result;
}

string to_lower(string s) {
  stringstream ss;
  locale loc;
  for (string::size_type i=0; i<s.length(); ++i)
    ss << tolower(s[i],loc);
  return ss.str();
}

string gen_var_name(Expr e) {
  static int counter = 0;
  counter++;
  string result;

  stringstream ss;
  if (e.isConst()) {
    ss << e.getKind() << "_" << e;
  } else {
    ss << e.getKind() << "_" << counter;
  }
  result = to_lower(ss.str());
  return result;
}

string mk_node(Expr e, vector<string> defined_children) {
  stringstream ss;
  if (e.getKind() == kind::BITVECTOR_EXTRACT) {
    int low =  e.getOperator().getConst<BitVectorExtract>().low;
    int high = e.getOperator().getConst<BitVectorExtract>().high;
    ss << "CVC4::theory::bv::utils::mkExtract(" << defined_children[0] << ", " << high << ", " <<  low << ");";
  } else {
    Kind k = e.getKind();
    ss <<  "nm->mkNode(" << k;
    for (string child : defined_children) {
      ss <<  ", " << child;
    }
    ss << ");";
  }
  return ss.str();
}

string gen_var_def(Expr e, unordered_map<Expr, pair<string, string>, ExprHashFunction> cache) {
  int num_of_children = e.getNumChildren();
  if (num_of_children == 0) {
    if (e.isConst()) {
      stringstream ss_kind;
      ss_kind << e.getKind();
      string k = ss_kind.str();
      k = k.substr(string("CONST_").size());
      k = k.substr(0,1) +  to_lower(k.substr(1));
      stringstream ss_def;
      ss_def <<  "nm->mkConst<" << k << ">(" << e << ");";
      return ss_def.str();
    } else {
      return "";
    }
  }
  else {
    vector<string> defined_children;
    for (Expr child : e) {
      defined_children.push_back(cache[child].first);
    }
    return mk_node(e, defined_children);
  }

}

string cache_to_code(unordered_map<Expr, pair<string, string>, ExprHashFunction> cache, Expr e) {
  list<string> definitions;
  vector<Expr> toVisit;
  vector<Expr> visited;
  toVisit.push_back(e);
  while (! toVisit.empty()) {
    Expr current = toVisit.back();
    toVisit.pop_back();
    if (! is_in(current, visited)) {
      for (Expr child : current) {
        toVisit.push_back(child);
      }
      string name = cache[current].first;
      string def = cache[current].second;
      if (def != "") {
        string line = "Node " + name + " = " + def; 
        definitions.push_front(line);
      }
      visited.push_back(current);
    }
  }
  string result = "";
  for (string def : definitions) {
    result += def + "\n";
  }
  return result;
}


string get_code(DefineFunctionCommand* command, string prefix="") {
  Expr formula = command->getFormula();
  Expr func = command->getFunction();
  vector<Expr> formals = command->getFormals();
  vector<Expr> toVisit;
  //map each Expr to its intended c++ variable name as well as its c++ definition.
  unordered_map<Expr,pair<string, string>, ExprHashFunction> cache;
  toVisit.push_back(formula);
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
        string name;
        if (is_in(current, formals)) {
          stringstream ss;
          ss << current;
          name = ss.str();
        } else {
          name = gen_var_name(current);
        }
        string def = gen_var_def(current, cache);
        pair<string, string> p = make_pair(name, def);
        cache[current] = p;
      }
    }
  }
  stringstream ss;
  ss << "// ";
  command->toStream(ss, -1, false, 0, language::output::LANG_SMTLIB_V2);
  ss << endl;
  ss << "Node " << prefix << func << "(";
  for (int i=0; i< formals.size(); i++) {
    Expr formal = formals[i];
    ss << "Node " << formal;
    if (i < formals.size() - 1) {
      ss << ", ";
    }
  }
  ss << ") {" << endl;
  ss << "NodeManager* nm = NodeManager::currentNM();" << endl ;
  string body = cache_to_code(cache, formula);
  ss << body << "return " << cache[formula].first << ";";
  ss << endl;
  ss << "}";
  return ss.str();
}

int main(int argc, char *argv[]) {
  string smtlib = file_to_string(argv[1]);
  unique_ptr<api::Solver> solver;
  solver.reset(new api::Solver());
  vector<DefineFunctionCommand*> commands = get_commands(smtlib, solver);
  cout << "#include \"theory/bv/theory_bv_utils.h\"" << endl;
  cout << "using namespace CVC4::kind;" <<endl;
  cout << "using namespace CVC4;" <<endl;
  for (DefineFunctionCommand* command : commands) {
    string code = get_code(command); 
    cout << code << endl << endl;
  }
  return 0;
}

