#!/bin/sh
tmux new-session -d -s blackjack_ring \; \
    rename-window 'Blackjack Ring' \; \
    send-keys 'clear' C-m 'python3 blackjack.py 0' C-m \; \
    split-window -v \; \
    send-keys 'clear' C-m 'python3 blackjack.py 1' C-m \; \
    split-window -h \; \
    send-keys 'clear' C-m 'python3 blackjack.py 2' C-m \; \
    select-pane -t 0 \; \
    split-window -h \; \
    send-keys 'clear' C-m 'python3 blackjack.py 3' C-m \; \
    select-layout tiled \; \
    attach-session -t blackjack_ring
