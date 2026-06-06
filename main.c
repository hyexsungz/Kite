#include <stdio.h>
#include <string.h>

int cmd_init(int argc, char **argv);
int cmd_add(int argc, char **argv);
int cmd_commit(int argc, char **argv);
int cmd_status(int argc, char **argv);
int cmd_log(int argc, char **argv);
int cmd_branch(int argc, char **argv);
int cmd_checkout(int argc, char **argv);
int cmd_merge(int argc, char **argv);
int cmd_clone(int argc, char **argv);
int cmd_fetch(int argc, char **argv);
int cmd_push(int argc, char **argv);
int cmd_pull(int argc, char **argv);
int cmd_remote(int argc, char **argv);
int cmd_diff(int argc, char **argv);
int cmd_stash(int argc, char **argv);
int cmd_tag(int argc, char **argv);

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("kite\n");
        return 1;
    }

    if (strcmp(argv[1], "init") == 0) return cmd_init(argc, argv);
    if (strcmp(argv[1], "add") == 0) return cmd_add(argc, argv);
    if (strcmp(argv[1], "commit") == 0) return cmd_commit(argc, argv);
    if (strcmp(argv[1], "status") == 0) return cmd_status(argc, argv);
    if (strcmp(argv[1], "log") == 0) return cmd_log(argc, argv);
    if (strcmp(argv[1], "branch") == 0) return cmd_branch(argc, argv);
    if (strcmp(argv[1], "checkout") == 0) return cmd_checkout(argc, argv);
    if (strcmp(argv[1], "merge") == 0) return cmd_merge(argc, argv);
    if (strcmp(argv[1], "clone") == 0) return cmd_clone(argc, argv);
    if (strcmp(argv[1], "fetch") == 0) return cmd_fetch(argc, argv);
    if (strcmp(argv[1], "push") == 0) return cmd_push(argc, argv);
    if (strcmp(argv[1], "pull") == 0) return cmd_pull(argc, argv);
    if (strcmp(argv[1], "remote") == 0) return cmd_remote(argc, argv);
    if (strcmp(argv[1], "diff") == 0) return cmd_diff(argc, argv);
    if (strcmp(argv[1], "stash") == 0) return cmd_stash(argc, argv);
    if (strcmp(argv[1], "tag") == 0) return cmd_tag(argc, argv);

    printf("unknown\n");
    return 1;
}
