#include "stdafx.h"
#include "Error.h"
#include "Parm.h"
#include "In.h"
#include "Log.h"
#include "Out.h"
#include "FST.h"
#include "Polish.h"
#include "LexAnalisys.h"
#include "MFST.h"
#include "GRB.h"

int wmain(int argc, wchar_t* argv[]) {
    setlocale(LC_ALL, "RUS");

    Log::LOG log = Log::INITLOG;

    try {
        Parm::PARM parm = Parm::getparm(argc, argv);
        log = Log::getlog(parm.log);
        Log::WriteLine(log, "Тест: ", "без ошибок ", "");
        Log::WriteLog(log);
        Log::WriteParm(log, parm);
        In::IN in = In::getin(parm);
        Log::WriteIn(log, in);
        Lex::LEX lex = Lex::LexAnaliz(log, in);
        IT::ShowTable(lex.idtable);
        Polish::startPolish(lex);
        Log::WriteLexTableLog(lex.lextable, log);
        LT::ShowTable(lex.lextable, parm);
        /*MFST_TRACE_START
        unsigned int start_time = clock();
        MFST::Mfst mfst(lex.lextable, GRB::getGreibach());
        mfst.start();
        unsigned int end_time = clock();
        unsigned int search_time = end_time - start_time;
        std::cout << search_time << std::endl;
        mfst.savededucation();
        mfst.printrules();*/
        Log::Close(log);
    }
    catch (Error::ERROR e) {
        Log::WriteError(log, e);
    }
}