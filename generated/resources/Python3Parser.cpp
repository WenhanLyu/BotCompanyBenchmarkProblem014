
// Generated from Python3Parser.g4 by ANTLR 4.13.2


#include "Python3ParserVisitor.h"

#include "Python3Parser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct Python3ParserStaticData final {
  Python3ParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  Python3ParserStaticData(const Python3ParserStaticData&) = delete;
  Python3ParserStaticData(Python3ParserStaticData&&) = delete;
  Python3ParserStaticData& operator=(const Python3ParserStaticData&) = delete;
  Python3ParserStaticData& operator=(Python3ParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag python3parserParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<Python3ParserStaticData> python3parserParserStaticData = nullptr;

void python3parserParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (python3parserParserStaticData != nullptr) {
    return;
  }
#else
  assert(python3parserParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<Python3ParserStaticData>(
    std::vector<std::string>{
      "file_input", "funcdef", "parameters", "typedargslist", "tfpdef", 
      "stmt", "simple_stmt", "small_stmt", "expr_stmt", "augassign", "flow_stmt", 
      "break_stmt", "continue_stmt", "return_stmt", "global_stmt", "compound_stmt", 
      "if_stmt", "while_stmt", "suite", "test", "or_test", "and_test", "not_test", 
      "comparison", "comp_op", "arith_expr", "addorsub_op", "term", "muldivmod_op", 
      "factor", "power", "atom_expr", "trailer", "atom", "format_string", 
      "testlist", "arglist", "argument"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "'def'", "'return'", "'if'", "'elif'", "'else'", 
      "'while'", "", "", "'or'", "'and'", "'not'", "'None'", "'True'", "'False'", 
      "'continue'", "'break'", "'global'", "", "", "", "", "", "", "", "", 
      "", "", "", "'.'", "'...'", "'*'", "'('", "')'", "','", "':'", "';'", 
      "'**'", "'='", "'['", "']'", "'|'", "'^'", "'&'", "'<<'", "'>>'", 
      "'+'", "'-'", "'/'", "'%'", "'//'", "'~'", "'{'", "'}'", "'<'", "'>'", 
      "'=='", "'>='", "'<='", "'<>'", "'!='", "'@'", "'->'", "'+='", "'-='", 
      "'*='", "'@='", "'/='", "'%='", "'&='", "'|='", "'^='", "'<<='", "'>>='", 
      "'**='", "'//='", "'f\"'"
    },
    std::vector<std::string>{
      "", "INDENT", "DEDENT", "STRING", "NUMBER", "INTEGER", "DEF", "RETURN", 
      "IF", "ELIF", "ELSE", "WHILE", "FOR", "IN", "OR", "AND", "NOT", "NONE", 
      "TRUE", "FALSE", "CONTINUE", "BREAK", "GLOBAL", "NEWLINE", "NAME", 
      "STRING_LITERAL", "FORMAT_STRING_LITERAL", "BYTES_LITERAL", "DECIMAL_INTEGER", 
      "OCT_INTEGER", "HEX_INTEGER", "BIN_INTEGER", "FLOAT_NUMBER", "IMAG_NUMBER", 
      "DOT", "ELLIPSIS", "STAR", "OPEN_PAREN", "CLOSE_PAREN", "COMMA", "COLON", 
      "SEMI_COLON", "POWER", "ASSIGN", "OPEN_BRACK", "CLOSE_BRACK", "OR_OP", 
      "XOR", "AND_OP", "LEFT_SHIFT", "RIGHT_SHIFT", "ADD", "MINUS", "DIV", 
      "MOD", "IDIV", "NOT_OP", "OPEN_BRACE", "CLOSE_BRACE", "LESS_THAN", 
      "GREATER_THAN", "EQUALS", "GT_EQ", "LT_EQ", "NOT_EQ_1", "NOT_EQ_2", 
      "AT", "ARROW", "ADD_ASSIGN", "SUB_ASSIGN", "MULT_ASSIGN", "AT_ASSIGN", 
      "DIV_ASSIGN", "MOD_ASSIGN", "AND_ASSIGN", "OR_ASSIGN", "XOR_ASSIGN", 
      "LEFT_SHIFT_ASSIGN", "RIGHT_SHIFT_ASSIGN", "POWER_ASSIGN", "IDIV_ASSIGN", 
      "FORMAT_QUOTATION", "QUOTATION", "SKIP_", "UNKNOWN_CHAR"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,84,355,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,1,0,1,0,5,0,79,8,0,10,0,12,0,82,9,0,1,0,1,0,1,
  	1,1,1,1,1,1,1,1,1,1,1,1,2,1,2,3,2,94,8,2,1,2,1,2,1,3,1,3,1,3,3,3,101,
  	8,3,1,3,1,3,1,3,1,3,3,3,107,8,3,5,3,109,8,3,10,3,12,3,112,9,3,1,4,1,4,
  	1,5,1,5,3,5,118,8,5,1,6,1,6,1,6,1,7,1,7,1,7,3,7,126,8,7,1,8,1,8,1,8,1,
  	8,1,8,1,8,5,8,134,8,8,10,8,12,8,137,9,8,3,8,139,8,8,1,9,1,9,1,10,1,10,
  	1,10,3,10,146,8,10,1,11,1,11,1,12,1,12,1,13,1,13,3,13,154,8,13,1,14,1,
  	14,1,14,1,14,5,14,160,8,14,10,14,12,14,163,9,14,1,15,1,15,1,15,3,15,168,
  	8,15,1,16,1,16,1,16,1,16,1,16,1,16,1,16,1,16,1,16,5,16,179,8,16,10,16,
  	12,16,182,9,16,1,16,1,16,1,16,3,16,187,8,16,1,17,1,17,1,17,1,17,1,17,
  	1,18,1,18,1,18,1,18,4,18,198,8,18,11,18,12,18,199,1,18,1,18,3,18,204,
  	8,18,1,19,1,19,1,20,1,20,1,20,5,20,211,8,20,10,20,12,20,214,9,20,1,21,
  	1,21,1,21,5,21,219,8,21,10,21,12,21,222,9,21,1,22,1,22,1,22,3,22,227,
  	8,22,1,23,1,23,1,23,1,23,5,23,233,8,23,10,23,12,23,236,9,23,1,24,1,24,
  	1,25,1,25,1,25,1,25,5,25,244,8,25,10,25,12,25,247,9,25,1,26,1,26,1,27,
  	1,27,1,27,1,27,5,27,255,8,27,10,27,12,27,258,9,27,1,28,1,28,1,29,1,29,
  	1,29,3,29,265,8,29,1,30,1,30,1,30,3,30,270,8,30,1,31,1,31,5,31,274,8,
  	31,10,31,12,31,277,9,31,1,32,1,32,3,32,281,8,32,1,32,1,32,1,32,1,32,1,
  	32,3,32,288,8,32,1,33,1,33,1,33,4,33,293,8,33,11,33,12,33,294,1,33,1,
  	33,1,33,1,33,1,33,3,33,302,8,33,1,33,1,33,1,33,3,33,307,8,33,1,33,1,33,
  	3,33,311,8,33,1,34,1,34,1,34,1,34,1,34,1,34,5,34,319,8,34,10,34,12,34,
  	322,9,34,1,34,1,34,1,35,1,35,1,35,5,35,329,8,35,10,35,12,35,332,9,35,
  	1,35,3,35,335,8,35,1,36,1,36,1,36,5,36,340,8,36,10,36,12,36,343,9,36,
  	1,36,3,36,346,8,36,1,37,1,37,1,37,1,37,1,37,3,37,353,8,37,1,37,0,0,38,
  	0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,
  	50,52,54,56,58,60,62,64,66,68,70,72,74,0,4,3,0,68,70,72,73,79,80,2,0,
  	59,63,65,65,1,0,51,52,2,0,36,36,53,55,366,0,80,1,0,0,0,2,85,1,0,0,0,4,
  	91,1,0,0,0,6,97,1,0,0,0,8,113,1,0,0,0,10,117,1,0,0,0,12,119,1,0,0,0,14,
  	125,1,0,0,0,16,127,1,0,0,0,18,140,1,0,0,0,20,145,1,0,0,0,22,147,1,0,0,
  	0,24,149,1,0,0,0,26,151,1,0,0,0,28,155,1,0,0,0,30,167,1,0,0,0,32,169,
  	1,0,0,0,34,188,1,0,0,0,36,203,1,0,0,0,38,205,1,0,0,0,40,207,1,0,0,0,42,
  	215,1,0,0,0,44,226,1,0,0,0,46,228,1,0,0,0,48,237,1,0,0,0,50,239,1,0,0,
  	0,52,248,1,0,0,0,54,250,1,0,0,0,56,259,1,0,0,0,58,264,1,0,0,0,60,266,
  	1,0,0,0,62,271,1,0,0,0,64,287,1,0,0,0,66,310,1,0,0,0,68,312,1,0,0,0,70,
  	325,1,0,0,0,72,336,1,0,0,0,74,352,1,0,0,0,76,79,5,23,0,0,77,79,3,10,5,
  	0,78,76,1,0,0,0,78,77,1,0,0,0,79,82,1,0,0,0,80,78,1,0,0,0,80,81,1,0,0,
  	0,81,83,1,0,0,0,82,80,1,0,0,0,83,84,5,0,0,1,84,1,1,0,0,0,85,86,5,6,0,
  	0,86,87,5,24,0,0,87,88,3,4,2,0,88,89,5,40,0,0,89,90,3,36,18,0,90,3,1,
  	0,0,0,91,93,5,37,0,0,92,94,3,6,3,0,93,92,1,0,0,0,93,94,1,0,0,0,94,95,
  	1,0,0,0,95,96,5,38,0,0,96,5,1,0,0,0,97,100,3,8,4,0,98,99,5,43,0,0,99,
  	101,3,38,19,0,100,98,1,0,0,0,100,101,1,0,0,0,101,110,1,0,0,0,102,103,
  	5,39,0,0,103,106,3,8,4,0,104,105,5,43,0,0,105,107,3,38,19,0,106,104,1,
  	0,0,0,106,107,1,0,0,0,107,109,1,0,0,0,108,102,1,0,0,0,109,112,1,0,0,0,
  	110,108,1,0,0,0,110,111,1,0,0,0,111,7,1,0,0,0,112,110,1,0,0,0,113,114,
  	5,24,0,0,114,9,1,0,0,0,115,118,3,12,6,0,116,118,3,30,15,0,117,115,1,0,
  	0,0,117,116,1,0,0,0,118,11,1,0,0,0,119,120,3,14,7,0,120,121,5,23,0,0,
  	121,13,1,0,0,0,122,126,3,16,8,0,123,126,3,20,10,0,124,126,3,28,14,0,125,
  	122,1,0,0,0,125,123,1,0,0,0,125,124,1,0,0,0,126,15,1,0,0,0,127,138,3,
  	70,35,0,128,129,3,18,9,0,129,130,3,70,35,0,130,139,1,0,0,0,131,132,5,
  	43,0,0,132,134,3,70,35,0,133,131,1,0,0,0,134,137,1,0,0,0,135,133,1,0,
  	0,0,135,136,1,0,0,0,136,139,1,0,0,0,137,135,1,0,0,0,138,128,1,0,0,0,138,
  	135,1,0,0,0,139,17,1,0,0,0,140,141,7,0,0,0,141,19,1,0,0,0,142,146,3,22,
  	11,0,143,146,3,24,12,0,144,146,3,26,13,0,145,142,1,0,0,0,145,143,1,0,
  	0,0,145,144,1,0,0,0,146,21,1,0,0,0,147,148,5,21,0,0,148,23,1,0,0,0,149,
  	150,5,20,0,0,150,25,1,0,0,0,151,153,5,7,0,0,152,154,3,70,35,0,153,152,
  	1,0,0,0,153,154,1,0,0,0,154,27,1,0,0,0,155,156,5,22,0,0,156,161,5,24,
  	0,0,157,158,5,39,0,0,158,160,5,24,0,0,159,157,1,0,0,0,160,163,1,0,0,0,
  	161,159,1,0,0,0,161,162,1,0,0,0,162,29,1,0,0,0,163,161,1,0,0,0,164,168,
  	3,32,16,0,165,168,3,34,17,0,166,168,3,2,1,0,167,164,1,0,0,0,167,165,1,
  	0,0,0,167,166,1,0,0,0,168,31,1,0,0,0,169,170,5,8,0,0,170,171,3,38,19,
  	0,171,172,5,40,0,0,172,180,3,36,18,0,173,174,5,9,0,0,174,175,3,38,19,
  	0,175,176,5,40,0,0,176,177,3,36,18,0,177,179,1,0,0,0,178,173,1,0,0,0,
  	179,182,1,0,0,0,180,178,1,0,0,0,180,181,1,0,0,0,181,186,1,0,0,0,182,180,
  	1,0,0,0,183,184,5,10,0,0,184,185,5,40,0,0,185,187,3,36,18,0,186,183,1,
  	0,0,0,186,187,1,0,0,0,187,33,1,0,0,0,188,189,5,11,0,0,189,190,3,38,19,
  	0,190,191,5,40,0,0,191,192,3,36,18,0,192,35,1,0,0,0,193,204,3,12,6,0,
  	194,195,5,23,0,0,195,197,5,1,0,0,196,198,3,10,5,0,197,196,1,0,0,0,198,
  	199,1,0,0,0,199,197,1,0,0,0,199,200,1,0,0,0,200,201,1,0,0,0,201,202,5,
  	2,0,0,202,204,1,0,0,0,203,193,1,0,0,0,203,194,1,0,0,0,204,37,1,0,0,0,
  	205,206,3,40,20,0,206,39,1,0,0,0,207,212,3,42,21,0,208,209,5,14,0,0,209,
  	211,3,42,21,0,210,208,1,0,0,0,211,214,1,0,0,0,212,210,1,0,0,0,212,213,
  	1,0,0,0,213,41,1,0,0,0,214,212,1,0,0,0,215,220,3,44,22,0,216,217,5,15,
  	0,0,217,219,3,44,22,0,218,216,1,0,0,0,219,222,1,0,0,0,220,218,1,0,0,0,
  	220,221,1,0,0,0,221,43,1,0,0,0,222,220,1,0,0,0,223,224,5,16,0,0,224,227,
  	3,44,22,0,225,227,3,46,23,0,226,223,1,0,0,0,226,225,1,0,0,0,227,45,1,
  	0,0,0,228,234,3,50,25,0,229,230,3,48,24,0,230,231,3,50,25,0,231,233,1,
  	0,0,0,232,229,1,0,0,0,233,236,1,0,0,0,234,232,1,0,0,0,234,235,1,0,0,0,
  	235,47,1,0,0,0,236,234,1,0,0,0,237,238,7,1,0,0,238,49,1,0,0,0,239,245,
  	3,54,27,0,240,241,3,52,26,0,241,242,3,54,27,0,242,244,1,0,0,0,243,240,
  	1,0,0,0,244,247,1,0,0,0,245,243,1,0,0,0,245,246,1,0,0,0,246,51,1,0,0,
  	0,247,245,1,0,0,0,248,249,7,2,0,0,249,53,1,0,0,0,250,256,3,58,29,0,251,
  	252,3,56,28,0,252,253,3,58,29,0,253,255,1,0,0,0,254,251,1,0,0,0,255,258,
  	1,0,0,0,256,254,1,0,0,0,256,257,1,0,0,0,257,55,1,0,0,0,258,256,1,0,0,
  	0,259,260,7,3,0,0,260,57,1,0,0,0,261,262,7,2,0,0,262,265,3,58,29,0,263,
  	265,3,60,30,0,264,261,1,0,0,0,264,263,1,0,0,0,265,59,1,0,0,0,266,269,
  	3,62,31,0,267,268,5,42,0,0,268,270,3,58,29,0,269,267,1,0,0,0,269,270,
  	1,0,0,0,270,61,1,0,0,0,271,275,3,66,33,0,272,274,3,64,32,0,273,272,1,
  	0,0,0,274,277,1,0,0,0,275,273,1,0,0,0,275,276,1,0,0,0,276,63,1,0,0,0,
  	277,275,1,0,0,0,278,280,5,37,0,0,279,281,3,72,36,0,280,279,1,0,0,0,280,
  	281,1,0,0,0,281,282,1,0,0,0,282,288,5,38,0,0,283,284,5,44,0,0,284,285,
  	3,38,19,0,285,286,5,45,0,0,286,288,1,0,0,0,287,278,1,0,0,0,287,283,1,
  	0,0,0,288,65,1,0,0,0,289,311,5,24,0,0,290,311,5,4,0,0,291,293,5,3,0,0,
  	292,291,1,0,0,0,293,294,1,0,0,0,294,292,1,0,0,0,294,295,1,0,0,0,295,311,
  	1,0,0,0,296,311,5,17,0,0,297,311,5,18,0,0,298,311,5,19,0,0,299,301,5,
  	37,0,0,300,302,3,70,35,0,301,300,1,0,0,0,301,302,1,0,0,0,302,303,1,0,
  	0,0,303,311,5,38,0,0,304,306,5,44,0,0,305,307,3,70,35,0,306,305,1,0,0,
  	0,306,307,1,0,0,0,307,308,1,0,0,0,308,311,5,45,0,0,309,311,3,68,34,0,
  	310,289,1,0,0,0,310,290,1,0,0,0,310,292,1,0,0,0,310,296,1,0,0,0,310,297,
  	1,0,0,0,310,298,1,0,0,0,310,299,1,0,0,0,310,304,1,0,0,0,310,309,1,0,0,
  	0,311,67,1,0,0,0,312,320,5,81,0,0,313,319,5,26,0,0,314,315,5,57,0,0,315,
  	316,3,70,35,0,316,317,5,58,0,0,317,319,1,0,0,0,318,313,1,0,0,0,318,314,
  	1,0,0,0,319,322,1,0,0,0,320,318,1,0,0,0,320,321,1,0,0,0,321,323,1,0,0,
  	0,322,320,1,0,0,0,323,324,5,82,0,0,324,69,1,0,0,0,325,330,3,38,19,0,326,
  	327,5,39,0,0,327,329,3,38,19,0,328,326,1,0,0,0,329,332,1,0,0,0,330,328,
  	1,0,0,0,330,331,1,0,0,0,331,334,1,0,0,0,332,330,1,0,0,0,333,335,5,39,
  	0,0,334,333,1,0,0,0,334,335,1,0,0,0,335,71,1,0,0,0,336,341,3,74,37,0,
  	337,338,5,39,0,0,338,340,3,74,37,0,339,337,1,0,0,0,340,343,1,0,0,0,341,
  	339,1,0,0,0,341,342,1,0,0,0,342,345,1,0,0,0,343,341,1,0,0,0,344,346,5,
  	39,0,0,345,344,1,0,0,0,345,346,1,0,0,0,346,73,1,0,0,0,347,353,3,38,19,
  	0,348,349,3,38,19,0,349,350,5,43,0,0,350,351,3,38,19,0,351,353,1,0,0,
  	0,352,347,1,0,0,0,352,348,1,0,0,0,353,75,1,0,0,0,40,78,80,93,100,106,
  	110,117,125,135,138,145,153,161,167,180,186,199,203,212,220,226,234,245,
  	256,264,269,275,280,287,294,301,306,310,318,320,330,334,341,345,352
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  python3parserParserStaticData = std::move(staticData);
}

}

Python3Parser::Python3Parser(TokenStream *input) : Python3Parser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

Python3Parser::Python3Parser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  Python3Parser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *python3parserParserStaticData->atn, python3parserParserStaticData->decisionToDFA, python3parserParserStaticData->sharedContextCache, options);
}

Python3Parser::~Python3Parser() {
  delete _interpreter;
}

const atn::ATN& Python3Parser::getATN() const {
  return *python3parserParserStaticData->atn;
}

std::string Python3Parser::getGrammarFileName() const {
  return "Python3Parser.g4";
}

const std::vector<std::string>& Python3Parser::getRuleNames() const {
  return python3parserParserStaticData->ruleNames;
}

const dfa::Vocabulary& Python3Parser::getVocabulary() const {
  return python3parserParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView Python3Parser::getSerializedATN() const {
  return python3parserParserStaticData->serializedATN;
}


//----------------- File_inputContext ------------------------------------------------------------------

Python3Parser::File_inputContext::File_inputContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Python3Parser::File_inputContext::EOF() {
  return getToken(Python3Parser::EOF, 0);
}

std::vector<tree::TerminalNode *> Python3Parser::File_inputContext::NEWLINE() {
  return getTokens(Python3Parser::NEWLINE);
}

tree::TerminalNode* Python3Parser::File_inputContext::NEWLINE(size_t i) {
  return getToken(Python3Parser::NEWLINE, i);
}

std::vector<Python3Parser::StmtContext *> Python3Parser::File_inputContext::stmt() {
  return getRuleContexts<Python3Parser::StmtContext>();
}

Python3Parser::StmtContext* Python3Parser::File_inputContext::stmt(size_t i) {
  return getRuleContext<Python3Parser::StmtContext>(i);
}


size_t Python3Parser::File_inputContext::getRuleIndex() const {
  return Python3Parser::RuleFile_input;
}


std::any Python3Parser::File_inputContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitFile_input(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::File_inputContext* Python3Parser::file_input() {
  File_inputContext *_localctx = _tracker.createInstance<File_inputContext>(_ctx, getState());
  enterRule(_localctx, 0, Python3Parser::RuleFile_input);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(80);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 6773129099545048) != 0) || _la == Python3Parser::FORMAT_QUOTATION) {
      setState(78);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case Python3Parser::NEWLINE: {
          setState(76);
          match(Python3Parser::NEWLINE);
          break;
        }

        case Python3Parser::STRING:
        case Python3Parser::NUMBER:
        case Python3Parser::DEF:
        case Python3Parser::RETURN:
        case Python3Parser::IF:
        case Python3Parser::WHILE:
        case Python3Parser::NOT:
        case Python3Parser::NONE:
        case Python3Parser::TRUE:
        case Python3Parser::FALSE:
        case Python3Parser::CONTINUE:
        case Python3Parser::BREAK:
        case Python3Parser::GLOBAL:
        case Python3Parser::NAME:
        case Python3Parser::OPEN_PAREN:
        case Python3Parser::OPEN_BRACK:
        case Python3Parser::ADD:
        case Python3Parser::MINUS:
        case Python3Parser::FORMAT_QUOTATION: {
          setState(77);
          stmt();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(82);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(83);
    match(Python3Parser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncdefContext ------------------------------------------------------------------

Python3Parser::FuncdefContext::FuncdefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Python3Parser::FuncdefContext::DEF() {
  return getToken(Python3Parser::DEF, 0);
}

tree::TerminalNode* Python3Parser::FuncdefContext::NAME() {
  return getToken(Python3Parser::NAME, 0);
}

Python3Parser::ParametersContext* Python3Parser::FuncdefContext::parameters() {
  return getRuleContext<Python3Parser::ParametersContext>(0);
}

tree::TerminalNode* Python3Parser::FuncdefContext::COLON() {
  return getToken(Python3Parser::COLON, 0);
}

Python3Parser::SuiteContext* Python3Parser::FuncdefContext::suite() {
  return getRuleContext<Python3Parser::SuiteContext>(0);
}


size_t Python3Parser::FuncdefContext::getRuleIndex() const {
  return Python3Parser::RuleFuncdef;
}


std::any Python3Parser::FuncdefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitFuncdef(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::FuncdefContext* Python3Parser::funcdef() {
  FuncdefContext *_localctx = _tracker.createInstance<FuncdefContext>(_ctx, getState());
  enterRule(_localctx, 2, Python3Parser::RuleFuncdef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(85);
    match(Python3Parser::DEF);
    setState(86);
    match(Python3Parser::NAME);
    setState(87);
    parameters();
    setState(88);
    match(Python3Parser::COLON);
    setState(89);
    suite();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParametersContext ------------------------------------------------------------------

Python3Parser::ParametersContext::ParametersContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Python3Parser::ParametersContext::OPEN_PAREN() {
  return getToken(Python3Parser::OPEN_PAREN, 0);
}

tree::TerminalNode* Python3Parser::ParametersContext::CLOSE_PAREN() {
  return getToken(Python3Parser::CLOSE_PAREN, 0);
}

Python3Parser::TypedargslistContext* Python3Parser::ParametersContext::typedargslist() {
  return getRuleContext<Python3Parser::TypedargslistContext>(0);
}


size_t Python3Parser::ParametersContext::getRuleIndex() const {
  return Python3Parser::RuleParameters;
}


std::any Python3Parser::ParametersContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitParameters(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::ParametersContext* Python3Parser::parameters() {
  ParametersContext *_localctx = _tracker.createInstance<ParametersContext>(_ctx, getState());
  enterRule(_localctx, 4, Python3Parser::RuleParameters);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(91);
    match(Python3Parser::OPEN_PAREN);
    setState(93);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == Python3Parser::NAME) {
      setState(92);
      typedargslist();
    }
    setState(95);
    match(Python3Parser::CLOSE_PAREN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypedargslistContext ------------------------------------------------------------------

Python3Parser::TypedargslistContext::TypedargslistContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<Python3Parser::TfpdefContext *> Python3Parser::TypedargslistContext::tfpdef() {
  return getRuleContexts<Python3Parser::TfpdefContext>();
}

Python3Parser::TfpdefContext* Python3Parser::TypedargslistContext::tfpdef(size_t i) {
  return getRuleContext<Python3Parser::TfpdefContext>(i);
}

std::vector<tree::TerminalNode *> Python3Parser::TypedargslistContext::ASSIGN() {
  return getTokens(Python3Parser::ASSIGN);
}

tree::TerminalNode* Python3Parser::TypedargslistContext::ASSIGN(size_t i) {
  return getToken(Python3Parser::ASSIGN, i);
}

std::vector<Python3Parser::TestContext *> Python3Parser::TypedargslistContext::test() {
  return getRuleContexts<Python3Parser::TestContext>();
}

Python3Parser::TestContext* Python3Parser::TypedargslistContext::test(size_t i) {
  return getRuleContext<Python3Parser::TestContext>(i);
}

std::vector<tree::TerminalNode *> Python3Parser::TypedargslistContext::COMMA() {
  return getTokens(Python3Parser::COMMA);
}

tree::TerminalNode* Python3Parser::TypedargslistContext::COMMA(size_t i) {
  return getToken(Python3Parser::COMMA, i);
}


size_t Python3Parser::TypedargslistContext::getRuleIndex() const {
  return Python3Parser::RuleTypedargslist;
}


std::any Python3Parser::TypedargslistContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitTypedargslist(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::TypedargslistContext* Python3Parser::typedargslist() {
  TypedargslistContext *_localctx = _tracker.createInstance<TypedargslistContext>(_ctx, getState());
  enterRule(_localctx, 6, Python3Parser::RuleTypedargslist);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(97);
    tfpdef();
    setState(100);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == Python3Parser::ASSIGN) {
      setState(98);
      match(Python3Parser::ASSIGN);
      setState(99);
      test();
    }
    setState(110);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == Python3Parser::COMMA) {
      setState(102);
      match(Python3Parser::COMMA);
      setState(103);
      tfpdef();
      setState(106);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == Python3Parser::ASSIGN) {
        setState(104);
        match(Python3Parser::ASSIGN);
        setState(105);
        test();
      }
      setState(112);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TfpdefContext ------------------------------------------------------------------

Python3Parser::TfpdefContext::TfpdefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Python3Parser::TfpdefContext::NAME() {
  return getToken(Python3Parser::NAME, 0);
}


size_t Python3Parser::TfpdefContext::getRuleIndex() const {
  return Python3Parser::RuleTfpdef;
}


std::any Python3Parser::TfpdefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitTfpdef(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::TfpdefContext* Python3Parser::tfpdef() {
  TfpdefContext *_localctx = _tracker.createInstance<TfpdefContext>(_ctx, getState());
  enterRule(_localctx, 8, Python3Parser::RuleTfpdef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(113);
    match(Python3Parser::NAME);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StmtContext ------------------------------------------------------------------

Python3Parser::StmtContext::StmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

Python3Parser::Simple_stmtContext* Python3Parser::StmtContext::simple_stmt() {
  return getRuleContext<Python3Parser::Simple_stmtContext>(0);
}

Python3Parser::Compound_stmtContext* Python3Parser::StmtContext::compound_stmt() {
  return getRuleContext<Python3Parser::Compound_stmtContext>(0);
}


size_t Python3Parser::StmtContext::getRuleIndex() const {
  return Python3Parser::RuleStmt;
}


std::any Python3Parser::StmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitStmt(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::StmtContext* Python3Parser::stmt() {
  StmtContext *_localctx = _tracker.createInstance<StmtContext>(_ctx, getState());
  enterRule(_localctx, 10, Python3Parser::RuleStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(117);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case Python3Parser::STRING:
      case Python3Parser::NUMBER:
      case Python3Parser::RETURN:
      case Python3Parser::NOT:
      case Python3Parser::NONE:
      case Python3Parser::TRUE:
      case Python3Parser::FALSE:
      case Python3Parser::CONTINUE:
      case Python3Parser::BREAK:
      case Python3Parser::GLOBAL:
      case Python3Parser::NAME:
      case Python3Parser::OPEN_PAREN:
      case Python3Parser::OPEN_BRACK:
      case Python3Parser::ADD:
      case Python3Parser::MINUS:
      case Python3Parser::FORMAT_QUOTATION: {
        enterOuterAlt(_localctx, 1);
        setState(115);
        simple_stmt();
        break;
      }

      case Python3Parser::DEF:
      case Python3Parser::IF:
      case Python3Parser::WHILE: {
        enterOuterAlt(_localctx, 2);
        setState(116);
        compound_stmt();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Simple_stmtContext ------------------------------------------------------------------

Python3Parser::Simple_stmtContext::Simple_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

Python3Parser::Small_stmtContext* Python3Parser::Simple_stmtContext::small_stmt() {
  return getRuleContext<Python3Parser::Small_stmtContext>(0);
}

tree::TerminalNode* Python3Parser::Simple_stmtContext::NEWLINE() {
  return getToken(Python3Parser::NEWLINE, 0);
}


size_t Python3Parser::Simple_stmtContext::getRuleIndex() const {
  return Python3Parser::RuleSimple_stmt;
}


std::any Python3Parser::Simple_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitSimple_stmt(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::Simple_stmtContext* Python3Parser::simple_stmt() {
  Simple_stmtContext *_localctx = _tracker.createInstance<Simple_stmtContext>(_ctx, getState());
  enterRule(_localctx, 12, Python3Parser::RuleSimple_stmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(119);
    small_stmt();
    setState(120);
    match(Python3Parser::NEWLINE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Small_stmtContext ------------------------------------------------------------------

Python3Parser::Small_stmtContext::Small_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

Python3Parser::Expr_stmtContext* Python3Parser::Small_stmtContext::expr_stmt() {
  return getRuleContext<Python3Parser::Expr_stmtContext>(0);
}

Python3Parser::Flow_stmtContext* Python3Parser::Small_stmtContext::flow_stmt() {
  return getRuleContext<Python3Parser::Flow_stmtContext>(0);
}

Python3Parser::Global_stmtContext* Python3Parser::Small_stmtContext::global_stmt() {
  return getRuleContext<Python3Parser::Global_stmtContext>(0);
}


size_t Python3Parser::Small_stmtContext::getRuleIndex() const {
  return Python3Parser::RuleSmall_stmt;
}


std::any Python3Parser::Small_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitSmall_stmt(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::Small_stmtContext* Python3Parser::small_stmt() {
  Small_stmtContext *_localctx = _tracker.createInstance<Small_stmtContext>(_ctx, getState());
  enterRule(_localctx, 14, Python3Parser::RuleSmall_stmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(125);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case Python3Parser::STRING:
      case Python3Parser::NUMBER:
      case Python3Parser::NOT:
      case Python3Parser::NONE:
      case Python3Parser::TRUE:
      case Python3Parser::FALSE:
      case Python3Parser::NAME:
      case Python3Parser::OPEN_PAREN:
      case Python3Parser::OPEN_BRACK:
      case Python3Parser::ADD:
      case Python3Parser::MINUS:
      case Python3Parser::FORMAT_QUOTATION: {
        enterOuterAlt(_localctx, 1);
        setState(122);
        expr_stmt();
        break;
      }

      case Python3Parser::RETURN:
      case Python3Parser::CONTINUE:
      case Python3Parser::BREAK: {
        enterOuterAlt(_localctx, 2);
        setState(123);
        flow_stmt();
        break;
      }

      case Python3Parser::GLOBAL: {
        enterOuterAlt(_localctx, 3);
        setState(124);
        global_stmt();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Expr_stmtContext ------------------------------------------------------------------

Python3Parser::Expr_stmtContext::Expr_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<Python3Parser::TestlistContext *> Python3Parser::Expr_stmtContext::testlist() {
  return getRuleContexts<Python3Parser::TestlistContext>();
}

Python3Parser::TestlistContext* Python3Parser::Expr_stmtContext::testlist(size_t i) {
  return getRuleContext<Python3Parser::TestlistContext>(i);
}

Python3Parser::AugassignContext* Python3Parser::Expr_stmtContext::augassign() {
  return getRuleContext<Python3Parser::AugassignContext>(0);
}

std::vector<tree::TerminalNode *> Python3Parser::Expr_stmtContext::ASSIGN() {
  return getTokens(Python3Parser::ASSIGN);
}

tree::TerminalNode* Python3Parser::Expr_stmtContext::ASSIGN(size_t i) {
  return getToken(Python3Parser::ASSIGN, i);
}


size_t Python3Parser::Expr_stmtContext::getRuleIndex() const {
  return Python3Parser::RuleExpr_stmt;
}


std::any Python3Parser::Expr_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitExpr_stmt(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::Expr_stmtContext* Python3Parser::expr_stmt() {
  Expr_stmtContext *_localctx = _tracker.createInstance<Expr_stmtContext>(_ctx, getState());
  enterRule(_localctx, 16, Python3Parser::RuleExpr_stmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(127);
    testlist();
    setState(138);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case Python3Parser::ADD_ASSIGN:
      case Python3Parser::SUB_ASSIGN:
      case Python3Parser::MULT_ASSIGN:
      case Python3Parser::DIV_ASSIGN:
      case Python3Parser::MOD_ASSIGN:
      case Python3Parser::POWER_ASSIGN:
      case Python3Parser::IDIV_ASSIGN: {
        setState(128);
        augassign();
        setState(129);
        testlist();
        break;
      }

      case Python3Parser::NEWLINE:
      case Python3Parser::ASSIGN: {
        setState(135);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == Python3Parser::ASSIGN) {
          setState(131);
          match(Python3Parser::ASSIGN);
          setState(132);
          testlist();
          setState(137);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AugassignContext ------------------------------------------------------------------

Python3Parser::AugassignContext::AugassignContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Python3Parser::AugassignContext::ADD_ASSIGN() {
  return getToken(Python3Parser::ADD_ASSIGN, 0);
}

tree::TerminalNode* Python3Parser::AugassignContext::SUB_ASSIGN() {
  return getToken(Python3Parser::SUB_ASSIGN, 0);
}

tree::TerminalNode* Python3Parser::AugassignContext::MULT_ASSIGN() {
  return getToken(Python3Parser::MULT_ASSIGN, 0);
}

tree::TerminalNode* Python3Parser::AugassignContext::DIV_ASSIGN() {
  return getToken(Python3Parser::DIV_ASSIGN, 0);
}

tree::TerminalNode* Python3Parser::AugassignContext::IDIV_ASSIGN() {
  return getToken(Python3Parser::IDIV_ASSIGN, 0);
}

tree::TerminalNode* Python3Parser::AugassignContext::MOD_ASSIGN() {
  return getToken(Python3Parser::MOD_ASSIGN, 0);
}

tree::TerminalNode* Python3Parser::AugassignContext::POWER_ASSIGN() {
  return getToken(Python3Parser::POWER_ASSIGN, 0);
}


size_t Python3Parser::AugassignContext::getRuleIndex() const {
  return Python3Parser::RuleAugassign;
}


std::any Python3Parser::AugassignContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitAugassign(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::AugassignContext* Python3Parser::augassign() {
  AugassignContext *_localctx = _tracker.createInstance<AugassignContext>(_ctx, getState());
  enterRule(_localctx, 18, Python3Parser::RuleAugassign);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(140);
    _la = _input->LA(1);
    if (!(((((_la - 68) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 68)) & 6199) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Flow_stmtContext ------------------------------------------------------------------

Python3Parser::Flow_stmtContext::Flow_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

Python3Parser::Break_stmtContext* Python3Parser::Flow_stmtContext::break_stmt() {
  return getRuleContext<Python3Parser::Break_stmtContext>(0);
}

Python3Parser::Continue_stmtContext* Python3Parser::Flow_stmtContext::continue_stmt() {
  return getRuleContext<Python3Parser::Continue_stmtContext>(0);
}

Python3Parser::Return_stmtContext* Python3Parser::Flow_stmtContext::return_stmt() {
  return getRuleContext<Python3Parser::Return_stmtContext>(0);
}


size_t Python3Parser::Flow_stmtContext::getRuleIndex() const {
  return Python3Parser::RuleFlow_stmt;
}


std::any Python3Parser::Flow_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitFlow_stmt(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::Flow_stmtContext* Python3Parser::flow_stmt() {
  Flow_stmtContext *_localctx = _tracker.createInstance<Flow_stmtContext>(_ctx, getState());
  enterRule(_localctx, 20, Python3Parser::RuleFlow_stmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(145);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case Python3Parser::BREAK: {
        enterOuterAlt(_localctx, 1);
        setState(142);
        break_stmt();
        break;
      }

      case Python3Parser::CONTINUE: {
        enterOuterAlt(_localctx, 2);
        setState(143);
        continue_stmt();
        break;
      }

      case Python3Parser::RETURN: {
        enterOuterAlt(_localctx, 3);
        setState(144);
        return_stmt();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Break_stmtContext ------------------------------------------------------------------

Python3Parser::Break_stmtContext::Break_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Python3Parser::Break_stmtContext::BREAK() {
  return getToken(Python3Parser::BREAK, 0);
}


size_t Python3Parser::Break_stmtContext::getRuleIndex() const {
  return Python3Parser::RuleBreak_stmt;
}


std::any Python3Parser::Break_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitBreak_stmt(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::Break_stmtContext* Python3Parser::break_stmt() {
  Break_stmtContext *_localctx = _tracker.createInstance<Break_stmtContext>(_ctx, getState());
  enterRule(_localctx, 22, Python3Parser::RuleBreak_stmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(147);
    match(Python3Parser::BREAK);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Continue_stmtContext ------------------------------------------------------------------

Python3Parser::Continue_stmtContext::Continue_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Python3Parser::Continue_stmtContext::CONTINUE() {
  return getToken(Python3Parser::CONTINUE, 0);
}


size_t Python3Parser::Continue_stmtContext::getRuleIndex() const {
  return Python3Parser::RuleContinue_stmt;
}


std::any Python3Parser::Continue_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitContinue_stmt(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::Continue_stmtContext* Python3Parser::continue_stmt() {
  Continue_stmtContext *_localctx = _tracker.createInstance<Continue_stmtContext>(_ctx, getState());
  enterRule(_localctx, 24, Python3Parser::RuleContinue_stmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(149);
    match(Python3Parser::CONTINUE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Return_stmtContext ------------------------------------------------------------------

Python3Parser::Return_stmtContext::Return_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Python3Parser::Return_stmtContext::RETURN() {
  return getToken(Python3Parser::RETURN, 0);
}

Python3Parser::TestlistContext* Python3Parser::Return_stmtContext::testlist() {
  return getRuleContext<Python3Parser::TestlistContext>(0);
}


size_t Python3Parser::Return_stmtContext::getRuleIndex() const {
  return Python3Parser::RuleReturn_stmt;
}


std::any Python3Parser::Return_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitReturn_stmt(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::Return_stmtContext* Python3Parser::return_stmt() {
  Return_stmtContext *_localctx = _tracker.createInstance<Return_stmtContext>(_ctx, getState());
  enterRule(_localctx, 26, Python3Parser::RuleReturn_stmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(151);
    match(Python3Parser::RETURN);
    setState(153);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 6773129083813912) != 0) || _la == Python3Parser::FORMAT_QUOTATION) {
      setState(152);
      testlist();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Global_stmtContext ------------------------------------------------------------------

Python3Parser::Global_stmtContext::Global_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Python3Parser::Global_stmtContext::GLOBAL() {
  return getToken(Python3Parser::GLOBAL, 0);
}

std::vector<tree::TerminalNode *> Python3Parser::Global_stmtContext::NAME() {
  return getTokens(Python3Parser::NAME);
}

tree::TerminalNode* Python3Parser::Global_stmtContext::NAME(size_t i) {
  return getToken(Python3Parser::NAME, i);
}

std::vector<tree::TerminalNode *> Python3Parser::Global_stmtContext::COMMA() {
  return getTokens(Python3Parser::COMMA);
}

tree::TerminalNode* Python3Parser::Global_stmtContext::COMMA(size_t i) {
  return getToken(Python3Parser::COMMA, i);
}


size_t Python3Parser::Global_stmtContext::getRuleIndex() const {
  return Python3Parser::RuleGlobal_stmt;
}


std::any Python3Parser::Global_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitGlobal_stmt(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::Global_stmtContext* Python3Parser::global_stmt() {
  Global_stmtContext *_localctx = _tracker.createInstance<Global_stmtContext>(_ctx, getState());
  enterRule(_localctx, 28, Python3Parser::RuleGlobal_stmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(155);
    match(Python3Parser::GLOBAL);
    setState(156);
    match(Python3Parser::NAME);
    setState(161);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == Python3Parser::COMMA) {
      setState(157);
      match(Python3Parser::COMMA);
      setState(158);
      match(Python3Parser::NAME);
      setState(163);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Compound_stmtContext ------------------------------------------------------------------

Python3Parser::Compound_stmtContext::Compound_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

Python3Parser::If_stmtContext* Python3Parser::Compound_stmtContext::if_stmt() {
  return getRuleContext<Python3Parser::If_stmtContext>(0);
}

Python3Parser::While_stmtContext* Python3Parser::Compound_stmtContext::while_stmt() {
  return getRuleContext<Python3Parser::While_stmtContext>(0);
}

Python3Parser::FuncdefContext* Python3Parser::Compound_stmtContext::funcdef() {
  return getRuleContext<Python3Parser::FuncdefContext>(0);
}


size_t Python3Parser::Compound_stmtContext::getRuleIndex() const {
  return Python3Parser::RuleCompound_stmt;
}


std::any Python3Parser::Compound_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitCompound_stmt(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::Compound_stmtContext* Python3Parser::compound_stmt() {
  Compound_stmtContext *_localctx = _tracker.createInstance<Compound_stmtContext>(_ctx, getState());
  enterRule(_localctx, 30, Python3Parser::RuleCompound_stmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(167);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case Python3Parser::IF: {
        enterOuterAlt(_localctx, 1);
        setState(164);
        if_stmt();
        break;
      }

      case Python3Parser::WHILE: {
        enterOuterAlt(_localctx, 2);
        setState(165);
        while_stmt();
        break;
      }

      case Python3Parser::DEF: {
        enterOuterAlt(_localctx, 3);
        setState(166);
        funcdef();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- If_stmtContext ------------------------------------------------------------------

Python3Parser::If_stmtContext::If_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Python3Parser::If_stmtContext::IF() {
  return getToken(Python3Parser::IF, 0);
}

std::vector<Python3Parser::TestContext *> Python3Parser::If_stmtContext::test() {
  return getRuleContexts<Python3Parser::TestContext>();
}

Python3Parser::TestContext* Python3Parser::If_stmtContext::test(size_t i) {
  return getRuleContext<Python3Parser::TestContext>(i);
}

std::vector<tree::TerminalNode *> Python3Parser::If_stmtContext::COLON() {
  return getTokens(Python3Parser::COLON);
}

tree::TerminalNode* Python3Parser::If_stmtContext::COLON(size_t i) {
  return getToken(Python3Parser::COLON, i);
}

std::vector<Python3Parser::SuiteContext *> Python3Parser::If_stmtContext::suite() {
  return getRuleContexts<Python3Parser::SuiteContext>();
}

Python3Parser::SuiteContext* Python3Parser::If_stmtContext::suite(size_t i) {
  return getRuleContext<Python3Parser::SuiteContext>(i);
}

std::vector<tree::TerminalNode *> Python3Parser::If_stmtContext::ELIF() {
  return getTokens(Python3Parser::ELIF);
}

tree::TerminalNode* Python3Parser::If_stmtContext::ELIF(size_t i) {
  return getToken(Python3Parser::ELIF, i);
}

tree::TerminalNode* Python3Parser::If_stmtContext::ELSE() {
  return getToken(Python3Parser::ELSE, 0);
}


size_t Python3Parser::If_stmtContext::getRuleIndex() const {
  return Python3Parser::RuleIf_stmt;
}


std::any Python3Parser::If_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitIf_stmt(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::If_stmtContext* Python3Parser::if_stmt() {
  If_stmtContext *_localctx = _tracker.createInstance<If_stmtContext>(_ctx, getState());
  enterRule(_localctx, 32, Python3Parser::RuleIf_stmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(169);
    match(Python3Parser::IF);
    setState(170);
    test();
    setState(171);
    match(Python3Parser::COLON);
    setState(172);
    suite();
    setState(180);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == Python3Parser::ELIF) {
      setState(173);
      match(Python3Parser::ELIF);
      setState(174);
      test();
      setState(175);
      match(Python3Parser::COLON);
      setState(176);
      suite();
      setState(182);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(186);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == Python3Parser::ELSE) {
      setState(183);
      match(Python3Parser::ELSE);
      setState(184);
      match(Python3Parser::COLON);
      setState(185);
      suite();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- While_stmtContext ------------------------------------------------------------------

Python3Parser::While_stmtContext::While_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Python3Parser::While_stmtContext::WHILE() {
  return getToken(Python3Parser::WHILE, 0);
}

Python3Parser::TestContext* Python3Parser::While_stmtContext::test() {
  return getRuleContext<Python3Parser::TestContext>(0);
}

tree::TerminalNode* Python3Parser::While_stmtContext::COLON() {
  return getToken(Python3Parser::COLON, 0);
}

Python3Parser::SuiteContext* Python3Parser::While_stmtContext::suite() {
  return getRuleContext<Python3Parser::SuiteContext>(0);
}


size_t Python3Parser::While_stmtContext::getRuleIndex() const {
  return Python3Parser::RuleWhile_stmt;
}


std::any Python3Parser::While_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitWhile_stmt(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::While_stmtContext* Python3Parser::while_stmt() {
  While_stmtContext *_localctx = _tracker.createInstance<While_stmtContext>(_ctx, getState());
  enterRule(_localctx, 34, Python3Parser::RuleWhile_stmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(188);
    match(Python3Parser::WHILE);
    setState(189);
    test();
    setState(190);
    match(Python3Parser::COLON);
    setState(191);
    suite();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SuiteContext ------------------------------------------------------------------

Python3Parser::SuiteContext::SuiteContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

Python3Parser::Simple_stmtContext* Python3Parser::SuiteContext::simple_stmt() {
  return getRuleContext<Python3Parser::Simple_stmtContext>(0);
}

tree::TerminalNode* Python3Parser::SuiteContext::NEWLINE() {
  return getToken(Python3Parser::NEWLINE, 0);
}

tree::TerminalNode* Python3Parser::SuiteContext::INDENT() {
  return getToken(Python3Parser::INDENT, 0);
}

tree::TerminalNode* Python3Parser::SuiteContext::DEDENT() {
  return getToken(Python3Parser::DEDENT, 0);
}

std::vector<Python3Parser::StmtContext *> Python3Parser::SuiteContext::stmt() {
  return getRuleContexts<Python3Parser::StmtContext>();
}

Python3Parser::StmtContext* Python3Parser::SuiteContext::stmt(size_t i) {
  return getRuleContext<Python3Parser::StmtContext>(i);
}


size_t Python3Parser::SuiteContext::getRuleIndex() const {
  return Python3Parser::RuleSuite;
}


std::any Python3Parser::SuiteContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitSuite(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::SuiteContext* Python3Parser::suite() {
  SuiteContext *_localctx = _tracker.createInstance<SuiteContext>(_ctx, getState());
  enterRule(_localctx, 36, Python3Parser::RuleSuite);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(203);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case Python3Parser::STRING:
      case Python3Parser::NUMBER:
      case Python3Parser::RETURN:
      case Python3Parser::NOT:
      case Python3Parser::NONE:
      case Python3Parser::TRUE:
      case Python3Parser::FALSE:
      case Python3Parser::CONTINUE:
      case Python3Parser::BREAK:
      case Python3Parser::GLOBAL:
      case Python3Parser::NAME:
      case Python3Parser::OPEN_PAREN:
      case Python3Parser::OPEN_BRACK:
      case Python3Parser::ADD:
      case Python3Parser::MINUS:
      case Python3Parser::FORMAT_QUOTATION: {
        enterOuterAlt(_localctx, 1);
        setState(193);
        simple_stmt();
        break;
      }

      case Python3Parser::NEWLINE: {
        enterOuterAlt(_localctx, 2);
        setState(194);
        match(Python3Parser::NEWLINE);
        setState(195);
        match(Python3Parser::INDENT);
        setState(197); 
        _errHandler->sync(this);
        _la = _input->LA(1);
        do {
          setState(196);
          stmt();
          setState(199); 
          _errHandler->sync(this);
          _la = _input->LA(1);
        } while ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 6773129091156440) != 0) || _la == Python3Parser::FORMAT_QUOTATION);
        setState(201);
        match(Python3Parser::DEDENT);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TestContext ------------------------------------------------------------------

Python3Parser::TestContext::TestContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

Python3Parser::Or_testContext* Python3Parser::TestContext::or_test() {
  return getRuleContext<Python3Parser::Or_testContext>(0);
}


size_t Python3Parser::TestContext::getRuleIndex() const {
  return Python3Parser::RuleTest;
}


std::any Python3Parser::TestContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitTest(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::TestContext* Python3Parser::test() {
  TestContext *_localctx = _tracker.createInstance<TestContext>(_ctx, getState());
  enterRule(_localctx, 38, Python3Parser::RuleTest);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(205);
    or_test();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Or_testContext ------------------------------------------------------------------

Python3Parser::Or_testContext::Or_testContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<Python3Parser::And_testContext *> Python3Parser::Or_testContext::and_test() {
  return getRuleContexts<Python3Parser::And_testContext>();
}

Python3Parser::And_testContext* Python3Parser::Or_testContext::and_test(size_t i) {
  return getRuleContext<Python3Parser::And_testContext>(i);
}

std::vector<tree::TerminalNode *> Python3Parser::Or_testContext::OR() {
  return getTokens(Python3Parser::OR);
}

tree::TerminalNode* Python3Parser::Or_testContext::OR(size_t i) {
  return getToken(Python3Parser::OR, i);
}


size_t Python3Parser::Or_testContext::getRuleIndex() const {
  return Python3Parser::RuleOr_test;
}


std::any Python3Parser::Or_testContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitOr_test(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::Or_testContext* Python3Parser::or_test() {
  Or_testContext *_localctx = _tracker.createInstance<Or_testContext>(_ctx, getState());
  enterRule(_localctx, 40, Python3Parser::RuleOr_test);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(207);
    and_test();
    setState(212);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == Python3Parser::OR) {
      setState(208);
      match(Python3Parser::OR);
      setState(209);
      and_test();
      setState(214);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- And_testContext ------------------------------------------------------------------

Python3Parser::And_testContext::And_testContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<Python3Parser::Not_testContext *> Python3Parser::And_testContext::not_test() {
  return getRuleContexts<Python3Parser::Not_testContext>();
}

Python3Parser::Not_testContext* Python3Parser::And_testContext::not_test(size_t i) {
  return getRuleContext<Python3Parser::Not_testContext>(i);
}

std::vector<tree::TerminalNode *> Python3Parser::And_testContext::AND() {
  return getTokens(Python3Parser::AND);
}

tree::TerminalNode* Python3Parser::And_testContext::AND(size_t i) {
  return getToken(Python3Parser::AND, i);
}


size_t Python3Parser::And_testContext::getRuleIndex() const {
  return Python3Parser::RuleAnd_test;
}


std::any Python3Parser::And_testContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitAnd_test(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::And_testContext* Python3Parser::and_test() {
  And_testContext *_localctx = _tracker.createInstance<And_testContext>(_ctx, getState());
  enterRule(_localctx, 42, Python3Parser::RuleAnd_test);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(215);
    not_test();
    setState(220);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == Python3Parser::AND) {
      setState(216);
      match(Python3Parser::AND);
      setState(217);
      not_test();
      setState(222);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Not_testContext ------------------------------------------------------------------

Python3Parser::Not_testContext::Not_testContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Python3Parser::Not_testContext::NOT() {
  return getToken(Python3Parser::NOT, 0);
}

Python3Parser::Not_testContext* Python3Parser::Not_testContext::not_test() {
  return getRuleContext<Python3Parser::Not_testContext>(0);
}

Python3Parser::ComparisonContext* Python3Parser::Not_testContext::comparison() {
  return getRuleContext<Python3Parser::ComparisonContext>(0);
}


size_t Python3Parser::Not_testContext::getRuleIndex() const {
  return Python3Parser::RuleNot_test;
}


std::any Python3Parser::Not_testContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitNot_test(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::Not_testContext* Python3Parser::not_test() {
  Not_testContext *_localctx = _tracker.createInstance<Not_testContext>(_ctx, getState());
  enterRule(_localctx, 44, Python3Parser::RuleNot_test);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(226);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case Python3Parser::NOT: {
        enterOuterAlt(_localctx, 1);
        setState(223);
        match(Python3Parser::NOT);
        setState(224);
        not_test();
        break;
      }

      case Python3Parser::STRING:
      case Python3Parser::NUMBER:
      case Python3Parser::NONE:
      case Python3Parser::TRUE:
      case Python3Parser::FALSE:
      case Python3Parser::NAME:
      case Python3Parser::OPEN_PAREN:
      case Python3Parser::OPEN_BRACK:
      case Python3Parser::ADD:
      case Python3Parser::MINUS:
      case Python3Parser::FORMAT_QUOTATION: {
        enterOuterAlt(_localctx, 2);
        setState(225);
        comparison();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ComparisonContext ------------------------------------------------------------------

Python3Parser::ComparisonContext::ComparisonContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<Python3Parser::Arith_exprContext *> Python3Parser::ComparisonContext::arith_expr() {
  return getRuleContexts<Python3Parser::Arith_exprContext>();
}

Python3Parser::Arith_exprContext* Python3Parser::ComparisonContext::arith_expr(size_t i) {
  return getRuleContext<Python3Parser::Arith_exprContext>(i);
}

std::vector<Python3Parser::Comp_opContext *> Python3Parser::ComparisonContext::comp_op() {
  return getRuleContexts<Python3Parser::Comp_opContext>();
}

Python3Parser::Comp_opContext* Python3Parser::ComparisonContext::comp_op(size_t i) {
  return getRuleContext<Python3Parser::Comp_opContext>(i);
}


size_t Python3Parser::ComparisonContext::getRuleIndex() const {
  return Python3Parser::RuleComparison;
}


std::any Python3Parser::ComparisonContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitComparison(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::ComparisonContext* Python3Parser::comparison() {
  ComparisonContext *_localctx = _tracker.createInstance<ComparisonContext>(_ctx, getState());
  enterRule(_localctx, 46, Python3Parser::RuleComparison);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(228);
    arith_expr();
    setState(234);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 59) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 59)) & 95) != 0)) {
      setState(229);
      comp_op();
      setState(230);
      arith_expr();
      setState(236);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Comp_opContext ------------------------------------------------------------------

Python3Parser::Comp_opContext::Comp_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Python3Parser::Comp_opContext::LESS_THAN() {
  return getToken(Python3Parser::LESS_THAN, 0);
}

tree::TerminalNode* Python3Parser::Comp_opContext::GREATER_THAN() {
  return getToken(Python3Parser::GREATER_THAN, 0);
}

tree::TerminalNode* Python3Parser::Comp_opContext::EQUALS() {
  return getToken(Python3Parser::EQUALS, 0);
}

tree::TerminalNode* Python3Parser::Comp_opContext::GT_EQ() {
  return getToken(Python3Parser::GT_EQ, 0);
}

tree::TerminalNode* Python3Parser::Comp_opContext::LT_EQ() {
  return getToken(Python3Parser::LT_EQ, 0);
}

tree::TerminalNode* Python3Parser::Comp_opContext::NOT_EQ_2() {
  return getToken(Python3Parser::NOT_EQ_2, 0);
}


size_t Python3Parser::Comp_opContext::getRuleIndex() const {
  return Python3Parser::RuleComp_op;
}


std::any Python3Parser::Comp_opContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitComp_op(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::Comp_opContext* Python3Parser::comp_op() {
  Comp_opContext *_localctx = _tracker.createInstance<Comp_opContext>(_ctx, getState());
  enterRule(_localctx, 48, Python3Parser::RuleComp_op);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(237);
    _la = _input->LA(1);
    if (!(((((_la - 59) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 59)) & 95) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Arith_exprContext ------------------------------------------------------------------

Python3Parser::Arith_exprContext::Arith_exprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<Python3Parser::TermContext *> Python3Parser::Arith_exprContext::term() {
  return getRuleContexts<Python3Parser::TermContext>();
}

Python3Parser::TermContext* Python3Parser::Arith_exprContext::term(size_t i) {
  return getRuleContext<Python3Parser::TermContext>(i);
}

std::vector<Python3Parser::Addorsub_opContext *> Python3Parser::Arith_exprContext::addorsub_op() {
  return getRuleContexts<Python3Parser::Addorsub_opContext>();
}

Python3Parser::Addorsub_opContext* Python3Parser::Arith_exprContext::addorsub_op(size_t i) {
  return getRuleContext<Python3Parser::Addorsub_opContext>(i);
}


size_t Python3Parser::Arith_exprContext::getRuleIndex() const {
  return Python3Parser::RuleArith_expr;
}


std::any Python3Parser::Arith_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitArith_expr(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::Arith_exprContext* Python3Parser::arith_expr() {
  Arith_exprContext *_localctx = _tracker.createInstance<Arith_exprContext>(_ctx, getState());
  enterRule(_localctx, 50, Python3Parser::RuleArith_expr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(239);
    term();
    setState(245);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == Python3Parser::ADD

    || _la == Python3Parser::MINUS) {
      setState(240);
      addorsub_op();
      setState(241);
      term();
      setState(247);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Addorsub_opContext ------------------------------------------------------------------

Python3Parser::Addorsub_opContext::Addorsub_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Python3Parser::Addorsub_opContext::ADD() {
  return getToken(Python3Parser::ADD, 0);
}

tree::TerminalNode* Python3Parser::Addorsub_opContext::MINUS() {
  return getToken(Python3Parser::MINUS, 0);
}


size_t Python3Parser::Addorsub_opContext::getRuleIndex() const {
  return Python3Parser::RuleAddorsub_op;
}


std::any Python3Parser::Addorsub_opContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitAddorsub_op(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::Addorsub_opContext* Python3Parser::addorsub_op() {
  Addorsub_opContext *_localctx = _tracker.createInstance<Addorsub_opContext>(_ctx, getState());
  enterRule(_localctx, 52, Python3Parser::RuleAddorsub_op);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(248);
    _la = _input->LA(1);
    if (!(_la == Python3Parser::ADD

    || _la == Python3Parser::MINUS)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TermContext ------------------------------------------------------------------

Python3Parser::TermContext::TermContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<Python3Parser::FactorContext *> Python3Parser::TermContext::factor() {
  return getRuleContexts<Python3Parser::FactorContext>();
}

Python3Parser::FactorContext* Python3Parser::TermContext::factor(size_t i) {
  return getRuleContext<Python3Parser::FactorContext>(i);
}

std::vector<Python3Parser::Muldivmod_opContext *> Python3Parser::TermContext::muldivmod_op() {
  return getRuleContexts<Python3Parser::Muldivmod_opContext>();
}

Python3Parser::Muldivmod_opContext* Python3Parser::TermContext::muldivmod_op(size_t i) {
  return getRuleContext<Python3Parser::Muldivmod_opContext>(i);
}


size_t Python3Parser::TermContext::getRuleIndex() const {
  return Python3Parser::RuleTerm;
}


std::any Python3Parser::TermContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitTerm(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::TermContext* Python3Parser::term() {
  TermContext *_localctx = _tracker.createInstance<TermContext>(_ctx, getState());
  enterRule(_localctx, 54, Python3Parser::RuleTerm);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(250);
    factor();
    setState(256);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 63050463502663680) != 0)) {
      setState(251);
      muldivmod_op();
      setState(252);
      factor();
      setState(258);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Muldivmod_opContext ------------------------------------------------------------------

Python3Parser::Muldivmod_opContext::Muldivmod_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Python3Parser::Muldivmod_opContext::STAR() {
  return getToken(Python3Parser::STAR, 0);
}

tree::TerminalNode* Python3Parser::Muldivmod_opContext::DIV() {
  return getToken(Python3Parser::DIV, 0);
}

tree::TerminalNode* Python3Parser::Muldivmod_opContext::IDIV() {
  return getToken(Python3Parser::IDIV, 0);
}

tree::TerminalNode* Python3Parser::Muldivmod_opContext::MOD() {
  return getToken(Python3Parser::MOD, 0);
}


size_t Python3Parser::Muldivmod_opContext::getRuleIndex() const {
  return Python3Parser::RuleMuldivmod_op;
}


std::any Python3Parser::Muldivmod_opContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitMuldivmod_op(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::Muldivmod_opContext* Python3Parser::muldivmod_op() {
  Muldivmod_opContext *_localctx = _tracker.createInstance<Muldivmod_opContext>(_ctx, getState());
  enterRule(_localctx, 56, Python3Parser::RuleMuldivmod_op);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(259);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 63050463502663680) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FactorContext ------------------------------------------------------------------

Python3Parser::FactorContext::FactorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

Python3Parser::FactorContext* Python3Parser::FactorContext::factor() {
  return getRuleContext<Python3Parser::FactorContext>(0);
}

tree::TerminalNode* Python3Parser::FactorContext::ADD() {
  return getToken(Python3Parser::ADD, 0);
}

tree::TerminalNode* Python3Parser::FactorContext::MINUS() {
  return getToken(Python3Parser::MINUS, 0);
}

Python3Parser::PowerContext* Python3Parser::FactorContext::power() {
  return getRuleContext<Python3Parser::PowerContext>(0);
}


size_t Python3Parser::FactorContext::getRuleIndex() const {
  return Python3Parser::RuleFactor;
}


std::any Python3Parser::FactorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitFactor(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::FactorContext* Python3Parser::factor() {
  FactorContext *_localctx = _tracker.createInstance<FactorContext>(_ctx, getState());
  enterRule(_localctx, 58, Python3Parser::RuleFactor);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(264);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case Python3Parser::ADD:
      case Python3Parser::MINUS: {
        enterOuterAlt(_localctx, 1);
        setState(261);
        _la = _input->LA(1);
        if (!(_la == Python3Parser::ADD

        || _la == Python3Parser::MINUS)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(262);
        factor();
        break;
      }

      case Python3Parser::STRING:
      case Python3Parser::NUMBER:
      case Python3Parser::NONE:
      case Python3Parser::TRUE:
      case Python3Parser::FALSE:
      case Python3Parser::NAME:
      case Python3Parser::OPEN_PAREN:
      case Python3Parser::OPEN_BRACK:
      case Python3Parser::FORMAT_QUOTATION: {
        enterOuterAlt(_localctx, 2);
        setState(263);
        power();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PowerContext ------------------------------------------------------------------

Python3Parser::PowerContext::PowerContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

Python3Parser::Atom_exprContext* Python3Parser::PowerContext::atom_expr() {
  return getRuleContext<Python3Parser::Atom_exprContext>(0);
}

tree::TerminalNode* Python3Parser::PowerContext::POWER() {
  return getToken(Python3Parser::POWER, 0);
}

Python3Parser::FactorContext* Python3Parser::PowerContext::factor() {
  return getRuleContext<Python3Parser::FactorContext>(0);
}


size_t Python3Parser::PowerContext::getRuleIndex() const {
  return Python3Parser::RulePower;
}


std::any Python3Parser::PowerContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitPower(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::PowerContext* Python3Parser::power() {
  PowerContext *_localctx = _tracker.createInstance<PowerContext>(_ctx, getState());
  enterRule(_localctx, 60, Python3Parser::RulePower);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(266);
    atom_expr();
    setState(269);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == Python3Parser::POWER) {
      setState(267);
      match(Python3Parser::POWER);
      setState(268);
      factor();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Atom_exprContext ------------------------------------------------------------------

Python3Parser::Atom_exprContext::Atom_exprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

Python3Parser::AtomContext* Python3Parser::Atom_exprContext::atom() {
  return getRuleContext<Python3Parser::AtomContext>(0);
}

std::vector<Python3Parser::TrailerContext *> Python3Parser::Atom_exprContext::trailer() {
  return getRuleContexts<Python3Parser::TrailerContext>();
}

Python3Parser::TrailerContext* Python3Parser::Atom_exprContext::trailer(size_t i) {
  return getRuleContext<Python3Parser::TrailerContext>(i);
}


size_t Python3Parser::Atom_exprContext::getRuleIndex() const {
  return Python3Parser::RuleAtom_expr;
}


std::any Python3Parser::Atom_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitAtom_expr(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::Atom_exprContext* Python3Parser::atom_expr() {
  Atom_exprContext *_localctx = _tracker.createInstance<Atom_exprContext>(_ctx, getState());
  enterRule(_localctx, 62, Python3Parser::RuleAtom_expr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(271);
    atom();
    setState(275);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == Python3Parser::OPEN_PAREN

    || _la == Python3Parser::OPEN_BRACK) {
      setState(272);
      trailer();
      setState(277);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TrailerContext ------------------------------------------------------------------

Python3Parser::TrailerContext::TrailerContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Python3Parser::TrailerContext::OPEN_PAREN() {
  return getToken(Python3Parser::OPEN_PAREN, 0);
}

tree::TerminalNode* Python3Parser::TrailerContext::CLOSE_PAREN() {
  return getToken(Python3Parser::CLOSE_PAREN, 0);
}

Python3Parser::ArglistContext* Python3Parser::TrailerContext::arglist() {
  return getRuleContext<Python3Parser::ArglistContext>(0);
}

tree::TerminalNode* Python3Parser::TrailerContext::OPEN_BRACK() {
  return getToken(Python3Parser::OPEN_BRACK, 0);
}

Python3Parser::TestContext* Python3Parser::TrailerContext::test() {
  return getRuleContext<Python3Parser::TestContext>(0);
}

tree::TerminalNode* Python3Parser::TrailerContext::CLOSE_BRACK() {
  return getToken(Python3Parser::CLOSE_BRACK, 0);
}


size_t Python3Parser::TrailerContext::getRuleIndex() const {
  return Python3Parser::RuleTrailer;
}


std::any Python3Parser::TrailerContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitTrailer(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::TrailerContext* Python3Parser::trailer() {
  TrailerContext *_localctx = _tracker.createInstance<TrailerContext>(_ctx, getState());
  enterRule(_localctx, 64, Python3Parser::RuleTrailer);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(287);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case Python3Parser::OPEN_PAREN: {
        enterOuterAlt(_localctx, 1);
        setState(278);
        match(Python3Parser::OPEN_PAREN);
        setState(280);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 6773129083813912) != 0) || _la == Python3Parser::FORMAT_QUOTATION) {
          setState(279);
          arglist();
        }
        setState(282);
        match(Python3Parser::CLOSE_PAREN);
        break;
      }

      case Python3Parser::OPEN_BRACK: {
        enterOuterAlt(_localctx, 2);
        setState(283);
        match(Python3Parser::OPEN_BRACK);
        setState(284);
        test();
        setState(285);
        match(Python3Parser::CLOSE_BRACK);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AtomContext ------------------------------------------------------------------

Python3Parser::AtomContext::AtomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Python3Parser::AtomContext::NAME() {
  return getToken(Python3Parser::NAME, 0);
}

tree::TerminalNode* Python3Parser::AtomContext::NUMBER() {
  return getToken(Python3Parser::NUMBER, 0);
}

tree::TerminalNode* Python3Parser::AtomContext::NONE() {
  return getToken(Python3Parser::NONE, 0);
}

tree::TerminalNode* Python3Parser::AtomContext::TRUE() {
  return getToken(Python3Parser::TRUE, 0);
}

tree::TerminalNode* Python3Parser::AtomContext::FALSE() {
  return getToken(Python3Parser::FALSE, 0);
}

Python3Parser::Format_stringContext* Python3Parser::AtomContext::format_string() {
  return getRuleContext<Python3Parser::Format_stringContext>(0);
}

tree::TerminalNode* Python3Parser::AtomContext::OPEN_PAREN() {
  return getToken(Python3Parser::OPEN_PAREN, 0);
}

tree::TerminalNode* Python3Parser::AtomContext::CLOSE_PAREN() {
  return getToken(Python3Parser::CLOSE_PAREN, 0);
}

tree::TerminalNode* Python3Parser::AtomContext::OPEN_BRACK() {
  return getToken(Python3Parser::OPEN_BRACK, 0);
}

tree::TerminalNode* Python3Parser::AtomContext::CLOSE_BRACK() {
  return getToken(Python3Parser::CLOSE_BRACK, 0);
}

std::vector<tree::TerminalNode *> Python3Parser::AtomContext::STRING() {
  return getTokens(Python3Parser::STRING);
}

tree::TerminalNode* Python3Parser::AtomContext::STRING(size_t i) {
  return getToken(Python3Parser::STRING, i);
}

Python3Parser::TestlistContext* Python3Parser::AtomContext::testlist() {
  return getRuleContext<Python3Parser::TestlistContext>(0);
}


size_t Python3Parser::AtomContext::getRuleIndex() const {
  return Python3Parser::RuleAtom;
}


std::any Python3Parser::AtomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitAtom(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::AtomContext* Python3Parser::atom() {
  AtomContext *_localctx = _tracker.createInstance<AtomContext>(_ctx, getState());
  enterRule(_localctx, 66, Python3Parser::RuleAtom);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(310);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case Python3Parser::NAME: {
        setState(289);
        match(Python3Parser::NAME);
        break;
      }

      case Python3Parser::NUMBER: {
        setState(290);
        match(Python3Parser::NUMBER);
        break;
      }

      case Python3Parser::STRING: {
        setState(292); 
        _errHandler->sync(this);
        _la = _input->LA(1);
        do {
          setState(291);
          match(Python3Parser::STRING);
          setState(294); 
          _errHandler->sync(this);
          _la = _input->LA(1);
        } while (_la == Python3Parser::STRING);
        break;
      }

      case Python3Parser::NONE: {
        setState(296);
        match(Python3Parser::NONE);
        break;
      }

      case Python3Parser::TRUE: {
        setState(297);
        match(Python3Parser::TRUE);
        break;
      }

      case Python3Parser::FALSE: {
        setState(298);
        match(Python3Parser::FALSE);
        break;
      }

      case Python3Parser::OPEN_PAREN: {
        setState(299);
        match(Python3Parser::OPEN_PAREN);
        setState(301);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 6773129083813912) != 0) || _la == Python3Parser::FORMAT_QUOTATION) {
          setState(300);
          testlist();
        }
        setState(303);
        match(Python3Parser::CLOSE_PAREN);
        break;
      }

      case Python3Parser::OPEN_BRACK: {
        setState(304);
        match(Python3Parser::OPEN_BRACK);
        setState(306);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 6773129083813912) != 0) || _la == Python3Parser::FORMAT_QUOTATION) {
          setState(305);
          testlist();
        }
        setState(308);
        match(Python3Parser::CLOSE_BRACK);
        break;
      }

      case Python3Parser::FORMAT_QUOTATION: {
        setState(309);
        format_string();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Format_stringContext ------------------------------------------------------------------

Python3Parser::Format_stringContext::Format_stringContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Python3Parser::Format_stringContext::FORMAT_QUOTATION() {
  return getToken(Python3Parser::FORMAT_QUOTATION, 0);
}

tree::TerminalNode* Python3Parser::Format_stringContext::QUOTATION() {
  return getToken(Python3Parser::QUOTATION, 0);
}

std::vector<tree::TerminalNode *> Python3Parser::Format_stringContext::FORMAT_STRING_LITERAL() {
  return getTokens(Python3Parser::FORMAT_STRING_LITERAL);
}

tree::TerminalNode* Python3Parser::Format_stringContext::FORMAT_STRING_LITERAL(size_t i) {
  return getToken(Python3Parser::FORMAT_STRING_LITERAL, i);
}

std::vector<tree::TerminalNode *> Python3Parser::Format_stringContext::OPEN_BRACE() {
  return getTokens(Python3Parser::OPEN_BRACE);
}

tree::TerminalNode* Python3Parser::Format_stringContext::OPEN_BRACE(size_t i) {
  return getToken(Python3Parser::OPEN_BRACE, i);
}

std::vector<Python3Parser::TestlistContext *> Python3Parser::Format_stringContext::testlist() {
  return getRuleContexts<Python3Parser::TestlistContext>();
}

Python3Parser::TestlistContext* Python3Parser::Format_stringContext::testlist(size_t i) {
  return getRuleContext<Python3Parser::TestlistContext>(i);
}

std::vector<tree::TerminalNode *> Python3Parser::Format_stringContext::CLOSE_BRACE() {
  return getTokens(Python3Parser::CLOSE_BRACE);
}

tree::TerminalNode* Python3Parser::Format_stringContext::CLOSE_BRACE(size_t i) {
  return getToken(Python3Parser::CLOSE_BRACE, i);
}


size_t Python3Parser::Format_stringContext::getRuleIndex() const {
  return Python3Parser::RuleFormat_string;
}


std::any Python3Parser::Format_stringContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitFormat_string(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::Format_stringContext* Python3Parser::format_string() {
  Format_stringContext *_localctx = _tracker.createInstance<Format_stringContext>(_ctx, getState());
  enterRule(_localctx, 68, Python3Parser::RuleFormat_string);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(312);
    match(Python3Parser::FORMAT_QUOTATION);
    setState(320);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == Python3Parser::FORMAT_STRING_LITERAL

    || _la == Python3Parser::OPEN_BRACE) {
      setState(318);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case Python3Parser::FORMAT_STRING_LITERAL: {
          setState(313);
          match(Python3Parser::FORMAT_STRING_LITERAL);
          break;
        }

        case Python3Parser::OPEN_BRACE: {
          setState(314);
          match(Python3Parser::OPEN_BRACE);
          setState(315);
          testlist();
          setState(316);
          match(Python3Parser::CLOSE_BRACE);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(322);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(323);
    match(Python3Parser::QUOTATION);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TestlistContext ------------------------------------------------------------------

Python3Parser::TestlistContext::TestlistContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<Python3Parser::TestContext *> Python3Parser::TestlistContext::test() {
  return getRuleContexts<Python3Parser::TestContext>();
}

Python3Parser::TestContext* Python3Parser::TestlistContext::test(size_t i) {
  return getRuleContext<Python3Parser::TestContext>(i);
}

std::vector<tree::TerminalNode *> Python3Parser::TestlistContext::COMMA() {
  return getTokens(Python3Parser::COMMA);
}

tree::TerminalNode* Python3Parser::TestlistContext::COMMA(size_t i) {
  return getToken(Python3Parser::COMMA, i);
}


size_t Python3Parser::TestlistContext::getRuleIndex() const {
  return Python3Parser::RuleTestlist;
}


std::any Python3Parser::TestlistContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitTestlist(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::TestlistContext* Python3Parser::testlist() {
  TestlistContext *_localctx = _tracker.createInstance<TestlistContext>(_ctx, getState());
  enterRule(_localctx, 70, Python3Parser::RuleTestlist);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(325);
    test();
    setState(330);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(326);
        match(Python3Parser::COMMA);
        setState(327);
        test(); 
      }
      setState(332);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx);
    }
    setState(334);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == Python3Parser::COMMA) {
      setState(333);
      match(Python3Parser::COMMA);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArglistContext ------------------------------------------------------------------

Python3Parser::ArglistContext::ArglistContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<Python3Parser::ArgumentContext *> Python3Parser::ArglistContext::argument() {
  return getRuleContexts<Python3Parser::ArgumentContext>();
}

Python3Parser::ArgumentContext* Python3Parser::ArglistContext::argument(size_t i) {
  return getRuleContext<Python3Parser::ArgumentContext>(i);
}

std::vector<tree::TerminalNode *> Python3Parser::ArglistContext::COMMA() {
  return getTokens(Python3Parser::COMMA);
}

tree::TerminalNode* Python3Parser::ArglistContext::COMMA(size_t i) {
  return getToken(Python3Parser::COMMA, i);
}


size_t Python3Parser::ArglistContext::getRuleIndex() const {
  return Python3Parser::RuleArglist;
}


std::any Python3Parser::ArglistContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitArglist(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::ArglistContext* Python3Parser::arglist() {
  ArglistContext *_localctx = _tracker.createInstance<ArglistContext>(_ctx, getState());
  enterRule(_localctx, 72, Python3Parser::RuleArglist);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(336);
    argument();
    setState(341);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(337);
        match(Python3Parser::COMMA);
        setState(338);
        argument(); 
      }
      setState(343);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx);
    }
    setState(345);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == Python3Parser::COMMA) {
      setState(344);
      match(Python3Parser::COMMA);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArgumentContext ------------------------------------------------------------------

Python3Parser::ArgumentContext::ArgumentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<Python3Parser::TestContext *> Python3Parser::ArgumentContext::test() {
  return getRuleContexts<Python3Parser::TestContext>();
}

Python3Parser::TestContext* Python3Parser::ArgumentContext::test(size_t i) {
  return getRuleContext<Python3Parser::TestContext>(i);
}

tree::TerminalNode* Python3Parser::ArgumentContext::ASSIGN() {
  return getToken(Python3Parser::ASSIGN, 0);
}


size_t Python3Parser::ArgumentContext::getRuleIndex() const {
  return Python3Parser::RuleArgument;
}


std::any Python3Parser::ArgumentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<Python3ParserVisitor*>(visitor))
    return parserVisitor->visitArgument(this);
  else
    return visitor->visitChildren(this);
}

Python3Parser::ArgumentContext* Python3Parser::argument() {
  ArgumentContext *_localctx = _tracker.createInstance<ArgumentContext>(_ctx, getState());
  enterRule(_localctx, 74, Python3Parser::RuleArgument);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(352);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 39, _ctx)) {
    case 1: {
      setState(347);
      test();
      break;
    }

    case 2: {
      setState(348);
      test();
      setState(349);
      match(Python3Parser::ASSIGN);
      setState(350);
      test();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

void Python3Parser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  python3parserParserInitialize();
#else
  ::antlr4::internal::call_once(python3parserParserOnceFlag, python3parserParserInitialize);
#endif
}
