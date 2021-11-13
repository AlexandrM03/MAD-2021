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
#include "SemAnalysis.h"

void check_syntax(Lex::LEX lex, Log::LOG log);

int wmain(int argc, wchar_t* argv[]) {
    setlocale(LC_ALL, "RUS");

    Log::LOG log = Log::INITLOG;

    try {
        Parm::PARM parm = Parm::getparm(argc, argv);
        log = Log::getlog(parm.log);
        Log::WriteLog(log);
        Log::WriteParm(log, parm);
        In::IN in = In::getin(parm);
        Log::WriteIn(log, in);
        Lex::LEX lex = Lex::LexAnaliz(log, in);
        check_syntax(lex, log);
        Semantic::Analyze(lex, log);
        Polish::startPolish(lex);
        Lex::Synchronization(lex);

        IT::ShowTable(lex.idtable, *log.stream);
        Log::WriteLexTableLog(lex.lextable, log);
        LT::ShowTable(lex.lextable, *log.stream);
        Log::Close(log);
    }
    catch (Error::ERROR e) {
        Log::WriteError(log, e);
    }
}

void check_syntax(Lex::LEX lex, Log::LOG log) {
    MFST::Mfst mfst(lex.lextable, GRB::getGreibach());
    mfst.start(*log.stream);
    mfst.savededucation();
    mfst.printrules(*log.stream);
}