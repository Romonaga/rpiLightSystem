
#include <signal.h>
#include <unistd.h>
#include <getopt.h>

#include <QCoreApplication>
#include "lightsystem.h"



LightSystem _lightSystem;
bool _forkProcess = true;


#include <initializer_list>
#include <signal.h>
#include <unistd.h>

void ignoreUnixSignals(std::initializer_list<int> ignoreSignals)
{
    // all these signals will be ignored.
    for (int sig : ignoreSignals)
        signal(sig, SIG_IGN);
}

void catchUnixSignals(std::initializer_list<int> quitSignals) {
    auto handler = [](int sig) -> void
    {
        // blocking and not aysnc-signal-safe func are valid
        printf("\nquit the application by signal(%d).\n", sig);
        _lightSystem.stopSystem();
        QCoreApplication::quit();
    };

    sigset_t blocking_mask;
    sigemptyset(&blocking_mask);
    for (auto sig : quitSignals)
        sigaddset(&blocking_mask, sig);

    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_mask    = blocking_mask;
    sa.sa_flags   = 0;

    for (auto sig : quitSignals)
        sigaction(sig, &sa, nullptr);
}



void parseargs(int argc, char **argv)
{
    int index;
    int c;

    static struct option longopts[] =
    {
        {"help", no_argument, 0, 'h'},
        {"nofork", no_argument, 0, 'n'},
        {0, 0, 0, 0}
    };

    while (1)
    {

        index = 0;
        c = getopt_long(argc, argv, "nh", longopts, &index);

        if (c == -1)
            break;

        switch (c)
        {
        case 0:
            /* handle flag options (array's 3rd field non-0) */
            break;

        case 'h':
            fprintf(stderr, "%s version %s\n", argv[0], "0.1");
            fprintf(stderr, "Usage: %s \n"
                "-h (--help)    - this information\n"
                "-n (--nofork)    - Do Not Fork Process\n"
                , argv[0]);
            exit(-1);


        case 'n':
            _forkProcess = false;
            break;


        case '?':
            fprintf(stderr,"Seems we got something wrong (?)");
            /* getopt_long already reported error? */
            exit(-1);

        default:
            fprintf(stderr,"Seems we did not expect this (%c)", c);

            exit(-1);
        }
    }
}



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    parseargs(argc, argv);
    catchUnixSignals({SIGQUIT, SIGINT, SIGTERM, SIGHUP});
    if(_forkProcess)
    {
        int pid = 0;
        pid = fork();
        if(0 != pid)
        {
            return 0;
        }
    }
    else
    {
        fprintf(stderr,"Not Forking\r\n");
    }

    _lightSystem.startSystem();
    return a.exec();
}
