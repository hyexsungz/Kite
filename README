# KiteVCS

# KiteVCS is a lightweight version control system written in C.
It is designed as a minimal Git-like engine for local repository management, diff tracking, hashing, and commit simulation.

FEATURES
========

- Repository initialization
- File tracking system
- Diff engine (line-based comparison)
- SHA1-based hashing system
- Commit simulation
- Status and log commands
- Local-only operation (no remote dependency required)

=====

BUILD
=====

Using Makefile:

make

Manual build:
'''
gcc -mconsole -Iinclude -o kite main.c sys/kite_sys.c commands/*.c core/*.c diff/*.c merge/*.c network/*.c hash/*.c hooks/*.c utils/*.c
'''
=====

USAGE
=====
'''
kite init
Initialize repository

kite add <file>
Add file to tracking

kite commit -m "message"
Create commit

kite status
Show repository status

kite diff <file1> <file2>
Compare files

kite log
Show commit history
'''
======

ARCHITECTURE
============

core/
Repository logic, objects, commits

diff/
File comparison engine

hash/
SHA1 hashing system

network/
Remote interface layer (future use)

commands/
CLI command handlers

utils/
Helper functions

hooks/
Pre/post commit logic

======

CONFIGURATION
=============
kite.conf

Used for:
- repository settings
- system behavior flags
- user identity

======

STATUS
======

Early development stage but functional local VCS workflow is working.

======

LICENSE

MIT License
