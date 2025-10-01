#define N 5

mtype = { thinking, hungry, eating };

byte fork[N];            /* 0 means the fork is on the table, 1 means taken */
mtype state[N];          /* current state of each philosopher */

inline take_left(id, left)
{
    atomic {
        if
        :: fork[left] == 0 ->
            fork[left] = 1;
            printf("P%d took left fork %d\n", id, left)
        fi
    }
}

inline take_right(id, right)
{
    atomic {
        if
        :: fork[right] == 0 ->
            fork[right] = 1;
            printf("P%d took right fork %d\n", id, right)
        fi
    }
}

inline put_down(id, left, right)
{
    atomic {
        fork[left] = 0;
        fork[right] = 0;
        state[id] = thinking;
        printf("P%d released forks %d and %d\n", id, left, right)
    }
}

proctype Philosopher(byte id)
{
    byte left = id;
    byte right = (id + 1) % N;

    do
    :: printf("P%d starts thinking\n", id);
       state[id] = thinking;

       state[id] = hungry;
       printf("P%d is hungry\n", id);

       take_left(id, left);
       /* at this point the philosopher holds the left fork and can block */

       take_right(id, right);

       state[id] = eating;
       progress: printf("P%d eats\n", id);

       put_down(id, left, right)
    od
}

init
{
    byte i = 0;

    atomic {
        do
        :: i < N ->
            state[i] = thinking;
            run Philosopher(i);
            i++
        :: else ->
            break
        od
    }
}

#define SOMEONE_EATING (state[0] == eating || state[1] == eating || state[2] == eating || state[3] == eating || state[4] == eating)

ltl no_deadlock { []<>(SOMEONE_EATING) }