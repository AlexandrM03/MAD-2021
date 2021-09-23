#include"stdafx.h"
#include"Error.h"
#include"Parm.h"
#include"In.h"
#include"Log.h"
#include"Out.h"
#include"FST.h"
#include"LexAnalisys.h"

int wmain(int argc, wchar_t* argv[]) {
    setlocale(LC_ALL, "RUS");

    Log::LOG log = Log::INITLOG;
    /*Out::OUT out = Out::INITOUT;
    try {
        Parm::PARM parm = Parm::getparm(argc, argv);
        log = Log::getlog(parm.log);
        In::IN in = In::getin(parm.in);
        Log::WriteLine(log, "Тест: ", "без ошибок ", "");
        Log::WriteLog(log);
        Log::WriteParm(log, parm);
        Log::WriteIn(log, in);
        Log::Close(log);
        out = Out::getout(parm.out);
        Out::WriteOut(out, in);
        Out::Close;
        std::cout << "Successfully ended!";
    }
    catch (Error::ERROR e) {
        Log::WriteError(log, e);
    }*/

    try {
        Parm::PARM parm = Parm::getparm(argc, argv);
        log = Log::getlog(parm.log);
        Log::WriteLine(log, "Тест: ", "без ошибок ", "");
        Log::WriteLog(log);
        Log::WriteParm(log, parm);
        In::IN in = In::getin(parm);
        Log::WriteIn(log, in);
        Lex::LEX lex = Lex::LexAnaliz(log, in);
        Log::WriteLexTableLog(lex.lextable, log);
        IT::ShowTable(lex.idtable);
        LT::ShowTable(lex.lextable, parm);
        Log::Close(log);
    }
    catch (Error::ERROR e) {
        Log::WriteError(log, e);
    }
}