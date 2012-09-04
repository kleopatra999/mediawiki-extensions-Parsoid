#include <vector>
#include <iostream>
#include "tokenizer.hpp"

using namespace std;
// Include the generated tokenizer source, namespaced to parsoid::WTTokenizer
namespace parsoid {
    // Wrap the tokenizer in its own sub-namespace

    namespace WTTokenizer {
        #include "wikitext_tokenizer.cpp"
    }

    WikiTokenizer::WikiTokenizer( const string& input ) {
        // TODO: check if the tokenizer modifies the string!
        WTTokenizer::yycontext* ctx = new WTTokenizer::yycontext;
        _ctx = (void*)ctx;
        memset(ctx, 0, sizeof(WTTokenizer::yycontext));

        ctx->buf = const_cast<char*>(input.c_str());
        // Setting the buflen to something larger than the actual length + 512
        // prevents realloc calls
        ctx->buflen = input.size() + 513;
        // The limit sets the boundary for the tokenizer
        ctx->limit = input.size() + 1;
        // The yytext buffer
        ctx->textlen= 1024;
        ctx->text= (char *)malloc(ctx->textlen);
        // Backtracking thunks
        ctx->thunkslen= 32;
        ctx->thunks= (WTTokenizer::yythunk *) 
            malloc( sizeof(WTTokenizer::yythunk) * ctx->thunkslen);
        // Semantic result type stack
        ctx->valslen= 32;
        ctx->vals= (YYSTYPE *) new YYSTYPE[ctx->valslen];
    }

    int WikiTokenizer::tokenize() {
        WTTokenizer::yycontext* ctx = (WTTokenizer::yycontext*) _ctx;
        // Init the accumulator stack
        cout << "tokenize(): " << endl;
        //cout << ctx->buf << endl;
        //WTTokenizer::accumStack = { vector<Tk>() };
        // Parse a single toplevel block per call, and remember the source
        // position
        WTTokenizer::yyparse(ctx);
        cout << "tokenize(): " << endl;
        return 0; // FIXME: return vector<Tk>
    }

    WikiTokenizer::~WikiTokenizer() {
        WTTokenizer::yycontext* ctx = (WTTokenizer::yycontext*) _ctx;
        free(ctx->thunks);
        free(ctx->text);
        delete[] ctx->vals;
        delete ctx;
    }

}
        
