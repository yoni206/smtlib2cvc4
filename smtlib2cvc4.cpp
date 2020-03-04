#include <iostream>
#include <fstream>
#include <string>
#include <locale> 
#include <algorithm>
#include <iterator>
#include <math.h> 

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

static int counter = 0;
static bool cvc4_flag;

//from https://thispointer.com/find-and-replace-all-occurrences-of-a-sub-string-in-c/
void findAndReplaceAll(std::string & data, std::string toSearch, std::string replaceStr)
{
	// Get the first occurrence
	size_t pos = data.find(toSearch);
 
	// Repeat till end is reached
	while( pos != std::string::npos)
	{
		// Replace this occurrence of Sub String
		data.replace(pos, toSearch.size(), replaceStr);
		// Get the next occurrence from the current position
		pos =data.find(toSearch, pos + replaceStr.size());
	}
}

void print_list(list<string> lis) {
  for (string s : lis) {
  }
}

void print_vec(vector<string> vec) {
  for (string v : vec) {
  }
}

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

DefineFunctionCommand* get_command(const string smtlib, unique_ptr<api::Solver> & solver) {
  return get_commands(smtlib, solver)[0];
}
string to_lower(string s) {
  stringstream ss;
  locale loc;
  for (string::size_type i=0; i<s.length(); ++i)
    ss << tolower(s[i],loc);
  return ss.str();
}

string gen_var_name(Expr e) {
  counter++;
  string result;

  stringstream ss;
  if (e.isConst()) {
    ss << e.getKind() << "_" << e;
  } else {
    ss << e.getKind() << "_" << counter;
  }
  result = to_lower(ss.str());
  findAndReplaceAll(result, "bitvector", "bv");
  findAndReplaceAll(result, "_0bin", "_b");
  return result;
}

string create_extract_node(string s, int high, int low) {
  stringstream ss;
  if (cvc4_flag) {
    ss << "CVC4::theory::bv::utils::mkExtract(" << s << ", " << high << ", " <<  low << ");";
  } else {
    ss << "solver->make_term(solver->make_op(Extract, " << high << ", " << low << "),"  << s << ");";
  }
  return ss.str();
}

string cvc4_kind_to_smt_switch_op(string k) {
  if (k == "PLUS") {
    return "Plus";
  } else if (k == "MINUS") {
    return "Minus";
  } else if (k == "MULT") {
    return "Mult";
  } else if (k == "ITE") {
    return "Ite";
  }
    cout << "support for smt-switch output does not yet include the operator " << k << endl; 
    assert(false);
}

string cvc4_kind_to_smt_switch_sort(string k) {
  if (k == "Rational") {
    return "solver->make_sort(INT)";
  }
    cout << "support for smt-switch output does not yet include the sort " << k << endl; 
    assert(false);
}

string create_ordinary_node(Kind k, vector<string> defined_children) {
    stringstream ks;
    ks << k;
    stringstream ss;
    if (cvc4_flag) {
      ss <<  "nm->mkNode(" << k;
    } else {
      ss << "solver->make_term(" << cvc4_kind_to_smt_switch_op(ks.str());
    }
    for (string child : defined_children) {
      ss <<  ", " << child;
    }
    ss << ");";
    return ss.str();
}

string mk_node(Expr e, vector<string> defined_children) {
  stringstream ss;
  Kind k = e.getKind();
  if (k == kind::BITVECTOR_EXTRACT) {
    int low =  e.getOperator().getConst<BitVectorExtract>().d_low;
    int high = e.getOperator().getConst<BitVectorExtract>().d_high;
    ss << create_extract_node(defined_children[0], high, low);
  } else {
    ss << create_ordinary_node(k, defined_children);
  }
  return ss.str();
}



//Special consts like 00... 1000... or 01111 should not be taken literally
//instead, they ahould be taken as 0, signed min and max of the appropriate bit-width.
vector<string> adjust_special_consts(Expr e, BitVector b, vector<Expr> formals,  unordered_map<Expr, pair<string, vector<string>>, ExprHashFunction> cache) {
  stringstream ss_def;
  stringstream bwss;
  bwss << formals[0] << ".getType().getBitVectorSize()";
  string bw = bwss.str();
  vector<string> result;
  string bwname = "bw_" + cache[e].first;
  result.push_back("int " + bwname + " = " + bw + ";");
  BitVector zero4 = BitVector("0000");
  BitVector s_max_4 = BitVector("0111");
  BitVector s_min_4 = BitVector("1000");
  BitVector u_max_4 = BitVector("1111");
  ss_def << "Node " << cache[e].first << " = " << endl;
  if (b == zero4) {
    ss_def << "CVC4::theory::bv::utils::mkConst(" << bwname << ", 0);";
  } else if (b == s_max_4) {
    ss_def << "CVC4::theory::bv::utils::mkConst(" << bwname << ", pow(2, " << bwname << "-1));";
  } else if (b == s_min_4) {
    ss_def << "CVC4::theory::bv::utils::mkConst(" << bwname << ", pow(2, " << bwname << ") -1 - pow(2, " << bwname << "-1));";
  } else if (b == u_max_4 ) {
    ss_def << "CVC4::theory::bv::utils::mkConst(" << bwname << ", pow(2, " << bwname << ") -1);";
  } else {
    ss_def << "somthing_that_doesn't_compile";
  }
  result.push_back(ss_def.str());
  return result;
}

vector<string> get_bv_constant(Expr e, BitVector b, vector<Expr> formals, unordered_map<Expr, pair<string, vector<string>>, ExprHashFunction> cache) {
  vector<string> result;
  if (cvc4_flag) {
     result = adjust_special_consts(e, b, formals, cache);
  } else {
    cout << "we don't support bv constant on smt-switch just yet" << endl;
    assert(false);
  }
  return result;
}
string get_const_def(unordered_map<Expr, pair<string, vector<string>>, ExprHashFunction> cache, Expr e, string k_str) {
  stringstream ss;
  if (cvc4_flag) {
      ss << "Node " <<  cache[e].first << " = " << "nm->mkConst<" << k_str << ">(" << e << ");";
  } else {
      stringstream ss_tmp;
      ss << "Term " << cache[e].first << " = " << "solver->make_term(" << e << ", " << cvc4_kind_to_smt_switch_sort(k_str) << ");";
  }
  return ss.str();
}

string get_compound_def(unordered_map<Expr, pair<string, vector<string>>, ExprHashFunction> cache, Expr e, vector<string> defined_children) {
    stringstream ss;
    if (cvc4_flag) {
      ss << "Node " << cache[e].first << " = " << mk_node(e, defined_children);
    } else {
      ss << "Term "<< cache[e].first << " = " << mk_node(e, defined_children);
    } 
    return ss.str();
}

vector<string> gen_var_def(Expr e, unordered_map<Expr, pair<string, vector<string>>, ExprHashFunction> cache, vector<Expr> formals) {
  int num_of_children = e.getNumChildren();
  vector<string> result;
  if (num_of_children == 0) {
    if (e.isConst()) {
      stringstream ss_kind;
      Kind k = e.getKind();
      ss_kind << k;
      string k_str = ss_kind.str();
      k_str = k_str.substr(string("CONST_").size());
      k_str = k_str.substr(0,1) +  to_lower(k_str.substr(1));
      string def;
      if (k_str == "Bitvector") {
        k_str = "BitVector";
        BitVector b = e.getConst<BitVector>();
        result = get_bv_constant(e, b, formals, cache);
      } else {
        stringstream ss_def;
        ss_def << get_const_def(cache, e, k_str);
        result.push_back(ss_def.str());
      }
    }  
  }
  else {
    vector<string> defined_children;
    for (Expr child : e) {
      defined_children.push_back(cache[child].first);
    }
    result.push_back(get_compound_def(cache, e, defined_children));
  }
  return result;
}

string cache_to_code(unordered_map<Expr, pair<string, vector<string>>, ExprHashFunction> cache, Expr e) {
  vector<Expr> toVisit;
  vector<Expr> order_of_definitions_with_dups;
  toVisit.push_back(e);
  while (! toVisit.empty()) {
    Expr current = toVisit.back();
    toVisit.pop_back();
    for (Expr child : current) {
      toVisit.push_back(child);
    }
    order_of_definitions_with_dups.insert(order_of_definitions_with_dups.begin(), current);
  }
  vector<Expr> order_of_definitions_no_dups;
  for (Expr e : order_of_definitions_with_dups) {
    if (!is_in(e, order_of_definitions_no_dups)) {
      order_of_definitions_no_dups.push_back(e);
    }
  }
  string result = "";
  for (Expr e : order_of_definitions_no_dups) {
    string name = cache[e].first;
    vector<string> def = cache[e].second;
    for (string d : def) {
      result += d + "\n";
    }
  }
  return result;
}

string get_function_sig(string prefix, Expr func, vector<Expr> formals) {
  stringstream ss;
  if (cvc4_flag) {
    ss << "Node ";
  } else {
    ss << "Term ";
  }
  ss << prefix << func << "(";
  for (int i=0; i< formals.size(); i++) {
    Expr formal = formals[i];
    if (cvc4_flag) {
      ss << "Node " << formal;
    } else {
      ss << "Term " << formal;
    }
    if (i < formals.size() - 1) {
      ss << ", ";
    }
  }
  if (!cvc4_flag) {
    ss << ", SmtSolver & solver";
  }
  ss << ")";
  return ss.str();
}

string get_function_body(unordered_map<Expr, pair<string, vector<string>>, ExprHashFunction> cache, Expr formula) {
  stringstream ss;
  if (cvc4_flag) {
    ss << "NodeManager* nm = NodeManager::currentNM();" << endl ; 
  } 
  string body = cache_to_code(cache, formula);
  ss << body << "return " << cache[formula].first << ";";
  ss << endl;
  return ss.str();
}

string get_code(DefineFunctionCommand* command, string prefix="") {
  Expr formula = command->getFormula();
  Expr func = command->getFunction();
  vector<Expr> formals = command->getFormals();
  vector<Expr> toVisit;
  //map each Expr to its intended c++ variable name as well as its c++ definition.
  unordered_map<Expr, pair<string, vector<string>>, ExprHashFunction> cache;
  toVisit.push_back(formula);
  while (! toVisit.empty()) {
    Expr current = toVisit.back();
    if (cache.find(current) == cache.end()) {
      vector<string> empty;
      string name;
      if (is_in(current, formals)) {
        stringstream ss;
        ss << current;
        name = ss.str();
      } else {
        name = gen_var_name(current);
      }
      cache[current] = make_pair(name, empty);
      for (Expr child : current) {
        toVisit.push_back(child);
      }
    } else {
      string name = cache[current].first;
      toVisit.pop_back();
      if (cache[current].second.empty()) {
        vector<string> def = gen_var_def(current, cache, formals);
        pair<string, vector<string>> p = make_pair(name, def);
        cache[current] = p;
      }
    }
  }
  stringstream ss;
  ss << get_function_sig(prefix, func, formals);
  ss << " {" << endl;
  ss << get_function_body(cache, formula);
  ss << "}";
  return ss.str();
}

void batch(string smtlib) {
  unique_ptr<api::Solver> solver;
  solver.reset(new api::Solver());
  vector<DefineFunctionCommand*> commands = get_commands(smtlib, solver);
  for (DefineFunctionCommand* command : commands) {
    counter = 0;
    string code = get_code(command); 
    cout << code << endl << endl;
  }
}

vector<string> get_lines(string str) {
  vector<string> cont;
  std::stringstream ss(str);
  std::string token;
  char delim = '\n';
  while (std::getline(ss, token, delim)) {
    if (token != "") {
      cont.push_back(token);
    }
  }
  return cont;
}

void print_preamble() {
  if (cvc4_flag) {
    cout << "#include \"theory/bv/theory_bv_utils.h\"" << endl;
    cout << "#include <math.h>" << endl;
    cout << "using namespace CVC4::kind;" <<endl;
    cout << "using namespace CVC4;" <<endl;
  }
}

int main(int argc, char *argv[]) {
  string smtlib = file_to_string(argv[1]);
  assert((argv[2] == string("cvc4")) || (argv[2] == string("smt-switch")));
  cvc4_flag = (argv[2] == string("cvc4"));
  print_preamble();
  //batch(smtlib);
  vector<string> lines = get_lines(smtlib);
  unique_ptr<api::Solver> solver;
  solver.reset(new api::Solver());
  for (string line : lines) {
    DefineFunctionCommand* command = get_command(line, solver);
    counter = 0;
    string code = get_code(command);
    cout << "// " << line << endl;
    cout << code << endl << endl;
  }
  return 0;
}

