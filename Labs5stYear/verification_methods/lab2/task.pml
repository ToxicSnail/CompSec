#define fin  (all_right_side == true)   /* все на правом берегу */
#define wg   (w_and_g == false)         /* волк не остался с козой без фермера */
#define gc   (g_and_c == false)         /* коза не осталась с капустой без фермера */

bool all_right_side, w_and_g, g_and_c;

active proctype river()
{
    /* 0 = левый берег, 1 = правый берег */
    bit f = 0, w = 0, g = 0, c = 0;

    do
    :: (f == 1) && (f == w) && (f == g) && (f == c) ->
         all_right_side = true;      /* цель достигнута */
         break

    :: else ->
        /* фермер перевозит одного из: волка | козу | капусту | никого */
        if
        :: (f == w) -> f = 1 - f; w = 1 - w
        :: (f == g) -> f = 1 - f; g = 1 - g
        :: (f == c) -> f = 1 - f; c = 1 - c
        :: else     -> f = 1 - f
        fi;

        /* фиксируем «плохие» ситуации, если фермер ушёл */
        if
        :: (f != g) && (g == c) -> g_and_c = true   /* коза с капустой без фермера */
        :: (f != w) && (w == g) -> w_and_g = true   /* волк с козой без фермера    */
        :: else -> skip
        fi
    od
}

never {
    do
    :: (fin && wg && gc) -> break
    :: else -> skip
    od
}
