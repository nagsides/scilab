/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "AnalysisVisitor.hxx"
#include "calls/ArgnAnalyzer.hxx"
#include "tools.hxx"

namespace analysis
{
bool ArgnAnalyzer::analyze(AnalysisVisitor & visitor, const unsigned int lhs, ast::CallExp & e)
{
    if (lhs > 2)
    {
        return false;
    }

    TIType type(visitor.getGVN(), TIType::DOUBLEUINT);
    FunctionBlock * fblock = visitor.getDM().topFunction();
    if (!fblock)
    {
        if (lhs == 1)
        {
            Result & res = e.getDecorator().setResult(type);
            res.getConstant().set(0.);
            e.getDecorator().setCall(Call(Call::IDENTITY, type, L"argn"));
            visitor.setResult(res);
            return true;
        }
        return false;
    }


    enum Kind 
    {
        LHS, RHS, LHSRHS
    } kind;
    const ast::exps_t args = e.getArgs();
    switch (args.size())
    {
        case 0:
        {
            if (lhs == 1)
            {
                kind = LHS;
            }
            else if (lhs == 2)
            {
                kind = LHSRHS;
            }
            break;
        }
        case 1:
        {
            ast::Exp * first = args.front();
            if (first && first->isDoubleExp())
            {
                const double arg1 = static_cast<ast::DoubleExp *>(first)->getValue();
                if (arg1 == 0)
                {
                    if (lhs == 1)
                    {
                        kind = LHS;
                    }
                    else if (lhs == 2)
                    {
                        kind = LHSRHS;
                    }
                }
                else if (arg1 == 1)
                {
                    kind = LHS;
                }
                else if (arg1 == 2)
                {
                    kind = RHS;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
            break;
        }
        default:
            return false;
    }

    switch (kind)
    {
        case LHS:
        case RHS:
        {
            const unsigned int val = kind == LHS ? fblock->getLHS() : fblock->getRHS();
            Result & res = e.getDecorator().setResult(type);
            res.getConstant().set(val);
            e.getDecorator().setCall(Call(Call::IDENTITY, type, L"argn"));
            visitor.setResult(res);
        }
        case LHSRHS:
        {
            std::vector<Result> & mlhs = visitor.getLHSContainer();
            mlhs.clear();
            mlhs.reserve(2);

            const unsigned int flhs = fblock->getLHS();
            const unsigned int frhs = fblock->getRHS();
            mlhs.emplace_back(type);
            mlhs.back().getConstant().set((double)flhs);
            mlhs.emplace_back(type);
            mlhs.back().getConstant().set((double)frhs);

            e.getDecorator().setCall(Call(Call::IDENTITY, type, L"argn"));
        }
    }

    return true;
}
}