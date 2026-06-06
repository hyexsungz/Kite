#include <stdio.h>
#include <string.h>

#include "include/kite_sys.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("KiteVCS\n");
        printf("usage: kite <command> [args]\n");
        return 0;
    }

    kite_init();

    const char *cmd = argv[1];

    if (strcmp(cmd, "init") == 0)
    {
        return kite_cmd_init(argc, argv);
    }
    else if (strcmp(cmd, "add") == 0)
    {
        return kite_cmd_add(argc, argv);
    }
    else if (strcmp(cmd, "commit") == 0)
    {
        return kite_cmd_commit(argc, argv);
    }
    else if (strcmp(cmd, "status") == 0)
    {
        return kite_cmd_status(argc, argv);
    }
    else if (strcmp(cmd, "diff") == 0)
    {
        return kite_cmd_diff(argc, argv);
    }
    else if (strcmp(cmd, "log") == 0)
    {
        return kite_cmd_log(argc, argv);
    }
    else if (strcmp(cmd, "help") == 0)
    {
        printf("init\nadd\ncommit\nstatus\ndiff\nlog\n");
        return 0;
    }

    printf("unknown command: %s\n", cmd);
    return 1;
}