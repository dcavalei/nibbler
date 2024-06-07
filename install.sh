#!/bin/sh

ln -s "$(git rev-parse --show-toplevel)"/git-hooks/pre-push "$(git rev-parse --show-toplevel)"/.git/hooks
