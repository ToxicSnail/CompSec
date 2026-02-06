#define N 5

mtype = { msgtype };

chan fork_left[N]  = [0] of { mtype };
chan fork_right[N] = [0] of { mtype };

proctype fork(chan left_phil, right_phil)
{
    byte cur_state_fork = 0;

    end:
    do
    :: (cur_state_fork == 0) ->
        if
        :: right_phil?msgtype -> cur_state_fork = 2
        :: left_phil?msgtype  -> cur_state_fork = 1
        fi
    :: (cur_state_fork == 1) ->
        left_phil?msgtype;
        cur_state_fork = 0
    :: (cur_state_fork == 2) ->
        right_phil?msgtype;
        cur_state_fork = 0
    od
}

proctype phil(chan left, right; byte mn)
{
    byte cur_state = 0;

    printf("MSC: phil # %d\n", mn);

    do
    :: (cur_state == 1) ->
        left!msgtype;
        cur_state = 2
    :: (cur_state == 2) ->
        right!msgtype;
        cur_state = 3
    :: (cur_state == 3) ->
        cur_state = 4
    :: (cur_state == 4) ->
        right!msgtype;
        cur_state = 5
    :: (cur_state == 5) ->
        left!msgtype;
        cur_state = 0
    :: (cur_state == 0) ->
        cur_state = 1
    od
}

init
{
    byte i = 0;

    do
    :: i < N ->
        run fork(fork_left[i], fork_right[i]);
        i++
    :: else ->
        break
    od;

    i = 0;
    do
    :: i < N ->
        run phil(fork_right[(i + N - 1) % N], fork_left[i], i);
        i++
    :: else ->
        break
    od
}
