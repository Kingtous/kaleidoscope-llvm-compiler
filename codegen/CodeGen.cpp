//
// Created by kingtous on 2021/1/31.
//

#include "CodeGen.h"

void CodeGenContext::push_block(BasicBlock *block) {
    auto bb = new CodeGenBlock();
    bb->block = block;
    if (blocks == nullptr) {
        blocks = new std::vector<CodeGenBlock *>();
    }
    this->blocks->push_back(bb);

}

CodeGenBlock *CodeGenContext::pop_block() {
    CodeGenBlock *top = (*this->blocks)[this->blocks->size() - 1];
    this->blocks->pop_back();
    return top;
}

CodeGenBlock *CodeGenContext::get_current_locals() {
    if (blocks == nullptr || blocks->empty()) {
        return nullptr;
    } else {
        return (*this->blocks)[this->blocks->size() - 1];
    }
}

void CodeGenContext::setFunction(Function *func) {
    function = func;
}

Function *CodeGenContext::getFunc() {
    return function;
}

void CodeGenContext::removeFunction() {
    function = nullptr;
}

Value *CodeGenContext::findValue(std::string &name) {
    if (blocks->empty()) {
        return nullptr;
    }
    auto it = blocks->rbegin();
    for (; it != blocks->rend(); it++) {
        auto itt = (*it)->localVars.find(name);
        if (itt != (*it)->localVars.end()) {
            return (*itt).second;
        }
    }
    auto gv = TheModule->getGlobalVariable(name);
    return gv;
}

CodeGenBlock *CodeGenContext::findTopCodeGenBlockTypeBlock(CodeGenBlockContextType type) {
    if (blocks->empty()) {
        return nullptr;
    }
    for (auto i = blocks->rbegin(); i != blocks->rend(); ++i) {
        if ((*i)->contextType == type) {
            return (*i);
        }
    }
    return nullptr;
}

CodeGenBlock *CodeGenContext::findTopLoopCodeGenBlockTypeBlock() {
    if (blocks->empty()) {
        return nullptr;
    }
    for (auto i = blocks->rbegin(); i != blocks->rend(); ++i) {
        if (isLoopCodeGenBlockContextType((*i)->contextType)) {
            return (*i);
        }
    }
    return nullptr;
}

BasicBlock *CodeGenContext::getRetBb() const {
    return retBB;
}

void CodeGenContext::setRetBb(BasicBlock *retBb) {
    retBB = retBb;
}

Value *CodeGenContext::getRetV() const {
    return retV;
}

void CodeGenContext::setRetV(Value *retV) {
    CodeGenContext::retV = retV;
}

bool isLoopCodeGenBlockContextType(CodeGenBlockContextType type) {
    return (type == CodeGenBlockContextType::FOR)
           || type == CodeGenBlockContextType::WHILE;
}

void CodeGenBlock::setContextType(CodeGenBlockContextType contextType) {
    CodeGenBlock::contextType = contextType;
}

void CodeGenContext::reset(){
    if (blocks != nullptr){
        blocks->clear();
    }
    setIsAborted(false);
    function = nullptr;
    retBB = nullptr;
    retV = nullptr;
}

bool CodeGenContext::isAborted() const {
    return is_aborted;
}

void CodeGenContext::setIsAborted(bool isAborted) {
    is_aborted = isAborted;
}

ForCodeGenBlockContext::ForCodeGenBlockContext(BasicBlock *bbStart, BasicBlock *bbCond, BasicBlock *bbStep,
                                               BasicBlock *bbBody, BasicBlock *bbEndFor) : bbStart(bbStart),
                                                                                           bbCond(bbCond),
                                                                                           bbStep(bbStep),
                                                                                           bbBody(bbBody),
                                                                                           bbEndFor(bbEndFor) {}

WhileCodeGenBlockContext::WhileCodeGenBlockContext(BasicBlock *bbCond, BasicBlock *bbBody, BasicBlock *bbEndWhile)
        : bbCond(bbCond), bbBody(bbBody), bbEndWhile(bbEndWhile) {}

IfCodeGenBlockContext::IfCodeGenBlockContext(BasicBlock *bbCond, BasicBlock *bbEndIf, BasicBlock *bbTrue,
                                             BasicBlock *bbElse) : bbCond(bbCond), bbEndIf(bbEndIf), bbTrue(bbTrue),
                                                                   bbElse(bbElse) {}
