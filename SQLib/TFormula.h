#pragma once
#include <fstream>
#include <iostream>
#include <initializer_list>
#include <string>
#include <map>
#include <cctype>
#include <algorithm>
#include <sstream>

#include "TError.hpp"
#include "TStack.h"

class TFormula {
private:
    std::string infix;
    std::string postfix;
    bool isConverted;
    std::map<char, int> priority = {
        {'(', 0},
        {')', 0},
        {'+', 1},
        {'-', 1},
        {'*', 2},
        {'/', 2}
    };

    bool IsOperator(char op) const;
    void ProcessOperator(char op, TStack<char>& opStack);
    double PerformOperation(double a, double b, char op);
    void CheckBrackets() const;
    void ValidateNumber(const std::string& number, size_t start_position) const;

public:
    TFormula();
    TFormula(const std::string& other);
                       
    void ConvertToPostfix();                    
    double Calculate();                         

    void SetExpression(const std::string& other);
    std::string GetInfixForm() const;
    std::string GetPostfixForm() const;
    bool IsConverted() const;
};

bool TFormula::IsOperator(char op) const {
    return ( op == '+' || op == '-' || op == '*' || op == '/' );
}

double TFormula::PerformOperation(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0.0) THROW_ERROR( "Division by zero" );
            return a / b;
        default:
            THROW_ERROR( "Unknown operator: " + std::string(1, op) );
    }
}

void TFormula::ProcessOperator(char op, TStack<char>& opStack) {
    while ( !opStack.IsEmpty() && priority.at(opStack.GetTopElem()) >= priority.at(op) && opStack.GetTopElem() != '(' ) {
        postfix += opStack.Get();
        postfix += ' ';
    }
    opStack.Put(op);
}

void TFormula::ValidateNumber(const std::string& number, size_t start_position) const {
    if (number.empty()) {
        THROW_ERROR("Empty number at position: " + std::to_string(start_position));
    }
    
    if (number[0] == '.') {
        THROW_ERROR("Number cannot start with dot at position: " + std::to_string(start_position));
    }
    
    if (number.back() == '.') {
        THROW_ERROR("Number cannot end with dot at position: " + std::to_string(start_position + number.length() - 1));
    }

    if (number.length() > 1 && number[0] == '0' && number[1] != '.') {
        THROW_ERROR("Number cannot start with 0 (except 0.x) at position: " + std::to_string(start_position));
    }
    
    size_t dot_count = std::count(number.begin(), number.end(), '.');
    if (dot_count > 1) {
        THROW_ERROR("Multiple dots in number at position: " + std::to_string(start_position));
    }
    
    for (size_t i = 0; i < number.length(); i++) {
        if (!std::isdigit(number[i]) && number[i] != '.') {
            THROW_ERROR("Invalid character in number at position: " + std::to_string(start_position + i));
        }
    }
}

TFormula::TFormula() 
    : infix(""), postfix(""), isConverted(false) {}

TFormula::TFormula(const std::string& other) 
    : infix(other), postfix(""), isConverted(false) {}

void TFormula::CheckBrackets() const
{
    TStack<char> bracket_stack(infix.length());
    
    for (size_t i = 0; i < infix.length(); i++) {
        char current = infix[i];
        
        if (current == '(') bracket_stack.Put(current);
        else if (current == ')') {
            if (bracket_stack.IsEmpty()) {
                THROW_ERROR( "Unmatched closing bracket at position: " + std::to_string(i) );
            }
            bracket_stack.Get();
        }
    }
    if (!bracket_stack.IsEmpty()) THROW_ERROR( "Unmatched opening brackets in expression" );
}

void TFormula::ConvertToPostfix()
{
    if (isConverted) return;
    
    CheckBrackets();
    TStack<char> op_stack(infix.length());
    postfix = "";
    
    for (size_t i = 0; i < infix.length(); i++) {
        char current = infix[i];
        
        if (current == ' ') continue;
        
        if (std::isdigit(current) || current == '.') {
            std::string number;
            bool hasDot = false;
            size_t number_start = i;
            
            while (i < infix.length() && (std::isdigit(infix[i]) || infix[i] == '.')) {
                number += infix[i++];
            }

            i--; 
            
            ValidateNumber(number, number_start);
            
            postfix += number + " ";
            continue;
        }
        else if (current == '(') {
            op_stack.Put(current);
        }
        else if (current == ')') {
            while (!op_stack.IsEmpty() && op_stack.GetTopElem() != '(') {
                postfix += op_stack.Get();
                postfix += ' ';
            }
            if (op_stack.IsEmpty()) THROW_ERROR("Unmatched closing bracket");

            op_stack.Get();
        }
        else if (IsOperator(current)) ProcessOperator(current, op_stack);
        else {
            std::string error_msg = "Invalid character: '";
            error_msg += current;
            error_msg += "' at position: " + std::to_string(i);
            THROW_ERROR(error_msg);
        }
    }
    
    while (!op_stack.IsEmpty()) {
        postfix += op_stack.Get();
        postfix += ' ';
    }
    
    if (!postfix.empty() && postfix.back() == ' ') {
        postfix.pop_back();
    }
    
    isConverted = true;
}