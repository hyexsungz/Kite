#ifndef HOOKS_H
#define HOOKS_H

typedef enum {
    HOOK_PRE_COMMIT,
    HOOK_POST_COMMIT,
    HOOK_PRE_PUSH,
    HOOK_POST_PUSH,
    HOOK_PRE_MERGE,
    HOOK_POST_MERGE,
    HOOK_PRE_REBASE,
    HOOK_POST_REBASE
} kite_hook_type;

int hook_run(kite_hook_type type);

#endif