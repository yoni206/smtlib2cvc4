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
    cout << "panda s = " << s << endl;
  }
}

void print_vec(vector<string> vec) {
  for (string v : vec) {
    cout << " panda v = " << v << endl;
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
  // ss_def <<  "nm->mkConst<" << k << ">(BitVector(\"" << b << "\"));";
}

vector<string> gen_var_def(Expr e, unordered_map<Expr, pair<string, vector<string>>, ExprHashFunction> cache, vector<Expr> formals) {
  int num_of_children = e.getNumChildren();
  vector<string> result;
  if (num_of_children == 0) {
    if (e.isConst()) {
      stringstream ss_kind;
      ss_kind << e.getKind();
      string k = ss_kind.str();
      k = k.substr(string("CONST_").size());
      k = k.substr(0,1) +  to_lower(k.substr(1));
      string def;
      if (k == "Bitvector") {
        k = "BitVector";
        BitVector b = e.getConst<BitVector>();
        result = adjust_special_consts(e, b, formals, cache);
      } else {
        stringstream ss_def;
        ss_def << "Node " <<  cache[e].first << " = " << "nm->mkConst<" << k << ">(" << e << ");";
        result.push_back(ss_def.str());
      }
    }  
  }
  else {
    vector<string> defined_children;
    for (Expr child : e) {
      defined_children.push_back(cache[child].first);
    }
    stringstream ss;
    ss << "Node " << cache[e].first << " = " << mk_node(e, defined_children);
    result.push_back(ss.str());
  }
  return result;
}

string cache_to_code(unordered_map<Expr, pair<string, vector<string>>, ExprHashFunction> cache, Expr e) {
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
      vector<string> def = cache[current].second;
      if (def.size() != 0) {
        definitions.insert(definitions.begin(), def.begin(), def.end());
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

int main(int argc, char *argv[]) {
  string smtlib = file_to_string(argv[1]);
  cout << "#include \"theory/bv/theory_bv_utils.h\"" << endl;
  cout << "#include <math.h>" << endl;
  cout << "using namespace CVC4::kind;" <<endl;
  cout << "using namespace CVC4;" <<endl;
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

