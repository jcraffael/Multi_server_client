#include "include/mes_process.hpp"

mes_process::mes_process(char *buff, bool rec_buf)
{
    new_message = new mes_buf();
    if(rec_buf)
    {
        memcpy(new_message->act, buff, BUF_ACT);
        memcpy(new_message->cont, buff + BUF_ACT, BUF_CONT);
        memcpy(new_message->val, buff + BUF_ACT + BUF_CONT, BUF_VAL);
    }
    else
    {
        sscanf(buff, "%s %s", new_message->act, new_message->cont);
        memcpy(new_message->val, buff+strlen(new_message->act)+strlen(new_message->cont)+2, BUF_VAL);
    }
}

mes_process::~mes_process()
{
    if(new_message)
        delete new_message;
}

mes_process::mes_process(int argc, char** argv, int *block)
{
    new_message = new mes_buf();
    if(argc == 1)
    {
        //perror("Missing argument!");
        throw RC_MISS_ARGS;
    }
    auto construct_mbuf = [](mes_buf *buf, const char *str, char *arg)
    {
        memcpy(buf->act, str, BUF_ACT);
        memcpy(buf->cont, arg, BUF_CONT);
    };
    
    int opt;
    while ((opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) != -1)
    {
        switch (opt)
        {
        case 'l':
            construct_mbuf(new_message, "LOAD", optarg);
            break;
        case 'g':
            construct_mbuf(new_message, "GET", optarg);
            break;
        case 's':
            construct_mbuf(new_message, "SET", optarg);
            if(argc > 3)
                memcpy(new_message->val, argv[3], BUF_VAL);
            break;
        case 'b':
            *block = 1;
            break;
        case 'v':
            TRACE_LEVEL = 3;
            break;
        case 'h': // -h or --help
        case '?': // Unrecognized option
        default:
            PrintHelp();
            throw RC_MISS_ARGS;
        }
    }
    //return mbuf;
}

rep_process::rep_process(char *buff)
{
    new_rep = new rep_buf();
    memcpy(new_rep, buff, sizeof(rep_buf));
}

rep_process::~rep_process()
{
    if(new_rep)
        delete new_rep;
}

void rep_process::set_code_val(string action, string key, string value)
{
    new_rep = new rep_buf();
    string k_value;
    switch(action.c_str()[0])
    {
        case 'L':
            if(new_rep->res = load_resource(key))
                traceEvent(TRACE_LEVEL, TRACE_LEVEL_ERROR, INFO, "Resource file open error ...");
            else
                traceEvent(TRACE_LEVEL, TRACE_LEVEL_NORMAL, INFO, "Loading file in %s", key.c_str());
            break;
        case 'G':
            new_rep->res = get_value(key, k_value);
            if(new_rep->res == 4)
                traceEvent(TRACE_LEVEL, TRACE_LEVEL_ERROR, INFO, "Resource file not loaded error ...");
            else if (new_rep->res == 3)
                traceEvent(TRACE_LEVEL, TRACE_LEVEL_ERROR, INFO, "Missing key ...");
            else if (new_rep->res == 255)
                traceEvent(TRACE_LEVEL, TRACE_LEVEL_ERROR, INFO, "No file available ...");
            else
                traceEvent(TRACE_LEVEL, TRACE_LEVEL_NORMAL, INFO, "Geting value from key %s", key.c_str());
            break;
        case 'S':
            new_rep->res = set_value(key, value);
            if(new_rep->res == 4)
                traceEvent(TRACE_LEVEL, TRACE_LEVEL_ERROR, INFO, "Resource file not loaded error ...");
            else if (new_rep->res == 255)
                traceEvent(TRACE_LEVEL, TRACE_LEVEL_ERROR, INFO, "No file available ...");
            else
                traceEvent(TRACE_LEVEL, TRACE_LEVEL_NORMAL, INFO, "Setting value %s for key %s", value.c_str(), key.c_str());
            break;
        default:
            new_rep->res = RC_GENERIC;        
            traceEvent(TRACE_LEVEL, TRACE_LEVEL_ERROR, INFO, "Incorrect action.");
    }
    if(k_value.size())
        memcpy(new_rep->k_val, k_value.c_str(), BUF_VAL);

}