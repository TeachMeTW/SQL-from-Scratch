#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H
#include "../../includes/stack/MyStack.h"
#include "../../includes/queue/MyQueue.h"
#include "../../includes/token/token.h"
#include "../../includes/token/child_tokens.h"
#include "../../includes/table/table.h"
using namespace std;

namespace Shunting_Yard {
    Queue<Token*> convertpostfix(vector<string> infix) {

        // New stack and queue
        Stack<Token*> OperatorStack;
        Queue<Token*> Outputs;
        // Cycle through vector
        for (int i = 0; i < infix.size(); i++) {
            // get tokens from parsertoken
            Token* t = Token::parsertoken(infix[i]);
            // cout << t->token_str() << endl;
            // check token type 
            auto copy = OperatorStack; // debugging
            // check the enum type
            if (t->type() == (int)Token::Type::Operator) {
                while (!OperatorStack.empty() && OperatorStack.top()->type() == (int)Token::Type::Operator) {
                    // precidence
                    Operators* preop = (Operators*)OperatorStack.top();
                    Operators* current = (Operators*)t; 
                    // Takes the front and current to compare

                    // cout << preop->token_str() << " " << current ->token_str() << endl;

                    // set the precedence here if needed
                    if (current->token_str() == "or") {
                        current->set_prec(2);
                    }
                    if (current->token_str() == "and") {
                        current->set_prec(1);
                    }
                    if (preop->token_str() == "or") {
                        preop->set_prec(2);
                    }
                    if (preop->token_str() == "and") {
                        preop->set_prec(1);
                    }
                    // check if logical v relational
                    if (preop->get_prec() < current ->get_prec()) {
                        Outputs.push(OperatorStack.pop());
                        continue;
                    }
                    // cout << endl;
                    
                    // while (!copy.empty()) {
                    //     cout << copy.pop()->token_str() << endl;
                    // }
                    break;
                    
                }
                // push to stack
                OperatorStack.push(t);
            }
            else if (t->type() == (int)Token::Type::Lparen) {
                OperatorStack.push(t);
            }
            else if (t->type() == (int)Token::Type::Rparen) {
                // if stack not empty and not left paren pop
                while (!OperatorStack.empty() && OperatorStack.top()->type() != (int)Token::Type::Lparen) {
                    Outputs.push(OperatorStack.pop());
                }
                OperatorStack.pop();
                
            }
            else {
                Outputs.push(t);
            }
        }
        //cout << OperatorStack << endl;

        
        while (!OperatorStack.empty()) {
            Outputs.push(OperatorStack.pop());
            //cout << "Popping" << endl;
            //cout << OperatorStack.size() << endl;
        }
        // cout << endl;
        // auto copy = Outputs;
        // while (!copy.empty()) {
        //     cout << copy.pop()->token_str() << endl;
        // }
        return Outputs;
    }
}





#endif //SHUNTING_YARD_H