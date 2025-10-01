	switch (t->back) {
	default: Uerror("bad return move");
	case  0: goto R999; /* nothing to undo */

		 /* PROC :init: */
;
		;
		
	case 4: // STATE 2
		;
		;
		delproc(0, now._nr_pr-1);
		;
		goto R999;

	case 5: // STATE 3
		;
		((P2 *)_this)->i = trpt->bup.oval;
		;
		goto R999;

	case 6: // STATE 9
		;
		((P2 *)_this)->i = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 8: // STATE 11
		;
		;
		delproc(0, now._nr_pr-1);
		;
		goto R999;

	case 9: // STATE 12
		;
		((P2 *)_this)->i = trpt->bup.oval;
		;
		goto R999;

	case 10: // STATE 18
		;
		p_restor(II);
		;
		;
		goto R999;

		 /* PROC phil */
;
		;
		
	case 12: // STATE 2
		;
	/* 0 */	((P1 *)_this)->cur_state = trpt->bup.oval;
		;
		;
		goto R999;

	case 13: // STATE 3
		;
		_m = unsend(((P1 *)_this)->left);
		;
		goto R999;

	case 14: // STATE 4
		;
		((P1 *)_this)->cur_state = trpt->bup.oval;
		;
		goto R999;

	case 15: // STATE 5
		;
	/* 0 */	((P1 *)_this)->cur_state = trpt->bup.oval;
		;
		;
		goto R999;

	case 16: // STATE 6
		;
		_m = unsend(((P1 *)_this)->right);
		;
		goto R999;

	case 17: // STATE 7
		;
		((P1 *)_this)->cur_state = trpt->bup.oval;
		;
		goto R999;

	case 18: // STATE 9
		;
		((P1 *)_this)->cur_state = trpt->bup.ovals[1];
	/* 0 */	((P1 *)_this)->cur_state = trpt->bup.ovals[0];
		;
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;

	case 19: // STATE 10
		;
	/* 0 */	((P1 *)_this)->cur_state = trpt->bup.oval;
		;
		;
		goto R999;

	case 20: // STATE 11
		;
		_m = unsend(((P1 *)_this)->right);
		;
		goto R999;

	case 21: // STATE 12
		;
		((P1 *)_this)->cur_state = trpt->bup.oval;
		;
		goto R999;

	case 22: // STATE 13
		;
	/* 0 */	((P1 *)_this)->cur_state = trpt->bup.oval;
		;
		;
		goto R999;

	case 23: // STATE 14
		;
		_m = unsend(((P1 *)_this)->left);
		;
		goto R999;

	case 24: // STATE 15
		;
		((P1 *)_this)->cur_state = trpt->bup.oval;
		;
		goto R999;

	case 25: // STATE 17
		;
		((P1 *)_this)->cur_state = trpt->bup.ovals[1];
	/* 0 */	((P1 *)_this)->cur_state = trpt->bup.ovals[0];
		;
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;

	case 26: // STATE 21
		;
		p_restor(II);
		;
		;
		goto R999;

		 /* PROC fork */

	case 27: // STATE 1
		;
	/* 0 */	((P0 *)_this)->cur_state_fork = trpt->bup.oval;
		;
		;
		goto R999;

	case 28: // STATE 2
		;
		XX = 1;
		unrecv(((P0 *)_this)->right_phil, XX-1, 0, 1, 1);
		;
		;
		goto R999;

	case 29: // STATE 3
		;
		((P0 *)_this)->cur_state_fork = trpt->bup.oval;
		;
		goto R999;

	case 30: // STATE 4
		;
		XX = 1;
		unrecv(((P0 *)_this)->left_phil, XX-1, 0, 1, 1);
		;
		;
		goto R999;

	case 31: // STATE 5
		;
		((P0 *)_this)->cur_state_fork = trpt->bup.oval;
		;
		goto R999;

	case 32: // STATE 8
		;
	/* 0 */	((P0 *)_this)->cur_state_fork = trpt->bup.oval;
		;
		;
		goto R999;

	case 33: // STATE 9
		;
		XX = 1;
		unrecv(((P0 *)_this)->left_phil, XX-1, 0, 1, 1);
		;
		;
		goto R999;

	case 34: // STATE 10
		;
		((P0 *)_this)->cur_state_fork = trpt->bup.oval;
		;
		goto R999;

	case 35: // STATE 11
		;
	/* 0 */	((P0 *)_this)->cur_state_fork = trpt->bup.oval;
		;
		;
		goto R999;

	case 36: // STATE 12
		;
		XX = 1;
		unrecv(((P0 *)_this)->right_phil, XX-1, 0, 1, 1);
		;
		;
		goto R999;

	case 37: // STATE 13
		;
		((P0 *)_this)->cur_state_fork = trpt->bup.oval;
		;
		goto R999;

	case 38: // STATE 17
		;
		p_restor(II);
		;
		;
		goto R999;
	}

